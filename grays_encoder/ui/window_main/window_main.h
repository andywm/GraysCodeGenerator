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
#include "render/qt_b2d_canvas.h"
#include "application/grays_encoder.h"
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

	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

	void HandleCommandLine();

	Q_SLOT void onTimer();
	void _updateTitle();

private:
	static uint32_t m_renderThreads;
    Ui::window_main_Class ui;

	QTimer m_timer;
	QT_B2D_Canvas m_canvas;
	GraysEncoder m_demo;
};
