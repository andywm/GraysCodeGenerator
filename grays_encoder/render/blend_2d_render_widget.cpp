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
#include <QPainter>
#include <qevent.h>
#include "application/core/render_action.h"
#include "render/blend_2d_render_widget.h"
//#include "utility/globals.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Blend2DRenderWidget
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Blend2DRenderWidget::Blend2DRenderWidget()
{
	setMouseTracking(false);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Blend2DRenderWidget::Invalidation()
{
	UpdateRenderBuffer();
	repaint( 0, 0, width(), height() );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Blend2DRenderWidget::resizeEvent(QResizeEvent* event)
{
	int w = width();
	int h = height();

	if ( m_renderBuffer.width() == w && m_renderBuffer.height() == h )
	{
		return;
	}

	m_renderBuffer = QImage( w, h, QImage::Format_ARGB32_Premultiplied );
	m_b2dRenderTarget.createFromData( w, h, BL_FORMAT_PRGB32, m_renderBuffer.bits(), m_renderBuffer.bytesPerLine() );

	UpdateRenderBuffer();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Blend2DRenderWidget::paintEvent( QPaintEvent* event )
{
	QPainter painter( this );
	painter.drawImage( QPoint( 0, 0 ), m_renderBuffer );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Blend2DRenderWidget::mousePressEvent(QMouseEvent* event)
{
	if( event != nullptr)
	{
		m_mouseCurrent = { static_cast<double>(event->x()), static_cast<double>(event->y()) };
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Blend2DRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
	if ( event != nullptr)
	{
		m_mousePrevious = m_mouseCurrent;
		m_mouseCurrent = { static_cast<double>(event->x()), static_cast<double>(event->y()) };

		const BLPoint difference = m_mouseCurrent - m_mousePrevious;
		m_userPosition += { difference.x, difference.y };

		Invalidation();
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Blend2DRenderWidget::wheelEvent( QWheelEvent* event )
{
 	QPoint numDegrees = event->angleDelta() / 8;
	if( !numDegrees.isNull() )
	{
		const double move = numDegrees.y();

		if( move > 0 )
		{
			m_zoomLevel = m_zoomLevel * 1.25f;
		}
		else
		{
			m_zoomLevel = m_zoomLevel * 0.75f;
		}

		event->accept();

		Invalidation();
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Blend2DRenderWidget::UpdateRenderBuffer()
{
	if ( m_renderer )
	{
		BLContextCreateInfo createInfo{};
		createInfo.threadCount = GetRenderThreads();

		BLContext ctx( m_b2dRenderTarget, createInfo );

		const double x = (ctx.targetWidth() * 0.5) + m_userPosition.x;
		const double y = (ctx.targetHeight() * 0.5) + m_userPosition.y;

		ctx.translate( x, y );
		ctx.scale( m_zoomLevel );

		ctx.setFillStyle( BLRgba32( 0xFF000000 ) );

		m_renderer->Render( ctx );
	}
}
