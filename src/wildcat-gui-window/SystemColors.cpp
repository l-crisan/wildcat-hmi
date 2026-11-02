
#include <wildcat/gui/window/SystemColors.h>

namespace wildcat {
namespace gui {
namespace window {

SystemColors::SystemColors()
{
    for (int i = 0; i < (int)ColorType::Count; ++i)
    {
        _colors[i].setPalette(0);
        _colors[i].setIndex(i);
    }

    load();
}

SystemColors::~SystemColors()
{
}

std::string SystemColors::toString(int index)
{
    switch ((ColorType)index)
    {
        case ColorType::Desktop:
            return "Desktop";

        case ColorType::Workingspace:
            return "Workingspace";

        case ColorType::Window:
            return "Window";

        case ColorType::WindowTitleText:
            return "WindowTitleText";

        case ColorType::WindowTitleBar:
            return "WindowTitleBar";

        case ColorType::WindowTitleBarActive:
            return "WindowTitleBarActive";

        case ColorType::WindowTitleBarDisabled:
            return "WindowTitleBarDisabled";

        case ColorType::WindowBorder:
            return "WindowBorder";

        case ColorType::WindowFrame:
            return "WindowFrame";

        case ColorType::WindowBorderActive:
            return "WindowBorderActive";

        case ColorType::WindowBorderDisabled:
            return "WindowBorderDisabled";

            //Control
        case ColorType::Control:
            return "Control";

        case ColorType::ControlDark:
            return "ControlDark";

        case ColorType::ControlDarkDark:
            return "ControlDarkDark";

        case ColorType::ControlLight:
            return "ControlLight";

        case ColorType::ControlLightLight:
            return "ControlLightLight";

        case ColorType::ControlDisabled:
            return "ControlDisabled";

        case ColorType::ControlHighlight:
            return "ControlHighlight";

            //Frame
        case ColorType::ControlFrame:
            return "ControlFrame";

        case ColorType::ControlFrameDisabled:
            return "ControlFrameDisabled";

            //text
        case ColorType::ControlText:
            return "ControlText";

        case ColorType::ControlTextDisabled:
            return "ControlTextDisabled";

        case ColorType::ControlTextHighlight:
            return "ControlTextHighlight";

        case ColorType::Info:
            return "Info";

        case ColorType::InfoText:
            return "InfoText";

        case ColorType::TextBackground:
            return "TextBackground";

        case ColorType::Text:
            return "Text";

        case ColorType::TextHighlight:
            return "TextHighlight";

        case ColorType::TextDisabled:
            return "TextDisabled";
        }
        return "";

}

void SystemColors::setPalleteId(uint32_t id)
{ 
    for( size_t i = 0; i < (size_t)ColorType::Count; ++i)
    {
        if (_colors[i].palette() != -1)
            _colors[i].setPalette(id);
    }   

    load();
}

void SystemColors::load()
{
    for (size_t i = 0; i < (size_t)ColorType::Count; ++i)
        _argbColors[i] = _colors[i].getColor();
}

}}}
