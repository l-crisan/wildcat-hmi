/*
   Copyright (C) 2010-2026 by Laurentiu-Gheorghe Crisan
   Copyright (C) 2010-2026 by ATESiON GmbH
   Copyright (C) 2026 Marc Boris Duerner

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  As a special exception, you may use this file as part of a free
  software library without restriction. Specifically, if other files
  instantiate templates or use macros or inline functions from this
  file, or you compile this file and link it with other files to
  produce an executable, this file does not by itself cause the
  resulting executable to be covered by the GNU General Public
  License. This exception does not however invalidate any other
  reasons why the executable file might be covered by the GNU Library
  General Public License.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the:
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301 USA
*/
#include "ScreenImpl.h"
#include "ApplicationImpl.h"
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/Cursor.h>
#include <wildcat/gui/window/PaintEvent.h>
#include "../WindowFrame.h"
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <Pt/System/Logger.h>
#include <Pt/System/Clock.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <algorithm>
#include <cmath>
#include <cstdint>

#if defined(__wasm_simd128__)
#include <wasm_simd128.h>
#endif

namespace wildcat {
namespace gui {
namespace window {

// avoids emscripten_enter_soft_fullscreen's WebGL render-target resize path,
// which crashes when proxied to the main thread under -sPROXY_TO_PTHREAD=1
EM_BOOL onWindowResized(int /*eventType*/, const EmscriptenUiEvent* /*uiEvent*/, void* /*userData*/)
{
    ScreenImpl::resizeCanvasToWindow();
    return true;
}


ScreenImpl::ScreenImpl(ApplicationImpl& app)
: _parent(0)
, _screenScaling(1)
, _scaleFactor(1)
{
    makeCanvasFullscreen("canvas");
    
	double width, height;
	emscripten_get_element_css_size("canvas", &width, &height);
    emscripten_set_canvas_element_size("canvas", int(width), int(height));

    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, onWindowResized);

    gfx::Size size(width, height);
    _graphics.reset(size);
                             
    const gfx::Pixmap& image = _graphics.image();
  

    Base::setGraphics(&_graphics, gfx::Point(0, 0));

    repaint(gfx::Rect(gfx::Point(0, 0), _graphics.size()));
}

ScreenImpl::~ScreenImpl()
{
    //Clear the screen
    _commitDamage = gfx::Rect(_graphics.size());

    _graphics.setCompositionMode(gfx::CompositionMode::SourceCopy);
    _graphics.setBrush(gfx::ArgbColor(0,0,0));

    _graphics.fillRect(_commitDamage);
       
    commitFrame();

    //Unregister 
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, 0);

    //Reset graphics
    Base::setGraphics(nullptr, gfx::Point(0, 0));

