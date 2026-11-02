
#include "X11Fd.h"
#include <stdexcept>

namespace wildcat {
namespace gui {
namespace window {

void onConnectionChanged(Display* display, XPointer client_data, 
                         int fd, Bool opening, XPointer *watch_data)
{
  std::clog << "### CONECTION ###   " << fd << std::endl;
}


X11Fd::X11Fd(Display* display)
: _display(display)
, _ioh(*this)
, _loop(0)
{
     XAddConnectionWatch(_display, &onConnectionChanged, 0);

    _ioh.fd = XConnectionNumber(_display);
    if( _ioh.fd < 0 )
        throw Pt::System::AccessFailed("invalid X11 display");
}


X11Fd::~X11Fd()
{
    try
    {
        this->close();
    }
    catch(...)
    {}
}


void X11Fd::begin()
{      
    //std::clog << "X11Fd BEGIN" << std::endl;

    if( ! _loop )
        throw std::logic_error("I/O device not active");

    Pt::System::Selector& selector = _loop->selector();
    selector.beginRead(&_ioh);

    //std::clog << "BEGIN X11FD" << std::endl;
}


void X11Fd::close()
{ 
    if(_ioh.fd != -1)
    {
        ::close(_ioh.fd); 
        _ioh.fd = -1;
    }
}


void X11Fd::processEvents()
{ 
    while( XPending(_display) > 0 ) 
    {
        XNextEvent(_display, &_xev);

        // if(_xev.xany.type == ConfigureNotify)
        //     std::clog << "ConfigureNotify" << std::endl;
        // else
        //     std::clog << "EVENT: " <<  _xev.xany.type << std::endl;

        _eventReady.send(_xev);
    }
}


bool X11Fd::onRun()
{
    //std::clog << "X11Fd::onRun BEGIN" << std::endl;

    Pt::System::Selector& selector = _loop->selector();
    bool isAvail = selector.isReadable(&_ioh);
    if( ! isAvail ) 
        return isAvail;

    selector.endRead(&_ioh);
    selector.beginRead(&_ioh);

    processEvents();

    //std::clog << "X11Fd::onRun END" << std::endl;  

    return isAvail;
}


void X11Fd::onCancel()
{ 
    throw std::logic_error("not implemented"); 
}


void X11Fd::onAttach(Pt::System::EventLoop& loop)
{ 
    _loop = &loop;
}


void X11Fd::onDetach(Pt::System::EventLoop& loop)
{ 
    _loop = 0; 
}

}}}
