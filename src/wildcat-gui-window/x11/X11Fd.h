#ifndef WILDCAT_GUI_WINDOW_X11FD_H 
#define WILDCAT_GUI_WINDOW_X11FD_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

// X11 defines conflict with enum values in Pt/Char.h
#undef Above
#undef Below

#include "Selector.h"

#include <Pt/System/Selectable.h>
#include <Pt/System/MainLoop.h>
#include <Pt/System/IONotifier.h>
#include <Pt/Singleton.h>
#include <Pt/Event.h>

#include <map>
#include <unistd.h>

namespace wildcat {
namespace gui {
namespace window {

class X11Fd : public Pt::System::Selectable
{
    public:
        X11Fd(Display* display);

        virtual ~X11Fd();

        void begin();

        void close();

        void processEvents();

        Pt::Signal<XEvent&>& eventReady()
        { 
            return _eventReady; 
        }
        
    protected:
        virtual bool onRun();

        virtual void onCancel();
        
        void onAttach(Pt::System::EventLoop& loop);

        void onDetach(Pt::System::EventLoop& loop);    

    private:
        Display*               _display;
        XEvent                 _xev;
        Pt::System::IOHandle   _ioh;
        Pt::System::EventLoop* _loop;
        Pt::Signal<XEvent&>    _eventReady; 
};

}}}

#endif
