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
#include <blend2d/geometry.h>
#include <blend2d/rgba.h>
#include <blend2d/random.h>
#include "ui/properties_menu/property_panel.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class QTreeView;
class RelativeMouseMove;
class RelativeMouseWheelMove;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// GraysEncoder
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class GraysEncoder
{
public:
	struct Mouse
	{
		BLPoint prev { FLT_MAX, FLT_MAX };
		BLPoint current { FLT_MAX, FLT_MAX };
	};

	void InitProperties( QTreeView& view, QObject* parent );
	void Initialise();

	void UpdateArea( float x, float y );
	void OnRelativeMouseMove( RelativeMouseMove& data );
	void OnScrollWheel( RelativeMouseWheelMove& data );

	void Tick( float dT );
	void Render( BLContext& ctx );

	void Generate();
	void Grays( int n );

private:
	void DrawArcSegment( BLContext& ctx, BLPoint& centre, float radius, float width, float startAngleDeg, float arcAngleDeg );

	void OnGrayChanged( const QVariant& qvr );
	void OnInnerRadiusChanged( const QVariant& qvr );
	void OnOuterRadiusChanged( const QVariant& qvr );
	void OnEndianChanged( const QVariant& qvr );
	void OnInstrumentationChanged( const QVariant& qvr );

private:
	PropertyPanel m_propertyPanel;
	float m_width = 0.0f;
	float m_height = 0.0f;

	//Viewport
	Mouse m_mouse;
	BLPoint m_userPosition{ 0,0 };
	float m_zoomLevel{ 1.0f };

	//Options
	int m_nFactor = 1;
	bool m_invertTree = false;
	bool m_drawInstrumentation = false;
	float m_innerRadius = 100.0f;
	float m_outerRadius = 200.0f;

	//Data
	std::vector<unsigned int> m_bits;
	bool m_dirty = true;
};
