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
#include <wildcat/gui/gfx/JpegWriter.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Size.h>
#include <Pt/IOError.h>
#include <vector>
#include <iostream>
#include <ios>
#include <cstdlib>

extern "C" {
#include <jpeglib.h>

}

namespace wildcat {
namespace gui {
namespace gfx {

class JpegWriterImpl
{
    public:
        JpegWriterImpl();        

        JpegWriterImpl(std::ostream& os);        

        ~JpegWriterImpl();

        void attach(std::ostream& os);

        void detach();

        void reset();

        void beginWrite(const Pixmap& image);

        bool advance();
        
        void write(const Pixmap& image);
        
        void setQuality(int percent)
        {
            _quality = percent;
        }

    private:
        void fillLine(std::vector<uint8_t>& data, int line) const;

    private:
        static void onInitDestination(j_compress_ptr cinfo);

        static boolean onEmptyOutputBuffer(j_compress_ptr cinfo);

        static void onTermDestination(j_compress_ptr cinfo);
    private:
        std::ostream* _target;
        const Pixmap* _image;
        int _quality;
        struct jpeg_compress_struct _cinfo;
        struct jpeg_error_mgr _jerr;
        struct jpeg_destination_mgr _dest;
        std::vector<uint8_t> _lineData;
        bool _init;
        std::vector<uint8_t> _outBuffer;

};

enum {BLOCK_SIZE = 1024};

JpegWriterImpl::JpegWriterImpl()
: _target(0)
, _image(0)
, _quality(90)
, _init(false)
, _outBuffer() 
{

}

JpegWriterImpl::JpegWriterImpl(std::ostream& os)
: _target(&os)
, _image(0)
, _quality(90)
, _init(false)
, _outBuffer()
{

}

JpegWriterImpl::~JpegWriterImpl()
{

}

void JpegWriterImpl::attach(std::ostream& os)
{
    _target = &os;
}

void JpegWriterImpl::detach()
{
    _target  = 0;
}

void JpegWriterImpl::reset()
{
    if (!_init)
        return;

    jpeg_finish_compress(&_cinfo);

    jpeg_destroy_compress(&_cinfo);
    _init = false;
}


void JpegWriterImpl::beginWrite(const Pixmap& image)
{
    unsigned char* mem = NULL;
    unsigned long memSize = 0;


    _image = &image;
    _outBuffer.resize(BLOCK_SIZE);

    _dest.init_destination    = &JpegWriterImpl::onInitDestination;
    _dest.empty_output_buffer = &JpegWriterImpl::onEmptyOutputBuffer;
    _dest.term_destination    = &JpegWriterImpl::onTermDestination;
    _dest.next_output_byte    = 0;//(JOCTET*)&_outBuffer[0];
    _dest.free_in_buffer      = 0;
    
    _cinfo.client_data = this;
    _cinfo.err = jpeg_std_error(&_jerr);
  
  //  jpeg_mem_dest(&_cinfo, &mem, &memSize);

    jpeg_create_compress(&_cinfo);


    _cinfo.image_width = image.width();
    _cinfo.image_height = image.height();
    _cinfo.input_components = 3;
    _cinfo.in_color_space = JCS_RGB;
    _cinfo.dest = &_dest;

    jpeg_set_defaults(&_cinfo);

    jpeg_set_quality(&_cinfo, _quality, false);

    jpeg_start_compress(&_cinfo, true);
    _init = true;
}

bool JpegWriterImpl::advance()
{    

    if (_cinfo.next_scanline < _cinfo.image_height)
    {
        fillLine(_lineData, _cinfo.next_scanline);

        jpeg_write_scanlines(&_cinfo, (JSAMPARRAY)&_lineData[0], 1);
        return true;
    }


    jpeg_finish_compress(&_cinfo);

    jpeg_destroy_compress(&_cinfo);
    return false;    
}


void JpegWriterImpl::onInitDestination(j_compress_ptr cinfo)
{

    JpegWriterImpl* self = static_cast<JpegWriterImpl*>(cinfo->client_data);

    self->_outBuffer.resize(BLOCK_SIZE);

    cinfo->dest->next_output_byte = &self->_outBuffer[0];
    cinfo->dest->free_in_buffer = self->_outBuffer.size();
}

boolean JpegWriterImpl::onEmptyOutputBuffer(j_compress_ptr cinfo)
{    
    JpegWriterImpl* self = static_cast<JpegWriterImpl*>(cinfo->client_data);

    self->_target->write((const char*)&self->_outBuffer[0], self->_outBuffer.size());
    self->_target->flush();

    cinfo->dest->next_output_byte = &self->_outBuffer[0];
    cinfo->dest->free_in_buffer = self->_outBuffer.size();
    return TRUE;
}

void JpegWriterImpl::onTermDestination(j_compress_ptr cinfo)
{
    JpegWriterImpl* self = static_cast<JpegWriterImpl*>(cinfo->client_data);
    int restSize = self->_outBuffer.size() - cinfo->dest->free_in_buffer;

    self->_target->write((const char*)&self->_outBuffer[0], restSize);
}

void JpegWriterImpl::fillLine(std::vector<uint8_t>& data, int line) const
{
    data.resize(_image->width() * 3);

    int offset = 0;

    for (int i = 0; i < _image->width(); ++i)
    {
            
        ArgbColor c =_image->color(Point(i, line));

        data[offset] = c.red();
        offset++;

        data[offset] = c.green();
        offset++;

        data[offset] = c.blue();
        offset++;
    }
}


void JpegWriterImpl::write(const Pixmap& image)
{

    beginWrite(image);

    while (_cinfo.next_scanline < _cinfo.image_height) 
    {
        fillLine(_lineData, _cinfo.next_scanline);
        const uint8_t* ptr = &_lineData[0];
        jpeg_write_scanlines(&_cinfo, (JSAMPARRAY) &ptr, 1);
    }

    jpeg_finish_compress(&_cinfo);    

    jpeg_destroy_compress(&_cinfo);
    
}


JpegWriter::JpegWriter()
: ImageWriter()
, _impl(new JpegWriterImpl())
{
}


JpegWriter::JpegWriter(std::ostream& os)
: ImageWriter(os)
, _impl(new JpegWriterImpl(os))
{
}


JpegWriter::~JpegWriter()
{
    delete _impl;
}


void JpegWriter::onAttach()
{
    _impl->attach(*_stream);
}


void JpegWriter::onDetach()
{
    _impl->detach();
}


void JpegWriter::onReset()
{
    _impl->reset();
}


void JpegWriter::onBeginWrite(const Pixmap& image)
{
    _impl->beginWrite(image);
}


bool JpegWriter::onAdvance()
{
    return _impl->advance();
}

void JpegWriter::setQuality(int percent)
{
    _impl->setQuality(percent);
}


void JpegWriter::onWrite(const Pixmap& image)
{
    _impl->write(image);
}

}}}
