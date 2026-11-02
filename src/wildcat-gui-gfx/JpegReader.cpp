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
#include <wildcat/gui/gfx/JpegReader.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Size.h>
#include <Pt/IOError.h>
#include <iostream>
#include <cstdlib>
#include <cassert>

extern "C" {
#include <jpeglib.h>
}

namespace wildcat {
namespace gui {
namespace gfx {

class JpegReaderImpl
{
    public:
        JpegReaderImpl()
        : _target(0)
        , _state(OnBegin)
        , _init(false)
        , _image(0)
        { }

        JpegReaderImpl(std::istream& is, Pixmap& image)
        : _target(&is)
        , _state(OnBegin)
        , _init(false)
        , _image(&image)

        { }

        ~JpegReaderImpl()
        {
            if(_init)
            {
                jpeg_destroy_decompress(&_decomp);
            }
        }

        void attach(std::istream& is, Pixmap& image)
        {
            _target = &is;
            _image = &image;
        }

        void detach()
        {
            _target = 0;
            _image = 0;
        }

        void reset()
        {
            if(_init)
            {
                jpeg_destroy_decompress(&_decomp);
                _init = false;
            }

            _state = OnBegin;
            
            detach();
        }

        Pixmap& get()
        {
            if( ! _image )
                throw Pt::IOError("jpeg error");

            int eof = std::ios::traits_type::eof();
            
            do
            {
                
                if(!_target)
                    throw Pt::IOError("invalid jpeg format");
            } 
            while( ! advance() );

            return *_image;
        }

        Pixmap* advance()
        {
            if( ! _target || ! _image )
              return 0;

            if( ! _init )
            {
             

                _source.init_source       = &JpegReaderImpl::onInitSource;
                _source.fill_input_buffer = &JpegReaderImpl::onFillBuffer;
                _source.skip_input_data   = &JpegReaderImpl::onSkipInput;
                _source.term_source       = &JpegReaderImpl::onTermSource;
                _source.resync_to_restart = &jpeg_resync_to_restart;
                _source.next_input_byte = 0;
                _source.bytes_in_buffer = 0;

                _err.error_exit = &JpegReaderImpl::onErrorExit;
                _err.emit_message = &JpegReaderImpl::onEmitMessage;
                _err.output_message = &JpegReaderImpl::onOutputMessage;
	              
                _decomp.err = jpeg_std_error(&_err);
                _decomp.client_data = this;
                jpeg_create_decompress(&_decomp);
                _decomp.src = &_source;
                
                _init = true;
            }

            if(_state == OnBegin)
            {
                int r = jpeg_read_header(&_decomp, 1);
                if(r == JPEG_SUSPENDED)
                    return 0;

                if(r != JPEG_HEADER_OK)
                    return 0;

                _state = OnHeader;
            }

            if(_state == OnHeader)
            {
                _decomp.out_color_space = JCS_RGB;

                int r = jpeg_start_decompress(&_decomp);
                if(r == 0)
                    return 0;

                 if(_decomp.output_components != 3)
                    throw Pt::IOError("jpeg error");

                 Size imageSize(_decomp.output_width, _decomp.output_height);

                _image->resize(imageSize);

                _state = OnData;
            }
            
            if(_state == OnData)
            {
                const size_t bytesPerLine = _decomp.output_components * _decomp.output_width;

                std::vector<uint8_t> lineData(bytesPerLine);

                uint8_t* ptr = &lineData[0];

                while(_decomp.output_scanline < _decomp.output_height)
                {                                                            
                    JDIMENSION n = jpeg_read_scanlines(&_decomp, (JSAMPARRAY)&ptr, 1);

                     writeLineToImage(lineData, _decomp.output_scanline - 1);
                }

                _state = OnFinish;
            }
            
            if(_state == OnFinish)
            {
                int r = jpeg_finish_decompress(&_decomp);
                if(r == 0)
                    return 0;

                _state = OnEnd;
            }

            return _state == OnEnd ? _image : 0;
        }   

