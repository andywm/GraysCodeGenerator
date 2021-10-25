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
#include "QLabel"
#include "QAbstractButton"
#include "QPushButton"
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
	HandleCommandLine();
	InitMenus();

	m_canvas.SetRenderFunction( m_grays.GetRenderAction() );
	m_printingService.SetRenderFunction( m_grays.GetRenderAction() );
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
	if( m_actionPrint = ui.menuFile->addAction( "Print" ) )
	{
		connect( m_actionPrint, SIGNAL( triggered() ), this, SLOT( onOpenPrintDialog() ) );
	}	
	
	if( m_actionAbout = ui.menuHelp->addAction( "About" ) )
	{
		connect( m_actionAbout, SIGNAL( triggered() ), this, SLOT( onAbout() ) );
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
void WindowMain::onOpenPrintDialog()
{
	m_printingService.Run();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindowMain::onAbout()
{
	QDialog dialogBox;
	dialogBox.setWindowTitle( "About" );

	QVBoxLayout* vlayout = new QVBoxLayout;
	QLabel* title = new QLabel( "Grays Code Generator" );
	QLabel* version = new QLabel( APP_VERSION );
	QLabel* copyright = new QLabel( APP_COPYRIGHT );

	QFont font = title->font();
	font.setPointSize( 22 );
	font.setBold( true );
	title->setFont( font );

	vlayout->addWidget( title );
	vlayout->addWidget( version );
	vlayout->addWidget( copyright );
	vlayout->setSizeConstraint( QLayout::SetFixedSize );
	dialogBox.setLayout( vlayout );
	dialogBox.show();

	dialogBox.exec();
}