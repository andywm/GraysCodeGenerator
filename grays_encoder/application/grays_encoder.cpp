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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <blend2d/context.h>
#include <QVariant>
#include <qevent.h>
#include "application/grays_encoder.h"
#include "utility/globals.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// GraysEncoder
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
GraysEncoder::GraysEncoder()
	: m_renderAction(*this)
{

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::DrawArcSegment( 
	BLContext& ctx,
	BLPoint& centre,
	float radius,
	float width, 
	float startAngleDeg, 
	float arcAngleDeg 
)
{
	const double angleBEGIN = DegToRad( startAngleDeg );
	const double arcLength = DegToRad( arcAngleDeg );
	const double angleEND = angleBEGIN + arcLength;

	const BLPoint innerRadius(radius, radius);
	const BLPoint outerRadius(radius + width, radius + width);

	//anti-clockwise winding
	const BLPoint p1 = {
		centre.x + (cos( angleBEGIN ) * innerRadius.x),
		centre.y + (sin( angleBEGIN ) * innerRadius.x)};

	const BLPoint p2 = {
		centre.x + (cos( angleEND ) * innerRadius.x),
		centre.y + (sin( angleEND ) * innerRadius.x)};

	const BLPoint p3 = {
		centre.x + (cos( angleEND ) * outerRadius.x),
		centre.y + (sin( angleEND ) * outerRadius.x) };

	const BLPoint p4 = {
		centre.x + (cos( angleBEGIN ) * outerRadius.x),
		centre.y + (sin( angleBEGIN ) * outerRadius.x) };

	BLPath path;

	path.moveTo( p1 );
	path.arcTo( centre, innerRadius, angleBEGIN, arcLength );
	path.lineTo( p3 );
	path.arcTo( centre, outerRadius, angleEND, -arcLength );
	path.lineTo( p1 );

	ctx.fillPath( path );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::Render( BLContext& ctx )
{
	if ( m_bits.empty() )
	{
		Generate();
	}

	BLPoint origin = { 0, 0 };

	//Render Options
	ctx.setRenderingQuality( BL_RENDERING_QUALITY_ANTIALIAS );
	ctx.setCompOp( BL_COMP_OP_SRC_OVER );
	
	//Clear Black
	ctx.setFillStyle( BLRgba32( 0xFF000000 ) );
	ctx.fillAll();

	//draw in white.
	ctx.setFillStyle( BLRgba32( 0xFFFFFFFF ) );
	
	const int segmentCount = pow( 2, m_nFactor );
	const double stepAngle = 360.0f / segmentCount;
	const double radDifference = m_outerRadius - m_innerRadius;
	const double trackWidth = radDifference / m_nFactor;

	//iterates over the circle by each step angle. The inner loop handles different
	//orbitals representing bits.
	double beginAngle = 0;
	for ( unsigned int segmentId = 0; segmentId < segmentCount; ++segmentId )
	{
		const double endAngle = beginAngle + stepAngle;
		const uint32_t grayCode = m_bits[segmentId];

		for ( int bit = 0; bit < m_nFactor; ++bit )
		{
			double localRadius = m_invertTree
				? m_innerRadius + (trackWidth * bit)
				: m_outerRadius - (trackWidth * (bit+1))
				;

			if( (grayCode & (0x1 << bit)) == 0 )
			{
				continue;
			}
		
			DrawArcSegment( ctx, origin, localRadius, trackWidth, beginAngle, fabs( endAngle - beginAngle ) );
		}
		beginAngle = endAngle;
	}

	//------------------------------------------------------
	//Instrumentation Passes
	//------------------------------------------------------
	if( !m_drawInstrumentation )
	{
		return;
	}

	// Configure Strokes.
	ctx.setStrokeStyle( BLRgba32( 0xFFFF0000 ) );
	ctx.setStrokeWidth( 1 );

	//Render concentric circles. (render helper lines around each track.)
	for ( int track = 0; track <= m_nFactor; ++track )
	{
		double radDifference = m_outerRadius - m_innerRadius;
		double trackWidth = radDifference / m_nFactor;
		double localRadius = m_innerRadius + (trackWidth * track);

		ctx.strokeCircle( origin.x, origin.y, localRadius );
	}

	// Radials. Render segmenting from the inner radius to the outer radius
	// representing each bit on each track. 
	beginAngle = 0;
	for ( unsigned int segmentId = 0; segmentId < segmentCount; ++segmentId )
	{
		const double endAngle = beginAngle + stepAngle;
		const double endPointAngRad = DegToRad( endAngle );

		double begX = m_innerRadius * cosf( endPointAngRad );
		double begY = m_innerRadius * sinf( endPointAngRad );

		double endX = m_outerRadius * cosf( endPointAngRad );
		double endY = m_outerRadius * sinf( endPointAngRad );

		ctx.strokeLine( {begX, begY}, {endX, endY} );
		beginAngle = endAngle;
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::Grays( int n )
{
	if ( n <= m_nFactor )
	{
		int iterateCount = pow( 2, n );
		int newElements = iterateCount / 2;

		int originals = newElements - 1;
		for ( int element = newElements; element < iterateCount; ++element, --originals )
		{
			unsigned int newElement = m_bits[originals] | 0x1 << (n - 1);
			m_bits[element] = newElement;
		}

		Grays( ++n );
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::Generate()
{
	unsigned int size = pow( 2, m_nFactor );
	m_bits.reserve( size );
	for ( int i = 0; i < size; i++ )
	{
		m_bits.push_back( 0 );
	}

	m_bits[1] = 1;
	Grays( 2 );
}

//------------------------------------------------------------------------------
// Actions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
actions::RenderAction& GraysEncoder::GetRenderAction()
{
	return m_renderAction;
}

//------------------------------------------------------------------------------
// Config Changed
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::SetGrayNumber( const uint8_t n )
{
	if ( n != m_nFactor )
	{
		m_nFactor = n;

		m_bits.clear();
		Generate();
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::SetInnerRadius( const double rad )
{
	m_innerRadius = rad;

	if ( m_innerRadius > m_outerRadius - 1 )
	{
		m_innerRadius = m_outerRadius - 1;
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::SetOuterRadius( const double rad )
{
	m_outerRadius = rad;

	if ( m_outerRadius < m_innerRadius + 1 )
	{
		m_outerRadius = m_innerRadius + 1;
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::SetInvert( const bool val )
{
	m_invertTree = val;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::DrawInstrumentation( const bool val )
{
	m_drawInstrumentation = val;
}
