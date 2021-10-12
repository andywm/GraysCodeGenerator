/*------------------------------------------------------------------------------
	()      File:   demo.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Derived from blend2d's samples, specifically
				https://github.com/blend2d/blend2d-samples/blob/master/qt/src/bl-qt-rects.cpp
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
#include "application/demo.h"
#include "QVariant"
#include <blend2d/context.h>
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Demo Program
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DemoProgram::DemoProgram()
	: m_random( 0x1234 )
	, m_compOp( BL_COMP_OP_SRC_OVER )
	, m_shapeType( kShapeRect )
	, m_rectSize( 64.0 )
{}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::InitProperties( QTreeView& view, QObject* parent )
{
	//init
	m_propertyPanel.Initialise( view, parent );

	//persistent properties.

	//compare mode.
	m_propertyPanel.AddProperty( "root.compare", "Compare Op", BL_COMP_OP_SRC_OVER,
		{ { "SrcOver", 	BL_COMP_OP_SRC_OVER }
		,{ "SrcCopy", 	BL_COMP_OP_SRC_COPY }
		,{ "DstAtop", 	BL_COMP_OP_DST_ATOP }
		,{ "Xor", 		BL_COMP_OP_XOR }
		,{ "Plus", 		BL_COMP_OP_PLUS }
		,{ "Screen", 	BL_COMP_OP_SCREEN }
		,{ "Lighten", 	BL_COMP_OP_LIGHTEN }
		,{ "Hard Light", BL_COMP_OP_HARD_LIGHT }
		,{ "Difference", BL_COMP_OP_DIFFERENCE }
		}
	).Connect<DemoProgram, &DemoProgram::OnCompOpChanged>( *this );

	//shape.
	m_propertyPanel.AddProperty( "root.shape", "Shape", DemoProgram::kShapeRect,
		{ { "Rect", 		DemoProgram::kShapeRect	}
		,{ "RectPath", 	DemoProgram::kShapeRectPath }
		,{ "RoundRect", DemoProgram::kShapeRoundRect }
		,{ "Polygon", 	DemoProgram::kShapePolyPath }
		}
	).Connect<DemoProgram, &DemoProgram::OnShapeTypeChanged>( *this );

	//count
	m_propertyPanel.AddProperty( "root.count", "Count", (int)m_coords.size(), 1, 10000 )
		.Connect<DemoProgram, &DemoProgram::OnCountChanged>( *this );

	//size
	m_propertyPanel.AddProperty( "root.size", "Size", (int)m_rectSize, 8, 128 )
		.Connect<DemoProgram, &DemoProgram::OnSizeChanged>( *this );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::Initialise()
{
	setCount( 1 );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::UpdateArea( float x, float y )
{
	m_width = x;
	m_height = y;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::Tick(const float dT)
{
	size_t size = m_coords.size();
	
	for ( size_t i = 0; i < size; i++ )
	{
		BLPoint& vertex = m_coords[i];
		BLPoint& step = m_steps[i];

		vertex += step;
		if ( vertex.x <= 0.0 || vertex.x >= m_width )
		{
			step.x = -step.x;
			vertex.x = blMin( vertex.x + step.x, static_cast<double>(m_width) );
		}

		if ( vertex.y <= 0.0 || vertex.y >= m_height )
		{
			step.y = -step.y;
			vertex.y = blMin( vertex.y + step.y, static_cast<double>(m_height) );
		}
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::Render( BLContext& ctx )
{
	ctx.setFillStyle( BLRgba32( 0xFF000000 ) );
	ctx.fillAll();

	size_t i;
	size_t size = m_coords.size();

	double rectSize = m_rectSize;
	double halfSize = m_rectSize * 0.5;

	ctx.setCompOp( m_compOp );

	switch ( m_shapeType )
	{
	case kShapeRect:
	for ( i = 0; i < size; i++ ) {
		double x = m_coords[i].x - halfSize;
		double y = m_coords[i].y - halfSize;

		ctx.setFillStyle( m_colors[i] );
		ctx.fillRect( m_coords[i].x - halfSize, m_coords[i].y - halfSize, rectSize, rectSize );
	}
	break;

	case kShapeRectPath:
	for ( i = 0; i < size; i++ ) {
		double x = m_coords[i].x - halfSize;
		double y = m_coords[i].y - halfSize;

		BLPath path;
		path.addRect( x, y, rectSize, rectSize );

		ctx.setFillStyle( m_colors[i] );
		ctx.fillPath( path );
	}
	break;

	case kShapePolyPath:
	for ( i = 0; i < size; i++ ) {
		double x = m_coords[i].x - halfSize;
		double y = m_coords[i].y - halfSize;

		BLPath path;
		path.moveTo( x + rectSize / 2, y );
		path.lineTo( x + rectSize, y + rectSize / 3 );
		path.lineTo( x + rectSize - rectSize / 3, y + rectSize );
		path.lineTo( x + rectSize / 3, y + rectSize );
		path.lineTo( x, y + rectSize / 3 );

		ctx.setFillStyle( m_colors[i] );
		ctx.fillPath( path );
	}
	break;

	case kShapeRoundRect:
	for ( i = 0; i < size; i++ ) {
		double x = m_coords[i].x - halfSize;
		double y = m_coords[i].y - halfSize;

		ctx.setFillStyle( m_colors[i] );
		ctx.fillRoundRect( x, y, rectSize, rectSize, 10 );
	}
	break;
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
double DemoProgram::randomSign() noexcept
{
	return m_random.nextDouble() < 0.5 ? 1.0 : -1.0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
BLRgba32 DemoProgram::randomColor() noexcept
{
	return BLRgba32( m_random.nextUInt32() );
}


//------------------------------------------------------------------------------
// Demo Program Config Changed
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::setCount( size_t size )
{
	size_t i = m_coords.size();

	m_coords.resize( size );
	m_steps.resize( size );
	m_colors.resize( size );

	while ( i < size )
	{
		m_coords[i].reset( m_random.nextDouble() * m_width, m_random.nextDouble() * m_height );
		m_steps[i].reset( (m_random.nextDouble() * 0.5f + 0.05f) * randomSign(), (m_random.nextDouble() * 0.5f + 0.05f) * randomSign() );
		m_colors[i].reset( randomColor() );
		i++;
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::OnCompOpChanged( const QVariant& qvr )
{
	m_compOp = qvr.toInt();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::OnShapeTypeChanged( const QVariant& qvr )
{
	m_shapeType = qvr.toInt();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::OnSizeChanged( const QVariant& qvr )
{
	m_rectSize = qvr.toInt();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DemoProgram::OnCountChanged( const QVariant& qvr )
{
	setCount( static_cast<size_t>(qvr.toInt()) );
}
