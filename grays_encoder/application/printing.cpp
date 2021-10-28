/*------------------------------------------------------------------------------
	()      File:   b2d_printing.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Handles printing
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
#pragma once
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QImage>
#include <blend2d.h>
#include <blend2d/image.h>
#include "application/printing.h"
#include "application/core/render_action.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PrintingService::PrintingService( QObject* parent /*= nullptr */)
	: QObject( parent )
{
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PrintingService::Run()
{
	InitPrintBuffer();

	QPrintPreviewDialog preview(&m_printer);
	//preview.setWindowTitle( "Print Document" );
	preview.setWindowFlags( Qt::Window );

	connect( &preview, SIGNAL( paintRequested( QPrinter* ) ), SLOT( PrintPreview( QPrinter* ) ) );
	
	if ( preview.exec() != QDialog::Accepted )
	{
		return;
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PrintingService::InitPrintBuffer()
{
	//get the true width/height from the printer device, using the canvas gives it you in millimetres not pixels.
	const double width = m_printer.width();
	const double height = m_printer.height();

	m_renderBuffer = QImage( width, height, QImage::Format_ARGB32_Premultiplied );
	m_b2dRenderTarget.createFromData( width, height, BL_FORMAT_PRGB32, m_renderBuffer.bits(), m_renderBuffer.bytesPerLine() );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PrintingService::PrintPreview( QPrinter* printer )
{
	const QPageLayout layout = m_printer.pageLayout();
	const QRectF pageRect = layout.paintRect( QPageLayout::Unit::Millimeter );

	printer->setOutputFileName( "grayscode.ps" );
	QPainter painter( printer );
	painter.begin( printer );

	if ( m_renderer )
	{
		BLContextCreateInfo createInfo{};
		createInfo.threadCount = 16;

		BLContext ctx( m_b2dRenderTarget, createInfo );

		const double x = ctx.targetWidth() * 0.5;
		const double y = ctx.targetHeight() * 0.5;

		const double widthInMm = pageRect.width();
		const double pixelsPerMm = x / widthInMm;

		ctx.translate( x, y );
		ctx.scale( pixelsPerMm );

		ctx.setFillStyle( BLRgba32( 0x00000000 ) );

		m_renderer->Render( ctx );

		painter.drawImage( QPoint( 0, 0 ), m_renderBuffer );
	}

	painter.end();
}


//QPageLayout layout = m_printer.pageLayout();
//QRectF pageRect = layout.paintRect();
// layout.orientation() != QPageLayout::Landscape ? pageRect.height() : pageRect.width();
// layout.orientation() != QPageLayout::Landscape ? pageRect.width() : pageRect.height();
//int dpi = m_printer.resolution();

//m_printer.pageLayout();