    //Reset parent
    setParent(0);

}

void ScreenImpl::resizeCanvasToWindow()
{
    double width, height;
    emscripten_get_element_css_size("canvas", &width, &height);
    emscripten_set_canvas_element_size("canvas", int(width), int(height));
   
    Screen&  screen = Application::inst().screen();
    const double sc= screen.scaleFactor();

    gfx::Size size(width, height);
    size.scale(sc);

    gui::window::ResizeEvent rev(screen, size);
    gui::window::Application::instance().processEvent(rev);

    gfx::Rect updateRect(gfx::Point(0, 0), size);
    screen.repaint(updateRect);
}


void ScreenImpl::blitCanvasImage(const char* selector, const unsigned char* pixels, int width, int height,
                                 int dx, int dy)
{
    MAIN_THREAD_EM_ASM({
        var canvas = document.querySelector(UTF8ToString($0));
        var ctx = canvas.ptContext2d || (canvas.ptContext2d = canvas.getContext("2d", { alpha: false }));
        var w = $2;
        var h = $3;
        var dx = $4;
        var dy = $5;
        var cw = canvas.width;
        var ch = canvas.height;

        var full = canvas.ptFullData;
        if (!full || full.w !== cw || full.h !== ch)
        {
            var buffer = new ArrayBuffer(cw * ch * 4);
            var offscreen = document.createElement("canvas");
            offscreen.width = cw;
            offscreen.height = ch;
            var offCtx = offscreen.getContext("2d", { alpha: false });

            // Object literal avoided: unparenthesized top-level commas would split the EM_ASM macro argument
            full = {};
            full.fastData = new Uint8Array(buffer);
            full.imgData = new ImageData(new Uint8ClampedArray(buffer), cw, ch);
            full.offscreen = offscreen;
            full.offCtx = offCtx;
            full.w = cw;
            full.h = ch;

            canvas.ptFullData = full;
            canvas.ptDirtyRects = [];
        }

        var x0 = Math.max(0, dx);
        var y0 = Math.max(0, dy);
        var x1 = Math.min(cw, dx + w);
        var y1 = Math.min(ch, dy + h);
        if(x1 <= x0 || y1 <= y0) return;

        var copyW = x1 - x0;
        var copyH = y1 - y0;
        var skipX = x0 - dx;
        var skipY = y0 - dy;

        // 1. ABSOLUT WICHTIG: Synchron kopieren, bevor C++ den Puffer (_canvasBuffer)
        // für das nächste Rechteck in Millisekunden überschreibt!
        var srcMem = HEAPU8;
        var dstMem = full.fastData;

        if (copyW === w && copyW === cw) {
            var srcPtr = $1 + (skipY * w) * 4;
            var dstPtr = y0 * cw * 4;
            dstMem.set(srcMem.subarray(srcPtr, srcPtr + copyH * w * 4), dstPtr);
        } else {
            var srcStride = w * 4;
            var dstStride = cw * 4;
            var rowBytes = copyW * 4;
            var srcPtr = $1 + ((skipY * w) + skipX) * 4;
            var dstPtr = (y0 * cw + x0) * 4;

            for(var y = 0; y < copyH; ++y) {
                dstMem.set(srcMem.subarray(srcPtr, srcPtr + rowBytes), dstPtr);
                srcPtr += srcStride;
                dstPtr += dstStride;
            }
        }

        // 2. Synchron in den unsichtbaren Offscreen-Puffer schreiben
        full.offCtx.putImageData(full.imgData, 0, 0, x0, y0, copyW, copyH);

        // 3. Atomares Flip-Scheduling: C++ ruft commitFrame nur einmal pro
        //    Event-Loop-Iteration auf, daher reicht ein einzelnes rAF-Posting
        if (!canvas.ptRafPending)
        {
            canvas.ptRafPending = true;
            requestAnimationFrame(function() {
                canvas.ptRafPending = false;
                ctx.drawImage(canvas.ptFullData.offscreen, 0, 0);
            });
        }
    }, selector, pixels, width, height, dx, dy);
}

void ScreenImpl::makeCanvasFullscreen(const char* selector)
{
    MAIN_THREAD_EM_ASM({
        var canvas = document.querySelector(UTF8ToString($0));
        document.body.style.margin = "0";
        document.body.style.overflow = "hidden";
        canvas.style.position = "fixed";
        canvas.style.top = "0";
        canvas.style.left = "0";
        canvas.style.width = "100vw";
        canvas.style.height = "100vh";
        canvas.style.display = "block";
        }, selector);
}

void ScreenImpl::setParent(Screen* screen)
{
    _parent = screen;

    if(_parent)
    {
        double width, height;
        emscripten_get_element_css_size("canvas", &width, &height);

        gfx::Size size(static_cast<int>(width), static_cast<int>(height));
        size.scale(1.0/scaleFactor());

        _parent->onResize(*this, size);
        _parent->onShow(*this, true);
    }

    onSetParent(_parent);
}

void ScreenImpl::addWindow(Window& w)
{
    w.setParent(*this);
}

void ScreenImpl::removeWindow(Window& w)
{ 
    w.unparent();
}

const std::vector<Window*>& ScreenImpl::windows() const
{
    return Base::windows();
}

WindowManagerBase& ScreenImpl::windowManager()
{
    return *this;
}

void ScreenImpl::setCapture(Element* capture)
{
}

Element* ScreenImpl::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    if( ! bounds().contains(p) )
        return nullptr;

