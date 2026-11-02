/*
   Copyright (C) 2017 Marc Boris Duerner
   Copyright (C) 2010-2026 by Laurentiu-Gheorghe Crisan
   Copyright (C) 2010-2026 by ATESiON GmbH

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
#ifndef WILDCAT_GUI_WINDOW_WIDGET_H
#define WILDCAT_GUI_WINDOW_WIDGET_H

#include <wildcat/gui/window/Control.h>
#include <wildcat/gui/window/SystemColors.h>
#include <wildcat/gui/gfx/Brush.h>
#include <wildcat/gui/gfx/Pen.h>
#include <wildcat/gui/gfx/Font.h>
#include <wildcat/gui/gfx/Rect.h>
#include <wildcat/gui/gfx/Polygon.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <wildcat/gui/window/DragAndDrop.h>
#include <Pt/System/Clock.h>
#include <Pt/Any.h>

namespace wildcat {
namespace gui {
namespace window {

enum class EchoMode
{
    Normal,
    Hidden
};

typedef Control WidgetBase;

class WILDCAT_GUI_WINDOW_API Widget : public WidgetBase
{
     public:
        Widget();

        virtual ~Widget();

        bool isHighlighted() const;



        const gfx::ArgbColor& aControlColor() const
        {
            return _colors.aControl();

        }

        const gfx::Color& controlColor() const
        {
            return _colors.control();
        }

        void setControlColor(const gfx::Color& v) 
        {
            _colors.setControl(v);
            Pt::Any a(v);
            _controlColorChanged.send(*this,a);   
            invalidate();
        }


        const gfx::ArgbColor& aControlDarkColor() const
        {
            return _colors.aControlDark();
        }

        const gfx::Color& controlDarkColor() const
        {         
            return _colors.controlDark();
        }

        void setControlDarkColor(const gfx::Color& v)
        {
            _colors.setControlDark(v);
            Pt::Any a(v);
            _controlDarkColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aControlDarkDarkColor() const
        {
            return _colors.aControlDarkDark();
        }

        const gfx::Color& controlDarkDarkColor() const
        {
            return _colors.controlDarkDark();
        }

        void setControlDarkDarkColor(const gfx::Color& v)
        {
            _colors.setControlDarkDark(v);
            Pt::Any a(v);
            _controlDarkDarkColorChanged.send(*this, a);
            invalidate();
        }

        
        const gfx::ArgbColor& aControlLightColor() const
        {
            return _colors.aControlLight();
        }

        const gfx::Color& controlLightColor() const
        {            
            return _colors.controlLight();
        }

        void setControlLightColor(const gfx::Color& v)
        {
            _colors.setControlLight(v);
            Pt::Any a(v);
            _controlLightColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aControlLightLightColor() const
        {
            return _colors.aControlLightLight();
        }

        const gfx::Color& controlLightLightColor() const
        {            
            return _colors.controlLightLight();
        }

        void setControlLightLightColor(const gfx::Color& v)
        {
            _colors.setControlLightLight(v);
            Pt::Any a(v);
            _controlLightLightColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aControlDisabledColor() const
        {
            return _colors.aControlDisabled();
        }

        const gfx::Color& controlDisabledColor() const
        {            
            return _colors.controlDisabled();
        }

        void setControlDisabledColor(const gfx::Color& v)
        {
            _colors.setControlDisabled(v);
            Pt::Any a(v);
            _controlDisabledColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aHighlightColor() const
        {            
            return _colors.aControlHighlight();
        }

        const gfx::Color& highlightColor() const
        {
            return _colors.controlHighlight();
        }

        void setHighlightColor(const gfx::Color& v)
        {
            _colors.setControlHighlight(v);
            Pt::Any a(v);
            _highlightColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aFrameColor() const
        {            
            return _colors.aControlFrame();
        }

        const gfx::Color& frameColor() const
        {
            return _colors.controlFrame();
        }

        void setFrameColor(const gfx::Color& v)
        {
            _colors.setControlFrame(v);
            Pt::Any a(v);
            _frameColorChanged.send(*this, a);
            invalidate();
        }
       

        const gfx::ArgbColor& aFrameDisabledColor() const
        {
            return _colors.aControlFrameDisabled();
        }

        const gfx::Color& frameDisabledColor() const
        {            
            return _colors.controlFrameDisabled();
        }

        void setFrameDisabledColor(const gfx::Color& v)
        {
            _colors.setControlFrameDisabled(v);
            Pt::Any a(v);
            _frameDisabledColorChanged.send(*this, a);
            invalidate();
        }
       

        const gfx::ArgbColor& aInfoColor() const
        {
            return _colors.aInfo();
        }

        const gfx::Color& infoColor() const
        {            
            return _colors.info();
        }

        void setInfoColor(const gfx::Color& v)
        {
            _colors.setInfo(v);
            Pt::Any a(v);
            _infoColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aInfoTextColor() const
        {
            return _colors.aInfoText();
        }

        const gfx::Color& infoTextColor() const
        {            
            return _colors.infoText();
        }

        void setInfoTextColor(const gfx::Color& v)
        {
            _colors.setInfoText(v);
            Pt::Any a(v);
            _infoTextColorChanged.send(*this, a);
            invalidate();
        }



        const gfx::ArgbColor& aTextColor() const
        {
            return _colors.aControlText();
        }

        const gfx::Color& textColor() const
        {            
            return _colors.controlText();
        }

        void setTextColor(const gfx::Color& v)
        {
            _colors.setControlText(v);
            Pt::Any a(v);
            _textColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aTextHighlightColor() const
        {
            return _colors.aControlTextHighlight();
        }

        const gfx::Color& textHighlightColor() const
        {            
            return _colors.controlTextHighlight();
        }

        void setTextHighlightColor(const gfx::Color& v)
        {
            _colors.setControlTextHighlight(v);
            Pt::Any a(v);
            _textHighlightColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aTextBackgroundColor() const
        {            
            return _colors.aTextBackground();
        }

        const gfx::Color& textBackgroundColor() const
        {
            return _colors.textBackground();
        }

        void setTextBackgroundColor(const gfx::Color& v)
        {
            _colors.setTextBackground(v);
            Pt::Any a(v);
            _textBackgroundColorChanged.send(*this, a);
            invalidate();
        }


        const gfx::ArgbColor& aTextDisabledColor() const
        {
            return _colors.aTextDisabled();
        }   

        const gfx::Color& textDisabledColor() const
        {
            return _colors.textDisabled();
        }

        void setTextDisabledColor(const gfx::Color& v)
        {
            _colors.setTextDisabled(v);
            Pt::Any a(v);
            _textDisabledColorChanged.send(*this, a);
            repaint();
        }


        SystemColors & colors()
        {
            return _colors;
        }   

        const SystemColors& colors() const
        {
            return _colors;
        }

        const gfx::Font& font() const;
        

        void setFont(const gfx::Font& v);

        void mv(int x, int y)
        {
            gui::window::Widget::move(gfx::Point(x, y));
        }

        void rs(int width, int height)
        {
            gui::window::Widget::resize(gfx::Size(width, height));
        }


        void setDetectDrag(bool v)
        {
            _detectDrag = onSetDetectDrag(v);
            Pt::Any a(v);
            _detectDragChanged.send(*this, a);
        }

        bool detectDrag() const
        {
            return _detectDrag;
        }

        const Control* getDragItem() const
        {
            return onGetDragItem();
        }


        bool hasBackground() const
        {
            return _hasBackground;
        }

        void setHasBackground(bool v)
        {
            _hasBackground = v;
            Pt::Any a(v);
            _hasBackgroundChanged.send(*this, a);  
            repaint();
        }

        bool hasFrame() const
        {
            return _hasFrame;
        }

        void setHasFrame(bool v)
        {
            _hasFrame = v;
            Pt::Any a(v);
            _hasFrameChanged.send(*this, a);
            repaint();
        }

        bool roundedFrame() const
        {
            return _roundedFrame;
        }

        void setRoundedFrame(bool v)
        {
            _roundedFrame = v;
            Pt::Any a(v);
            _roundedFrameChanged.send(*this, a);
            repaint();
        }

        int corner() const
        {
            return _roundedFrame ? 3: 0 ;
        }

        bool  isErrorState() const
        {
            return _errorState;
        }

        void setErrorState(bool v, const Pt::String& message = "");
        

    public: //Signals
        wildcat::gui::base::Signal& beginDrag()
        {
            return _beginDrag;
        }

        wildcat::gui::base::Signal& draging()
        {
            return _draging;
        }

        wildcat::gui::base::Signal& drop()
        {
            return _drop;
        }

        wildcat::gui::base::Signal& doubleClickSignal()
        {
            return _doubleClickSignal;  
        }

        wildcat::gui::base::Signal&  slowDoubleClickSignal()
        {
            return _slowDoubleClickSignal;
        }   

        wildcat::gui::base::Signal& focusChanged()
        {
            return _focusChanged;
        }


        wildcat::gui::base::Signal& controlColorChanged()
        {
            return _controlColorChanged;
        }

        wildcat::gui::base::Signal& controlDarkColorChanged()
        {
            return _controlDarkColorChanged;
        }

        wildcat::gui::base::Signal& controlDarkDarkColorChanged()
        {
            return _controlDarkDarkColorChanged;
        }

        wildcat::gui::base::Signal& controlLightColorChanged()
        {
            return _controlLightColorChanged;
        }

        wildcat::gui::base::Signal& controlLightLightColorChanged()
        {
            return _controlLightLightColorChanged;
        }

        wildcat::gui::base::Signal& controlDisabledColorChanged()
        {
            return _controlDisabledColorChanged;
        }

        wildcat::gui::base::Signal& highlightColorChanged()
        {
            return _highlightColorChanged;
        }

        wildcat::gui::base::Signal& frameColorChanged()
        {
            return _frameColorChanged;
        }

        wildcat::gui::base::Signal& frameDisabledColorChanged()
        {
            return _frameDisabledColorChanged;
        }

        wildcat::gui::base::Signal& infoColorChanged()
        {
            return _infoColorChanged;
        }

        wildcat::gui::base::Signal& infoTextColorChanged()
        {
            return _infoTextColorChanged;
        }

        wildcat::gui::base::Signal& textColorChanged()
        {
            return _textColorChanged;
        }

        wildcat::gui::base::Signal& textHighlightColorChanged()
        {
            return _textHighlightColorChanged;
        }

        wildcat::gui::base::Signal& textBackgroundColorChanged()
        {
            return  _textBackgroundColorChanged;    
        }

        wildcat::gui::base::Signal& textDisabledColorChanged()
        {
            return _textDisabledColorChanged;
        }

        wildcat::gui::base::Signal& hasBackgroundChanged()
        {
            return _hasBackgroundChanged;
        }

        wildcat::gui::base::Signal& hasFrameChanged()
        {
            return _hasFrameChanged;
        }

        wildcat::gui::base::Signal& roundedFrameChanged()
        {
            return _roundedFrameChanged;
        }

        wildcat::gui::base::Signal& fontChanged()
        {
            return _fontChanged;
        }

        wildcat::gui::base::Signal& detectDragChanged()
        {
            return _detectDragChanged;
        }

    protected:
        virtual void onInvalidate();

        virtual void onLayout(const gfx::Rect& rect);

        virtual void onPaint(gfx::Graphics& graphics, const gfx::Rect& rect);        

        virtual gfx::Size onMeasure(const gfx::Size& parentSize);

    protected:
        virtual bool onDoubleClickEvent(const gui::window::MouseEvent& ev);

        virtual bool onDoubleClickSlowEvent(const gui::window::MouseEvent& ev);

        void onPaintEvent(const PaintEvent& ev);

        virtual void onMoveEvent(const MoveEvent& ev);

        virtual void onResizeEvent(const ResizeEvent& ev);

        virtual void onFocusEvent(const FocusEvent& ev);

        virtual bool onEnterEvent( const EnterEvent& ev );

        virtual bool onLeaveEvent(const LeaveEvent& ev );

        virtual bool onMouseEvent(const gui::window::MouseEvent& ev);

        virtual void onBeginDrag(const gfx::Point& pos)
        {
            Pt::Any a(pos);
            _beginDrag.send(*this, a);
        }

        virtual void onDraging(const gfx::Point& pos)
        {   
            Pt::Any a(pos);
            _draging.send(*this,a);
        }

        virtual void onDrop(const gfx::Point& pos)
        {
            Pt::Any a(pos);
            _drop.send(*this, a);
        }

        virtual bool onSetDetectDrag(bool v)
        {
            return v;
        }

        virtual const Control* onGetDragItem() const
        {
            return this;
        }


        virtual const gfx::Font& onGetFont() const
        {
            return _font;   
        }

        virtual void onSetFont(const gfx::Font& v)
        {
            _font = v;
            Pt::Any a(v);
            _fontChanged.send(*this, a);
            repaint();
        }

    private:
        class DoubleClickInfo
        {
            public:
                DoubleClickInfo()
                : clock()
                , running(false)
                {
                }

                Pt::System::Clock clock;
                bool running;
        };


     
    private:
        bool evalDoubleClick(const gui::window::MouseEvent& ev, DoubleClickInfo& info);

        void onErrorStateChanged();
        
    protected:
        std::vector<DoubleClickInfo>                   _doubleClickState;

        gfx::Font   _font;
        bool        _isHighlighted;
        DragAndDrop _drag;
        bool        _detectDrag;
        bool        _isDraging;
        int         _corner;

    private:
        SystemColors  _colors;
        bool            _hasBackground;
        bool            _hasFrame;
        bool            _roundedFrame;
        bool           _errorState;
        Pt::String      _errorMessage;
        Pt::String      _toolTipCached;

    private: //Action Signals
        wildcat::gui::base::Signal  _focusChanged;
        wildcat::gui::base::Signal  _beginDrag;
        wildcat::gui::base::Signal  _draging;
        wildcat::gui::base::Signal  _drop;
        wildcat::gui::base::Signal  _doubleClickSignal;
        wildcat::gui::base::Signal  _slowDoubleClickSignal;

    private: //Property signals
        wildcat::gui::base::Signal  _controlColorChanged;
        wildcat::gui::base::Signal  _controlDarkColorChanged;
        wildcat::gui::base::Signal  _controlDarkDarkColorChanged;
        wildcat::gui::base::Signal  _controlLightColorChanged;
        wildcat::gui::base::Signal  _controlLightLightColorChanged;
        wildcat::gui::base::Signal  _controlDisabledColorChanged;
        wildcat::gui::base::Signal  _highlightColorChanged;
        wildcat::gui::base::Signal  _frameColorChanged;
        wildcat::gui::base::Signal  _frameDisabledColorChanged;
        wildcat::gui::base::Signal  _infoColorChanged;
        wildcat::gui::base::Signal  _infoTextColorChanged;
        wildcat::gui::base::Signal  _textColorChanged;
        wildcat::gui::base::Signal  _textHighlightColorChanged;
        wildcat::gui::base::Signal  _textBackgroundColorChanged;
        wildcat::gui::base::Signal  _textDisabledColorChanged;
        wildcat::gui::base::Signal  _hasBackgroundChanged;
        wildcat::gui::base::Signal  _hasFrameChanged;
        wildcat::gui::base::Signal  _roundedFrameChanged;
        wildcat::gui::base::Signal  _fontChanged;
        wildcat::gui::base::Signal  _detectDragChanged;


}; 



}}}

#endif