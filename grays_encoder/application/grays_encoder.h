/*------------------------------------------------------------------------------
	()      File:   grays_encoder.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Grays encoder implementation. 
				 * Generates grays pattern. 
				 * Handles rendering
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
#include <blend2d/geometry.h>
#include <blend2d/rgba.h>
#include <blend2d/random.h>
#include "ui/properties_menu/property_panel.h"
#include "core/render_action.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//#define USE_SECTOR_RENDER 1

//------------------------------------------------------------------------------
// Forwards
//------------------------------------------------------------------------------
class BLContext;
class BLPoint;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// GraysEncoder
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class GraysEncoder
{
public:
	GraysEncoder();

	void Render( BLContext& ctx );
	void Generate();
	void Grays( int n );

	actions::RenderAction& GetRenderAction();

	void DrawArcSegment( BLContext& ctx, BLPoint& centre, float radius, float width, float startAngleDeg, float arcAngleDeg );

	void SetGrayNumber( const uint8_t n );
	void SetInnerRadius( const double rad );
	void SetOuterRadius( const double rad );
	void SetInvert( const bool val );
	void DrawInstrumentation( const bool val );

private:
	actions::RenderActionT<GraysEncoder, &GraysEncoder::Render> m_renderAction;

	//Config
	int m_nFactor = 1;
	bool m_invertTree = false;
	bool m_drawInstrumentation = false;
	float m_innerRadius = 100.0f;
	float m_outerRadius = 200.0f;

	//Data
	std::vector<unsigned int> m_bits;
};
