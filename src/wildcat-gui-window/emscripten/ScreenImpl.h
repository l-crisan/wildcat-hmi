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
#ifndef WILDCAT_GUI_WINDOW_SCREENIMPL_H
#define WILDCAT_GUI_WINDOW_SCREENIMPL_H

#include <wildcat/gui/window/WindowManager.h>
#include <wildcat/gui/gfx/Color.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <Pt/Event.h>
#include <emscripten/html5.h>

namespace wildcat {
namespace gui {
namespace window   {

class ApplicationImpl;
class Cursor;
class Screen;

class ScreenImpl : public WindowManager
{
        typedef WindowManager Base;

        friend class Screen;
        friend EM_BOOL onWindowResized(int /*eventType*/, const EmscriptenUiEvent* /*uiEvent*/, void* /*userData*/);

    public:
        ScreenImpl(ApplicationImpl& app);

        virtual ~ScreenImpl();


        void setParent(Screen* screen);


        void addWindow(Window& w);

        void removeWindow(Window& w);


        const std::vector<Window*>& windows() const;

        WindowManagerBase& windowManager();


        void setCapture(Element* capture);

        void output(const gfx::Rect& rect);


        bool commitPending() const
        {
            return !_commitDamage.isNull();
        }

        void commitFrame();

        void setScaleFactor(double v)
        {
            
            _scaleFactor = v;
            updateScaling();
            
        }

        double scaleFactor() const
        {
            return _scaleFactor;
        }


        // Responder
    protected:
        virtual bool onMouseEvent(const MouseEvent& ev);

        virtual bool onTouchEvent(const TouchEvent& ev);

        virtual bool onScrollEvent(const ScrollEvent& ev);

        virtual bool onKeyEvent(const KeyEvent& ev);

        // Element
    protected:
        virtual Element* onHitTest(const gfx::Point& pos, HitTestHint hint);

        virtual gfx::Point onToParent(const gfx::Point& pos) const;

        virtual gfx::Point onFromParent(const gfx::Point& pos) const;

        virtual bool onProcessEvent(const Pt::Event& ev);

        virtual void onRequestResize(const gfx::Size& s);

        virtual void onRequestRepaint(const gfx::Rect& rect);

        virtual void onUpdateScaling();

        virtual void onMove(WindowFrame& w, const gfx::Point& to);
        // scaling
    protected:

        virtual void onResizeEvent(const ResizeEvent& ev);

        // enable
    protected:
        void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onEnable(bool e);

        // painting
    protected:
        virtual void onProcessPaintEvent(const PaintEvent& ev);

        virtual void onPaintEvent(const PaintEvent& ev);

        virtual void onPaint(gfx::Graphics& surface,
            const gfx::Rect& updateRect);

        // input
    protected:
        bool onProcessMouseEvent(const MouseEvent& ev);

        bool onProcessTouchEvent(const TouchEvent& ev);

        bool onProcessScrollEvent(const ScrollEvent& ev);

        bool onProcessKeyEvent(const KeyEvent& ev);

    private:
        static void resizeCanvasToWindow();

        static void makeCanvasFullscreen(const char* selector);

        static void blitCanvasImage(const char* selector, const unsigned char* pixels,
            int width, int height, int dx, int dy);

    private:
        void updateCanvasBuffer(const gfx::Pixmap& image, std::size_t x0, std::size_t y0, std::size_t width, std::size_t height);

    private:
        Screen* _parent;
        gfx::PixmapGraphics  _graphics;
        double               _screenScaling;
        std::vector<Pt::uint8_t> _canvasBuffer;
        gfx::Rect               _commitDamage;
        double _scaleFactor;
};

}}}

#endif
