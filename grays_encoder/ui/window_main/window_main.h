/*------------------------------------------------------------------------------
	()      File:   window_main.h
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
#pragma once
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QtWidgets/QMainWindow>
#include "ui_window_main.h"
#include "render/blend_2d_render_widget.h"
#include "application/grays_encoder.h"
#include "application/printing.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// WindowMain
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class WindowMain : public QMainWindow
{
    Q_OBJECT

public:
    WindowMain(QWidget *parent = nullptr);

private slots:
	void onOpenPrintDialog();
	void onAbout();

private:
	void HandleCommandLine();
	void InitMenus();
	void InitMenuBar();
	void InitGraysConfig();

	//Configuration Changed
	void OnGrayChanged( const QVariant& qvr );
	void OnInnerRadiusChanged( const QVariant& qvr );
	void OnOuterRadiusChanged( const QVariant& qvr );
	void OnEndianChanged( const QVariant& qvr );
	void OnInstrumentationChanged( const QVariant& qvr );

private:
	//menu
    Ui::window_main_Class ui;
	QAction* m_actionPrint = nullptr;
	QAction* m_actionAbout = nullptr;

	//application
	GraysEncoder m_grays;
	PropertyPanel m_propertyPanel;
	Blend2DRenderWidget m_canvas;
	PrintingService m_printingService;
};