    Element* hit = Base::onHitTest(p, hint);

    if(hit)
        return hit;

    return nullptr;
}

gfx::Point ScreenImpl::onToParent(const gfx::Point& pos) const
{
    if( ! _parent )
        return pos;

    return _parent->toParent(pos);
}

gfx::Point ScreenImpl::onFromParent(const gfx::Point& pos) const
{
    if( ! _parent )
        return pos;

    return _parent->fromParent(pos);
}

bool ScreenImpl::onProcessEvent(const Pt::Event& ev)
{
    return Base::onProcessEvent(ev);
}

void ScreenImpl::onRequestRepaint(const gfx::Rect& rect)
{
    if(_parent)
        _parent->repaint(rect);
}

void ScreenImpl::onRequestResize(const gfx::Size& s)
{
    if(_parent)
        _parent->onResize(*this, s);
}

void ScreenImpl::onProcessEnableEvent(const EnableEvent& ev)
{
    Base::onProcessEnableEvent(ev);
}

void ScreenImpl::onEnable(bool e)
{
    Element::onEnable(e);
}

bool ScreenImpl::onProcessMouseEvent(const MouseEvent& ev)
{
    return Base::onProcessMouseEvent(ev);
}

bool ScreenImpl::onMouseEvent(const MouseEvent& ev)
{ 
    return Base::onMouseEvent(ev);
}

bool ScreenImpl::onProcessTouchEvent(const TouchEvent& ev)
{ 
    return Base::onProcessTouchEvent(ev);
}

bool ScreenImpl::onTouchEvent(const TouchEvent& ev)
{ 
    return Base::onTouchEvent(ev);
}

bool ScreenImpl::onProcessScrollEvent(const ScrollEvent& ev)
{
    if( ! isEnabled() )
        return false;
  
    return Base::onProcessScrollEvent(ev);
}

bool ScreenImpl::onScrollEvent(const ScrollEvent& ev)
{
    return Base::onScrollEvent(ev);
}

bool ScreenImpl::onProcessKeyEvent(const KeyEvent& ev)
{
    if( ! isEnabled() )
        return false;
    
    return Base::onProcessKeyEvent(ev);
}

bool ScreenImpl::onKeyEvent(const KeyEvent& ev)
{
    return Base::onKeyEvent(ev);
}

void ScreenImpl::onUpdateScaling()
{
    _graphics.setScaleFactor(_scaleFactor);

    double width, height;
    emscripten_get_element_css_size("canvas", &width, &height);

    gfx::Size logical(static_cast<int>(width), static_cast<int>(height));
    logical.scale(1.0/_scaleFactor);

    _graphics.reset(logical);
    _commitDamage.clear();

    if (_parent)
        _parent->onResize(*this, logical);

    Base::onUpdateScaling();
}

