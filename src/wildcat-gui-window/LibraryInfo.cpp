/*
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
#include <wildcat/gui/window/ElementInfo.h>
#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/resource/icon/WindowIcon.xpm>
#include <wildcat/gui/resource/icon/MdiIcon.xpm>
#include <wildcat/gui/resource/icon/DockinLayoutIcon.xpm>
#include <wildcat/gui/resource/icon/FlowLayoutIcon.xpm>
#include <wildcat/gui/resource/icon/ScrollLayoutIcon.xpm>   
#include <wildcat/gui/resource/icon/StackLayoutIcon.xpm> 
#include <wildcat/gui/gfx/Pixmap.h>
#include <vector>

extern "C"
{
    WILDCAT_GUI_WINDOW_API const std::vector<wildcat::gui::window::ElementInfo>& wildcat_gui_window_library_info()
    {
        static std::vector<wildcat::gui::window::ElementInfo> info;

        if( !info.empty())
            return info;
            
        //System, like window
        //info.push_back(wildcat::gui::window::ElementInfo("Window", "wildcat.gui.window.Window", "System"));
        //info.push_back(wildcat::gui::window::ElementInfo("Popup", "wildcat.gui.window.Popup", "System"));

        //Container
        wildcat::gui::gfx::Pixmap icon;   
        icon.loadXpm(MdiIcon);
        info.push_back(wildcat::gui::window::ElementInfo("MdiArea", "wildcat.gui.window.MdiArea", "Container", "", icon));

        //Layout
        icon.loadXpm(DockinLayoutIcon);
        info.push_back(wildcat::gui::window::ElementInfo("DockingLayout", "wildcat.gui.window.DockingLayout", "Layout", "", icon ));

        icon.loadXpm(FlowLayoutIcon);
        info.push_back(wildcat::gui::window::ElementInfo("FlowLayout", "wildcat.gui.window.FlowLayout", "Layout", "", icon));

        icon.loadXpm(ScrollLayoutIcon);
        info.push_back(wildcat::gui::window::ElementInfo("ScrollLayout", "wildcat.gui.window.ScrollLayout", "Layout", "", icon));

        icon.loadXpm(StackLayoutIcon);
        info.push_back(wildcat::gui::window::ElementInfo("StackLayout", "wildcat.gui.window.StackLayout", "Layout", "", icon));
        return info;
    }
}


namespace wildcat {
namespace gui {
namespace window {


WILDCAT_GUI_WINDOW_API const std::vector<wildcat::gui::window::ElementInfo>& getLibraryInfo()
{
    return wildcat_gui_window_library_info();
}

}}}