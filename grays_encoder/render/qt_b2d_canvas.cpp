/*------------------------------------------------------------------------------
	()      File:   qt_b2d_canvas.cpp
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Canvas for managing rendering a blend2d canvas onto a qt canvas.
				Heavily based on the blend2d sample code.
------------------------------
------------------------------
License Text - The MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "render/qt_b2d_canvas.h"
#include "utility/types_helper.h"
#include "utility/bits_helper.h"
#include "utility/globals.h"
#include "qevent.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
QPainter::CompositionMode Blend2DCompOpToQtCompositionMode( uint32_t compOp )
{
	switch ( compOp )
	{
	default:
	case BL_COMP_OP_SRC_OVER: return QPainter::CompositionMode_SourceOver;
	case BL_COMP_OP_SRC_COPY: return QPainter::CompositionMode_Source;
	case BL_COMP_OP_SRC_IN: return QPainter::CompositionMode_SourceIn;
	case BL_COMP_OP_SRC_OUT: return QPainter::CompositionMode_SourceOut;
	case BL_COMP_OP_SRC_ATOP: return QPainter::CompositionMode_SourceAtop;
	case BL_COMP_OP_DST_OVER: return QPainter::CompositionMode_DestinationOver;
	case BL_COMP_OP_DST_COPY: return QPainter::CompositionMode_Destination;
	case BL_COMP_OP_DST_IN: return QPainter::CompositionMode_DestinationIn;
	case BL_COMP_OP_DST_OUT: return QPainter::CompositionMode_DestinationOut;
	case BL_COMP_OP_DST_ATOP: return QPainter::CompositionMode_DestinationAtop;
	case BL_COMP_OP_XOR: return QPainter::CompositionMode_Xor;
	case BL_COMP_OP_CLEAR: return QPainter::CompositionMode_Clear;
	case BL_COMP_OP_PLUS: return QPainter::CompositionMode_Plus;
	case BL_COMP_OP_MULTIPLY: return QPainter::CompositionMode_Multiply;
	case BL_COMP_OP_SCREEN: return QPainter::CompositionMode_Screen;
	case BL_COMP_OP_OVERLAY: return QPainter::CompositionMode_Overlay;
	case BL_COMP_OP_DARKEN: return QPainter::CompositionMode_Darken;
	case BL_COMP_OP_LIGHTEN: return QPainter::CompositionMode_Lighten;
	case BL_COMP_OP_COLOR_DODGE: return QPainter::CompositionMode_ColorDodge;
	case BL_COMP_OP_COLOR_BURN: return QPainter::CompositionMode_ColorBurn;
	case BL_COMP_OP_HARD_LIGHT: return QPainter::CompositionMode_HardLight;
	case BL_COMP_OP_SOFT_LIGHT: return QPainter::CompositionMode_SoftLight;
	case BL_COMP_OP_DIFFERENCE: return QPainter::CompositionMode_Difference;
	case BL_COMP_OP_EXCLUSION: return QPainter::CompositionMode_Exclusion;
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// QT_B2D_Canvas
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
QT_B2D_Canvas::QT_B2D_Canvas()
{
	m_elapsedTimer.start();
	setMouseTracking(false);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::resizeEvent(QResizeEvent* event)
{
	OnResize();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	if (m_dirty)
	{
		OnRender();
	}

	painter.drawImage(QPoint(0, 0), m_qtImageBuffer);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::mousePressEvent(QMouseEvent* event)
{
	if (fn_relativeMouseMove && event != nullptr)
	{
		m_mouseCurrent = { (float)event->x(), (float)event->y() };

		//m_mouseState.previous_button_state = m_mouseState.current_button_state;
		//m_mouseState.current_button_state |= event->button();

		//fn_relativeMouseMove( *event );
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (fn_relativeMouseMove && event != nullptr)
	{
		//m_mouseState.previous_button_state = m_mouseState.current_button_state;
		//m_mouseState.current_button_state &= ~event->button();

		//fn_onMouseEvent(m_mouseState );
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::mouseMoveEvent(QMouseEvent* event)
{
	if (fn_relativeMouseMove && event != nullptr)
	{
		m_mousePrevious = m_mouseCurrent;
		m_mouseCurrent = { (float)event->x(), (float)event->y() };

		BLPoint difference = m_mouseCurrent - m_mousePrevious;
		RelativeMouseMove move = { difference.x, difference.y };
		//m_mouseState.previous_absolute_pos = m_mouseState.current_absolute_pos;
		//m_mouseState.current_absolute_pos = { event->x(), event->y() };

		fn_relativeMouseMove( move );
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::wheelEvent( QWheelEvent* event )
{
 	//QPoint numPixels = event->pixelDelta();
 	QPoint numDegrees = event->angleDelta() / 8;
	if ( !numDegrees.isNull() )
	{
		RelativeMouseWheelMove move = { numDegrees.y() };
		fn_scrollWheelMove(move);
		event->accept();
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::OnResize()
{
	int w = width();
	int h = height();

	if (m_qtImageBuffer.width() == w && m_qtImageBuffer.height() == h)
	{
		return;
	}

	m_qtImageBuffer = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
	m_blImageBuffer.createFromData(w, h, BL_FORMAT_PRGB32, m_qtImageBuffer.bits(), m_qtImageBuffer.bytesPerLine());

	OnUpdateCanvas(false);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::OnUpdateCanvas(bool force/*=false*/)
{
	m_dirty = !force;
	if (force)
	{
		OnRender();
	}

	repaint(0, 0, width(), height());
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::OnRender()
{
	if (fn_onRenderB2D)
	{
		// In Blend2D case the non-zero _rendererType specifies the number of threads.
		BLContextCreateInfo createInfo{};
		createInfo.threadCount = GetRenderThreads();

		BLContext ctx(m_blImageBuffer, createInfo);
		fn_onRenderB2D(ctx);
	}

	m_dirty = false;
	OnPostRender();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void QT_B2D_Canvas::OnPostRender()
{
	uint64_t t = m_elapsedTimer.elapsed();

	m_frameCount++;
	if (t >= 1000) 
	{
		m_fps = m_frameCount / float(t) * 1000.0f;
		m_frameCount = 0;
		m_elapsedTimer.start();
	}
}