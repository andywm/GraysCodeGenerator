/*------------------------------------------------------------------------------
    ()      File:   main.cpp
    /\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
                entry point
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
#include <QtWidgets/QApplication>
#include "ui/window_main/window_main.h"
#include "utility/globals.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#if !defined QBLCANVAS_H
#define QBLCANVAS_H
#endif //!defined QBLCANVAS_H

//------------------------------------------------------------------------------
//Watch Window functions, as can't for the life of me get Qt's natvis to work as expected,
//------------------------------------------------------------------------------
int InspectInt( const QVariant& qvar )
{
    return qvar.toInt();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool InspectBool( const QVariant& qvar )
{
	return qvar.toBool();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
float InspectFloat( const QVariant& qvar )
{
	return qvar.toFloat();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const char* InspectString( const QVariant& qvar )
{
    static QByteArray mem;
    mem = qvar.toString().toLatin1();
	return mem.data();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    //cache arguments
    for( int i = 0; i < argc; i++ )
    {
        g_commandLineArgs.args.push_back( argv[i] );
    }

    QApplication a(argc, argv);
    WindowMain window;
    window.setMinimumSize(QSize(400, 320));
    window.resize(QSize(580, 520));
    window.show();

    return a.exec();
}