    private:

        void writeLineToImage(const std::vector<uint8_t>& lineData, size_t line)
        {
            int offset = 0;

            for (size_t i = 0; i < _image->width(); ++i)
            {
                const uint8_t r = lineData[offset];
                offset++;

                const uint8_t g = lineData[offset];
                offset++;

                const uint8_t b = lineData[offset];
                offset++;

                ArgbColor c = ArgbColor::fromRgb8(r, g, b);

                _image->setColor(c, Point(i, line));
            }
        }

        int readNextChunk()
        {
            size_t n = 0;

            while (_target && n < _bufferSize)
            {
                if (!_target->read((char*)&_buffer[n], 1))
                    break;

                ++n;
            }

            return n;
        }

    private:

        static void onErrorExit(j_common_ptr cinfo)
        {
            throw Pt::IOError("invalid jpeg format");
        }

        static void onEmitMessage(j_common_ptr, int)
        { }

        static void onOutputMessage(j_common_ptr)
        { }

        static void onInitSource(j_decompress_ptr)
        {

        }

        static boolean onFillBuffer(j_decompress_ptr decomp)
        {
            JpegReaderImpl* self = static_cast<JpegReaderImpl*>(decomp->client_data);

            if (!self->_target)
                return FALSE;


            size_t n = self->readNextChunk();

            if (n <= 0)
                return FALSE;

            decomp->src->next_input_byte = reinterpret_cast<JOCTET*>(self->_buffer);
            decomp->src->bytes_in_buffer = n;
            return TRUE;
        }

        static void onSkipInput(j_decompress_ptr decomp, long count)
        {
            if (count < 1)
                return;

            JpegReaderImpl* self = static_cast<JpegReaderImpl*>(decomp->client_data);

            for (int i = 0; i < count; ++i)
            {
                if (decomp->src->bytes_in_buffer <= 0)
                {
                    int n = self->readNextChunk();

                    if (n <= 0)
                        return;

                    decomp->src->next_input_byte = reinterpret_cast<JOCTET*>(self->_buffer);
                    decomp->src->bytes_in_buffer = n;

                }

                decomp->src->next_input_byte += 1;
                decomp->src->bytes_in_buffer -= 1;

            }
        }

        static void onTermSource(j_decompress_ptr  decomp)
        {
            JpegReaderImpl* self = static_cast<JpegReaderImpl*>(decomp->client_data);

            int n = self->readNextChunk();

            decomp->src->bytes_in_buffer = n;
            if (n)
                decomp->src->next_input_byte = reinterpret_cast<JOCTET*>(self->_buffer);
            else
                decomp->src->next_input_byte = 0;

        }

    private:
        enum State 
        {
            OnBegin = 0,
            OnHeader = 1,
            OnData = 2,
            OnFinish = 3,
            OnEnd = 4
        };
    
    private:
        std::istream*         _target;
        State                  _state;
        jpeg_error_mgr         _err;
        jpeg_source_mgr        _source;
        jpeg_decompress_struct _decomp;
        bool                   _init;
        Pixmap*                _image;
        static const int       _bufferSize = 2048;
        char                   _buffer[_bufferSize];
        size_t                 _offset;
};


JpegReader::JpegReader()
: ImageReader()
, _impl( new JpegReaderImpl() )
{
}


JpegReader::JpegReader(std::istream& is, Pixmap& image)
: ImageReader(is, image)
, _impl( new JpegReaderImpl(is, image) )
{
}


JpegReader::~JpegReader()
{
  delete _impl;
}

void JpegReader::onAttach()
{
    _impl->attach(*_stream, *_image);
}

void JpegReader::onDetach()
{
    _impl->detach();
}

void JpegReader::onReset()
{
    _impl->reset();
}

bool JpegReader::onAdvance()
{
    return _impl->advance() != 0;
}

void JpegReader::onRead()
{
    _impl->get();
}

}}}
