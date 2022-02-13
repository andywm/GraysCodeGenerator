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
#include <algorithm> 
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
	float radius,
	float width, 
	float startAngleDeg, 
	float arcAngleDeg 
)
{
	BLPoint centre = { 0, 0 };

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

	static double val = 0.2f;
	ctx.setStrokeWidth( val );
	ctx.strokePath( path );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void GraysEncoder::Render( BLContext& ctx )
{
	if ( m_bits.empty() )
	{
		Generate();
	}

	//Render Options
	static int test = BL_COMP_OP_SRC_OVER;// BL_COMP_OP_PLUS;

	ctx.setCompOp( test /*BL_COMP_OP_SRC_OVER*/ );

	//Draw background under the encoder ring
	//BLRgba32 backColour = BLRgba32( 0xFF000000 );
	BLRgba32 backColour = BLRgba32( 0xFFFFFFFF );


	ctx.setFillStyle( backColour );
	ctx.setStrokeStyle( backColour );
	ctx.setStrokeJoin( BL_STROKE_JOIN_MITER_BEVEL );
	DrawArcSegment( ctx, m_innerRadius + 1, m_outerRadius - m_innerRadius - 1.5, 0.0, 360.0 );

	//draw in white.
	BLRgba32 foreColour = BLRgba32( 0xFF000000 );
	//BLRgba32 foreColour = BLRgba32( 0xFFFFFFFF );
	ctx.setFillStyle( foreColour );
	ctx.setStrokeStyle( foreColour );

	const int segmentCount = pow( 2, m_nFactor );
	const double stepAngle = 360.0f / segmentCount;
	const double radDifference = m_outerRadius - m_innerRadius;
	const double trackWidth = radDifference / m_nFactor;
	double beginAngle = 0;

	int lastTrack = m_invertTree? m_nFactor-1 : 0;

	//draw each track concentrically.
	for( int track = 0; track < m_nFactor; ++track )
	{
		uint32_t numericWrap = 0;
		uint32_t firstDrawn = UINT32_MAX;
		uint32_t drawStart = UINT32_MAX;
		uint32_t drawEnd = UINT32_MAX;

		const double localRadius = m_invertTree
			? m_innerRadius + (trackWidth * track)
			: m_outerRadius - (trackWidth * (track + 1))
			;

		//draw arcs, go until we wrap back onto our first drawn arc.
		for( uint32_t sectorThis = 0; ; sectorThis++ )
		{
 			if( sectorThis >= segmentCount )
			{
				numericWrap++;
				sectorThis = 0;
				assert( numericWrap < 2 );
			}

			//we've wrapped.
			if( sectorThis == firstDrawn )
			{
				break;
			}

			const uint32_t sectorLast = std::min( sectorThis - 1u, segmentCount - 1u );

			const bool lastValue = (m_bits[sectorLast] & (0x1 << track)) != 0;
			const bool thisValue = (m_bits[sectorThis] & (0x1 << track)) != 0;

			if( lastValue == false && thisValue == true )
			{
				drawStart = sectorThis;
			}
			else if( drawStart != UINT32_MAX && thisValue == false )
			{
				drawEnd = sectorThis;
			}
	
			if( drawStart != UINT32_MAX && drawEnd != UINT32_MAX )
			{
				if( firstDrawn == UINT32_MAX )
				{
					firstDrawn = drawStart;
				}

				const double beginAngle = drawStart * stepAngle;
				const double endAngle = drawEnd * stepAngle;

				DrawArcSegment( ctx, localRadius, trackWidth, beginAngle, fabs( endAngle - beginAngle ) );
				drawStart = UINT32_MAX;
				drawEnd = UINT32_MAX;
			}
		}
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
	for( int track = 0; track <= m_nFactor; ++track )
	{
		double radDifference = m_outerRadius - m_innerRadius;
		double trackWidth = radDifference / m_nFactor;
		double localRadius = m_innerRadius + (trackWidth * track);

		ctx.strokeCircle( 0, 0, localRadius );
	}

	// Radials. Render segmenting from the inner radius to the outer radius
	// representing each bit on each track. 
	beginAngle = 0;
	for( unsigned int segmentId = 0; segmentId < segmentCount; ++segmentId )
	{
		const double endAngle = beginAngle + stepAngle;
		const double endPointAngRad = DegToRad( endAngle );

		double begX = m_innerRadius * cosf( endPointAngRad );
		double begY = m_innerRadius * sinf( endPointAngRad );

		double endX = m_outerRadius * cosf( endPointAngRad );
		double endY = m_outerRadius * sinf( endPointAngRad );

		ctx.strokeLine( { begX, begY }, { endX, endY } );
		beginAngle = endAngle;
	};	
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
