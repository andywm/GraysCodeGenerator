/*------------------------------------------------------------------------------
	()      File:   globals.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
			  Generates an n Gray's encoding pattern
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
#include <vector>
#include <string>
#include "utility/version.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
struct GlobalData
{
	std::vector<std::string> args;
};

extern GlobalData g_commandLineArgs;

namespace maths
{
	extern const double Tau;
	extern const double Pi;
	extern const double Pi2;
	extern const double PiBy2;
	extern const double PiBy4;
	extern const double PiBy8;
}

#define DegToRad(x) (x*(maths::Pi/180.0f))
#define RadToDeg(x) (x*(180.0f/maths::Pi))
