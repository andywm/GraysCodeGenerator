/*------------------------------------------------------------------------------
	()      File:   window_main.cpp
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
			  Main window for gray's encoder application.
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
#include "ui/window_main/window_main.h"
#include "utility/globals.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// WindowMain
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
uint32_t WindowMain::m_renderThreads = 1;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
WindowMain::WindowMain( QWidget* parent/*=nullptr*/)
	: QMainWindow( parent )
{
    ui.setupUi(this);

	QVBoxLayout* vBox = new QVBoxLayout();
	vBox->setContentsMargins(0, 0, 0, 0);
	vBox->setSpacing(0);

	QGridLayout* grid = new QGridLayout();
	grid->setContentsMargins(5, 5, 5, 5);
	grid->setSpacing(5);

	vBox->addLayout(grid);
	vBox->addWidget( &m_canvas );
	ui.Viewport->setLayout( vBox );

	//loop
	m_timer.setInterval( 60 / 1000.0f );
	connect( &m_timer, SIGNAL( timeout() ), this, SLOT( onTimer() ) );

	//demo
	m_demo.Initialise();
	m_demo.InitProperties( *ui.configurator, parent );

	//bind graphics to demo draw function.
	m_canvas.SetRenderFunction<DemoProgram, &DemoProgram::Render>(m_demo);
	m_canvas.SetNumberOfRenderThreads( m_renderThreads );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::HandleCommandLine()
{
	bool checkNextForRT = false;
	for( const std::string& str : g_commandLineArgs.args )
	{	
		if( checkNextForRT )
		{
			if ( std::all_of( str.begin(), str.end(), ::isdigit ) )
			{
				m_renderThreads = atoi( str.c_str() );
			}
		}

		checkNextForRT = false;
		if( str._Equal("set-render-threads") )
		{
			checkNextForRT = true;
		}
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Q_SLOT void WindowMain::onTimer()
{
	const BLImage& buffer = m_canvas.GetImageBuffer();
	m_demo.UpdateArea( buffer.width(), buffer.height() );
	m_demo.Tick( 60/1000.0f );
	m_canvas.OnUpdateCanvas( true );
	_updateTitle();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::_updateTitle()
{
	// 	char buf[256];
	// 	snprintf(buf, 256, "Rectangles Sample [%dx%d] [Size=%d Count=%zu] [%.1f FPS]",
	// 		m_canvas.width(),
	// 		m_canvas.height(),
	// 		int(m_rectSize),
	// 		m_coords.size(),
	// 		m_canvas.GetFPS());

	QString title = QString::fromUtf8( "Demo Program" /*buf*/ );
	if ( title != windowTitle() )
	{
		setWindowTitle( title );
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::showEvent(QShowEvent* event) 
{
	m_timer.start(); 
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::hideEvent(QHideEvent* event)
{
	m_timer.stop();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::keyPressEvent(QKeyEvent* event)
{
}
