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
#ifndef WILDCAT_GUI_GFX_RASTERIZER_H
#define WILDCAT_GUI_GFX_RASTERIZER_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Brush.h>
#include <wildcat/gui/gfx/Algorithm.h>
#include <wildcat/gui/gfx/FontInfo.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <Pt/String.h>
#include <Pt/System/Path.h>

namespace wildcat {
namespace gui {
namespace gfx {

class LineSlope;
class LineEdge;
class LineFace;
class DrawText;
class Pixmap;
class ActiveEdgeTable;

class Rasterizer
{
protected:
    class ClipRect : public Rect
    {
    public:
        ClipRect(const Rect& r = Rect())
            : Rect(r)
            , _bottom(0)
            , _right(0)
        {
            _right = x() + width();
            _bottom = y() + height();
        }

        ClipRect& operator=(const ClipRect& r)
        {
            Rect::operator=(r);
            _right = x() + width();
            _bottom = y() + height();
            return *this;
        }

        int bottom() const
        {
            return _bottom;
        }

        int right() const
        {
            return _right;
        }

    private:
        int _bottom;
        int _right;
    };

public:
    Rasterizer(Pixmap& image);

    ~Rasterizer();

    const Pixmap& image() const
    {
        return _image;
    }

    Pixmap& image()
    {
        return _image;
    }

    void reset(const Pixmap& image)
    {
        _image.resize(image.size());
        copy(image.begin(), image.end(), _image.begin());
    }

    void reset(const Size& size, std::size_t stride)
    {
        _image.resize(size, stride);
    }

    void setClip(const Rect& clip);

    void resetClip();

    void setCompositionMode(const CompositionMode& mode)
    {
        _compositionMode = mode;
    }

    void setPen(const Pen& pen);

    void setBrush(const Brush& brush);

    void setFont(const Font& font);

    FontMetrics fontMetrics(const Pt::String& text) const;

    void drawLine(const Point& from, const Point& to);

    void drawText(const Point& to, const Pt::String& Text);

    void drawText(const Point& to, const Pt::String& Text, const Transform& trans);

    void drawRect(const Rect& rectangle);

    void fillRect(const Rect& rectangle);

    void drawEllipse(const Point& topLeft, const Size& size);

    void fillEllipse(const Point& topLeft, const Size& size);

    void drawPolyline(const Point* points, size_t pointCount);

    void fillPolygon(const Point* points, size_t pointCount);

    void drawImage(const Point& to, const Pixmap& image);

    void drawImage(const Point& to, const Pixmap& image, const Rect& imgRect);

    //void drawPath(const Path& path, float smoothness);

    //void fillPath(const Path& path, float smoothness);

    void drawArc(const Point& topLeft, const Size& size, float degBegin, float degEnd);

    void fillChord(const Point& topLeft, const Size& size, float degBegin, float degEnd);

    void fillPie(const Point& topLeft, const Size& size, float degBegin, float degEnd);

public:
    static void setFontDir(const Pt::System::Path& path);

    static const gfx::Font& defaultFont();

    static void setDefaultFont(const gfx::Font& name);

    static const std::vector<FontInfo>& fonts();

    static FontMetrics fontMetrics(const Font& font, const Pt::String& text);

private:
    void image(const Point& to, const Pixmap& image);

    void image(const Point& toIn,
        const Pixmap& image,
        const Rect& imageRect);

    void fillRect(const Rect& r, const ClipRect& currentClip);

    void stroke(const Point* points, size_t pointCount, const ClipRect& currentClip);

    void stroke(const Point& pixel, const ClipRect& currentClip);

    void fill(const Point* points, size_t pointCount, const ClipRect& currentClip);

    void strokeText(const Point& to, const Pt::String& text, const ClipRect& currentClip);

    void strokeText(const Point& to, const Pt::String& text, const Transform& trans, const ClipRect& currentClip);

    void strokeEllipse(const Point& topLeft, const Size& size, const ClipRect& currentClip);

    void fillEllipse(const Point& topLeft, const Size& size, const ClipRect& currentClip);

    //Output algo.
protected:
    void outputSpan(const Point& topLeft, int x, int y, int width);
    void fill(const Point& origin, const Point& pos, int length);
    void fillSolid(const Point& pos, int length);
    void fillVerticalGradient(const Point& origin, const Point& pos, int length);
    void fillHorizontalGradient(const Point& origin, const Point& pos, int length);
    void fillTexture(const Point& origin, const Point& pos, int length);
    //void createGradientTexture(Picmap& img, int width, int height,
    //                           ArgbColor gradientStart,
    //                           ArgbColor gradientStop, 
    //Brush::GradientDirection style);
    void clipSpan(int& x, int& y, int& length, const ClipRect& currentClip);

