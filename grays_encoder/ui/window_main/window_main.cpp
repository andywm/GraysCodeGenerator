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
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include "ui/window_main/window_main.h"
#include "utility/globals.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// WindowMain
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
WindowMain::WindowMain( QWidget* parent/*=nullptr*/)
	: QMainWindow( parent )
{
	QString windowTitle = QString::fromUtf8( "Andy's Grays Encoder" );
	setWindowTitle( windowTitle );

	HandleCommandLine();

	//init program loop
	//m_timer.setInterval( 60 / 1000.0f );
	//connect( &m_timer, SIGNAL( timeout() ), this, SLOT( onTimer() ) );

	InitMenus();


	//m_grays.Initialise()


	m_canvas.SetRenderFunction( m_grays.GetRenderAction() );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::InitMenus()
{
	ui.setupUi( this );

	QVBoxLayout* vBox = new QVBoxLayout();
	vBox->setContentsMargins( 0, 0, 0, 0 );
	vBox->setSpacing( 0 );

	QGridLayout* grid = new QGridLayout();
	grid->setContentsMargins( 5, 5, 5, 5 );
	grid->setSpacing( 5 );

	vBox->addLayout( grid );
	vBox->addWidget( &m_canvas );
	ui.Viewport->setLayout( vBox );

	InitMenuBar();
	InitGraysConfig();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::InitMenuBar()
{
	if ( m_actionPrint = ui.menuFile->addAction( "Print" ) )
	{
		connect( m_actionPrint, SIGNAL( QAction::triggered() ), this, SLOT( onOpenPrintDialog() ) );
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::InitGraysConfig()
{
	//init
	m_propertyPanel.Initialise( *ui.configurator, parent() );

	//Gray Number
	m_propertyPanel.AddProperty( "root.gray", "Gray N", 1, 1, 31 )
		.Connect<WindowMain, &WindowMain::OnGrayChanged>( *this );

	//Endianness.
	m_propertyPanel.AddProperty( "root.instrum", "Draw Instrumentation", true )
		.Connect<WindowMain, &WindowMain::OnInstrumentationChanged>( *this );

	//Endianness.
	m_propertyPanel.AddProperty( "root.endian", "Inverted", true )
		.Connect<WindowMain, &WindowMain::OnEndianChanged>( *this );

	//Radius
	m_propertyPanel.AddProperty( "root.innerrad", "Inner Radius", 100.0f, 0.0f, 200.0f )
		.Connect<WindowMain, &WindowMain::OnInnerRadiusChanged>( *this );

	m_propertyPanel.AddProperty( "root.outerrad", "Outer Radius", 150.0f, 0.0f, 300.0f )
		.Connect<WindowMain, &WindowMain::OnOuterRadiusChanged>( *this );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::OnGrayChanged( const QVariant& qvr )
{
	m_grays.SetGrayNumber( qvr.toInt() );
	m_canvas.Invalidation();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::OnInnerRadiusChanged( const QVariant& qvr )
{
	m_grays.SetInnerRadius( qvr.toDouble() );
	m_canvas.Invalidation();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::OnOuterRadiusChanged( const QVariant& qvr )
{
	m_grays.SetOuterRadius( qvr.toDouble() );
	m_canvas.Invalidation();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::OnEndianChanged( const QVariant& qvr )
{
	m_grays.SetInvert( qvr.toBool() );
	m_canvas.Invalidation();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::OnInstrumentationChanged( const QVariant& qvr )
{
	m_grays.DrawInstrumentation( qvr.toBool() );
	m_canvas.Invalidation();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::HandleCommandLine()
{
	using iter_str = std::vector<std::string>::iterator;

	//defaults
	m_canvas.SetNumberOfRenderThreads( 16 );

	for( iter_str strIter = g_commandLineArgs.args.begin(); strIter != g_commandLineArgs.args.end(); ++strIter )
	{	
		if( strIter->_Equal("set-render-threads") && ++strIter != g_commandLineArgs.args.end() )
		{
			std::string& candiateNumber = *strIter;
			if( std::all_of( candiateNumber.begin(), candiateNumber.end(), ::isdigit ) )
			{
				m_canvas.SetNumberOfRenderThreads( atoi( candiateNumber.c_str() ) );
			}
		}
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Q_SLOT void WindowMain::onTimer()
{
	//const BLImage& buffer = m_canvas.GetImageBuffer();
	//m_graysService.UpdateArea( buffer.width(), buffer.height() );
	//m_graysService.Tick( 60/1000.0f );
	//m_canvas.OnUpdateCanvas( true );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::onOpenPrintDialog()
{
	QPrinter printer;
	int dpi = printer.resolution();
	
	printer.pageLayout();

	QPrintDialog dialog( &printer, this );
	dialog.setWindowTitle( tr( "Print Document" ) );
	
// 	if ( editor->textCursor().hasSelection() )
// 	{
// 		dialog.addEnabledOption( QAbstractPrintDialog:: );
// 	}

	if ( dialog.exec() != QDialog::Accepted )
	{
		return;
	}
}