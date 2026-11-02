
#ifndef WILDCAT_GUI_WINDOW_KEYHANDLER_H    
#define WILDCAT_GUI_WINDOW_KEYHANDLER_H

#include <inttypes.h>

namespace wildcat {
namespace gui {
namespace window {
namespace KeyHandler{

wchar_t keySymToUtf(int sym);

uint32_t keySymToCode(int keySym);

}}}}

#endif