    ClipRect updateClip() const
    {
        const Rect imageRect(_image.size());
        return _clip.intersect(imageRect);
    }

    void outputEdges(const ActiveEdgeTable& edges, const Point& origin, int scalLine);

    //Thin polyline algo.
protected:
    enum { xAxis, yAxis };
    void drawThinSolidPolyline(const Point* points, int pointCount, const ClipRect& currentClip);
    void drawThinDashPolyline(const Point* points, int pointCount,
        int dashOn, int dashOff, const ClipRect& currentClip);
    void stepDash(int dist, int* pDashNum, int* pDashIndex, const int* pDash, int numInDashList, int* pDashOffset);

    void bresenhamDasheLineSegment(int* pdashNum, int* pdashIndex, const  int* pDash, int numInDashList, int* pdashOffset,
        bool isDoubleDash, int signdx, int signdy, int axis, int x1, int y1,
        int e, int e1, int e2, int len, const ClipRect& currentClip);

    void bresenhamLineSegment(int signdx, int signdy, int axis, int x1, int y1,
        int e, int e1, int e2, int len, const ClipRect& currentClip);

    //Wide polyline base algo.
protected:
    int polyBuildPoly(const Point* vertices, const LineSlope* slopes, int count, int xi, int yi, LineEdge* left, LineEdge* right, int* pnleft, int* pnright, int* h);
    int buildLineEdge(double x0, double y0, double k, int dx, int dy, int xi, int yi, bool left, LineEdge* edge);
    void fillSpans(int x, int y, int w, int h, const ClipRect& currentClip);
    void fillLine(int y, int overall_height, LineEdge* left, LineEdge* right, int left_count, int right_count, const ClipRect& currentClip);
    void lineArc(LineFace* leftFace, LineFace* rightFace, double xorg, double yorg, bool isInt, const ClipRect& currentClip);
    void roundJoinClip(LineFace* pLeft, LineFace* pRight, LineEdge* edge1, LineEdge* edge2, int* y1, int* y2, bool* left1, bool* left2);
    int roundCapClip(const LineFace* face, bool isInt, LineEdge* edge, bool* leftEdge);
    int lineArcI(int xorg, int yorg, std::vector<Point>& points, std::vector<int>& widths);
    int lineArcD(double xorg, double yorg, std::vector<Point>& points, std::vector<int>& widths, LineEdge* edge1, int edgey1, bool edgeleft1, LineEdge* edge2, int edgey2, bool edgeleft2);
    int roundJoinFace(const LineFace* face, LineEdge* edge, bool* leftEdge);
    void lineJoin(LineFace* pLeft, LineFace* pRight, const ClipRect& currentClip);
    void lineProjectingCap(const LineFace* face, bool isLeft, bool isInt, const ClipRect& currentClip);
    void clipStepEdge(int ybase, int& xcl, int& xcr, int& edgey, LineEdge* edge, bool edgeleft);

    //Wide solid polyline algo.
protected:
    void drawWideSolidPolyline(const Point* points, int pointCount, const ClipRect& currentClip);
    void drawSegment(Point from, Point to, bool projectLeft, bool projectRight, LineFace* leftFace, LineFace* rightFace, const ClipRect& currentClip);

    //Wide dashed polyline algo.
protected:
    enum { V_TOP = 0, V_RIGHT = 1, V_BOTTOM = 2, V_LEFT = 3 };

    void drawWideDashPolyline(const Point* points, int pointCount,
        int dashOn, int dashOff, const ClipRect& currentClip);

    void dashSegment(int* pDashNum, int* pDashIndex, int* pDashOffset, int x1, int y1, int x2, int y2,
        bool projectLeft, bool projectRight, LineFace* leftFace, LineFace* rightFace, int* dash, const ClipRect& currentClip);

private:
    void stroke(int x, int y, const ClipRect& currentClip);

    void stroke(int xpos, int ypos, int length, const ClipRect& currentClip);

    void updateGradientBrush(int width, int height);

private:
    Pixmap& _image;
    DrawText* _text;
    Rect            _clip;
    Font            _font;

    Brush           _brush;
    Pixmap           _brushBuffer;
    const Pixmap* _brushImage;
    gfx::Point      _brushPixel;
    bool            _isGradient;

    Pen             _pen;
    Pixmap          _penBuffer;
    gfx::Point      _penPixel;

    CompositionMode _compositionMode;
};

}}}

#endif
