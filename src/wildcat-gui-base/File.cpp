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
#include <wildcat/gui/base/File.h>
#include <Pt/System/FileInfo.h>
#include <fstream>
#include <vector>
#include <stdexcept>

namespace wildcat {
namespace gui {
namespace base {

void File::copy(const std::string& srcFile, const std::string& destFile)
{
    std::ifstream source(srcFile.c_str(), std::ios::binary);
    std::ofstream dest(destFile.c_str(), std::ios::binary);

    dest << source.rdbuf();

    source.close();
    dest.close();
}

std::string File::readText(const std::string& file)
{
    Pt::System::Path path(file.c_str());

    if(!Pt::System::FileInfo::exists(path))
        throw std::runtime_error("File not found");

    const size_t size = Pt::System::FileInfo::size(path);
    
    std::vector<char> buffer(size+1);

    std::ifstream f(file);
    f.read(&buffer[0], size);

    buffer[size] = 0;
    return &buffer[0];
}

std::string File::readText( std::istream& s)
{
    s.seekg(0, std::ios::end);
    const size_t size = s.tellg();
    s.seekg(0, std::ios::beg);

    std::vector<char> buffer(size + 1);

    s.read(&buffer[0], size);

    buffer[size] = 0;
    return &buffer[0];
}

void File::writeText(const std::string& file, const std::string& text)
{
    char term = 0;

    std::ofstream f(file);
    f.write(&text[0], text.size());
    f.write(&term, 1);
    f.flush();
    f.close();
}

void File::writeText(std::ostream& os, const std::string& text)
{
    char term = 0;

    os.write(&text[0], text.size());
    os.write(&term, 1);
    os.flush();
}

void File::skippWhiteSpace(std::istream& is)
{
    while (std::isspace(is.peek()))
        is.get();
}

}}}