void ScreenImpl::updateCanvasBuffer(const gfx::Pixmap& image, std::size_t x0, std::size_t y0,
    std::size_t width, std::size_t height)
{
    const std::size_t srcStride = image.lineSize();
    const std::size_t dstStride = width * 4;

    _canvasBuffer.resize(dstStride * height);

    const Pt::uint8_t* src = image.data();
    Pt::uint8_t* dst = _canvasBuffer.data();

#if defined(__wasm_simd128__)
    // Maske, um das h chste Byte (Alpha im Little-Endian-Format) auf 0xFF zu setzen
    const v128_t alpha_mask = wasm_i32x4_splat(0xFF000000u);    
#endif

    for (std::size_t y = 0; y < height; ++y)
    {
        const Pt::uint8_t* srcRow = src + (y0 + y) * srcStride + x0 * 4;
        Pt::uint8_t* dstRow = dst + y * dstStride;

        std::size_t x = 0;

#if defined(__wasm_simd128__)
        for (; x + 4 <= width; x += 4)
        {
            v128_t pixels = wasm_v128_load(srcRow + x * 4);

            // 1-Source Shuffle: nur R und B swappen, Alpha (Indizes 3, 7, 11, 15) ignorieren/behalten.
            //  bersetzt sich nativ in genau 1x pshufb (x86).
            v128_t swapped = wasm_i8x16_shuffle(pixels, pixels,
                2, 1, 0, 3,
                6, 5, 4, 7,
                10, 9, 8, 11,
                14, 13, 12, 15);

            // Alpha per OR fixieren.  bersetzt sich nativ in 1x por.
            v128_t out = wasm_v128_or(swapped, alpha_mask);

            wasm_v128_store(dstRow + x * 4, out);
        }
#endif

        for (; x < width; ++x)
        {
            Pt::uint32_t v;
            std::memcpy(&v, srcRow + x * 4, sizeof(v));

            Pt::uint32_t o = 0xFF000000u
                | (v & 0x0000FF00u)
                | ((v & 0x000000FFu) << 16)
                | ((v & 0x00FF0000u) >> 16);

            std::memcpy(dstRow + x * 4, &o, sizeof(o));
        }
    }
}


void ScreenImpl::onResizeEvent(const ResizeEvent& ev)
{
    if( size() == ev.size() )
        return;

    Base::onResizeEvent(ev);

    _graphics.reset(ev.size());

    _commitDamage.clear();
}

void ScreenImpl::onProcessPaintEvent(const PaintEvent& ev)
{
    const gfx::Rect& rect = ev.rect();

    if (rect.isNull())
        return;

    Base::onProcessPaintEvent(ev);

    _commitDamage.unify(rect);

}

void ScreenImpl::commitFrame()
{
    if (_commitDamage.isNull())
        return;
    
    gfx::Rect updateRectL = _commitDamage;
    updateRectL.scale(_scaleFactor);
    gfx::Pixmap& image = _graphics.image(); 

    const long imgWidth  = static_cast<long>(image.width());
    const long imgHeight = static_cast<long>(image.height());

    // expand fractional edges outward by 1px so no seam/stale-pixel line remains
    const long x0 = std::max<long>(0, updateRectL.x());
    const long y0 = std::max<long>(0, updateRectL.y());
    const long x1 = std::min<long>(imgWidth, updateRectL.x() + updateRectL.width());
    const long y1 = std::min<long>(imgHeight, updateRectL.y() + updateRectL.height());

    if (x1 <= x0 || y1 <= y0)
    {
        _commitDamage.clear();
        return;
    }

    const std::size_t dirtyWidth = static_cast<std::size_t>(x1 - x0);
    const std::size_t dirtyHeight = static_cast<std::size_t>(y1 - y0);

    updateCanvasBuffer(image, static_cast<std::size_t>(x0), static_cast<std::size_t>(y0), dirtyWidth, dirtyHeight);


    
    blitCanvasImage("canvas", _canvasBuffer.data(), int(dirtyWidth), int(dirtyHeight), int(x0), int(y0));

    _commitDamage.clear();
}

void ScreenImpl::onPaintEvent(const PaintEvent& ev)
{    

    Base::onPaintEvent(ev);

    const gfx::Rect& rect = ev.rect();

    onPaint(_graphics, rect);
    

}

void ScreenImpl::onMove(WindowFrame& frame, const gfx::Point& pos)
{
    gfx::Point aligedPos = pos;

    if (aligedPos.x() < 0)
        return;

    if (aligedPos.y() < 0)
        return;

    if (aligedPos.x() > size().width() - 20)
        return;

    if (aligedPos.y() > size().height() - 10)
        return;
        
    MoveEvent mev(frame, aligedPos);
    Application::instance().commitEvent(mev);
}

void ScreenImpl::onPaint(gfx::Graphics& surface, const gfx::Rect& rect)
{ 
    surface.setCompositionMode(gfx::CompositionMode::SourceCopy);    
    surface.setBrush(gfx::Color(0, (int)SystemColors::ColorType::Desktop).getColor());
    surface.fillRect(rect);    
}

}}}
