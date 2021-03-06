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
#include <unordered_map>
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forwards
//------------------------------------------------------------------------------
class BLContext;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// RenderAction
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace actions
{
	class RenderAction
	{
	public:
		virtual void Render( BLContext& context ) = 0;

		std::unordered_map<const char*, bool> options;
		std::unordered_map<const char*, double> parameters;
	};

	template<class Base, void (Base::*renderFn)(BLContext&) >
	class RenderActionT final : public RenderAction
	{
	public:
		RenderActionT( Base& self ) : m_self( self ){}

		virtual void Render( BLContext& context ) override
		{	
			(m_self.*renderFn)( context );
		}

	private:
		Base& m_self;
	};
}