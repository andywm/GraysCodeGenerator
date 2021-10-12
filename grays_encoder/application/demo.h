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
#pragma once
#include <blend2d/geometry.h>
#include <blend2d/rgba.h>
#include <blend2d/random.h>
#include "ui/properties_menu/property_panel.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class QTreeView;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// DemoProgram
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class DemoProgram
{
public:
	enum ShapeType
	{
		kShapeRect,
		kShapeRectPath,
		kShapeRoundRect,
		kShapePolyPath,
	};

	DemoProgram();

	void InitProperties( QTreeView& view, QObject* parent );
	void Initialise();

	void UpdateArea( float x, float y );

	void Tick( float dT );
	void Render( BLContext& ctx );


	//void OnRendererChanged( const QVariant& qvr );
	void OnCompOpChanged( const QVariant& qvr );
	void OnShapeTypeChanged( const QVariant& qvr );
	//void OnLimitFpsChanged( const QVariant& qvr );
	void OnSizeChanged( const QVariant& qvr );
	void OnCountChanged( const QVariant& qvr );



private:
	void setCount( size_t size );
	double randomSign() noexcept;
	BLRgba32 randomColor() noexcept;

private:
	PropertyPanel m_propertyPanel;
	float m_width = 0.0f;
	float m_height = 0.0f;

private:
	BLRandom m_random;

	std::vector<BLPoint> m_coords;
	std::vector<BLPoint> m_steps;
	std::vector<BLRgba32> m_colors;
	uint32_t m_compOp;
	uint32_t m_shapeType;
	float m_rectSize;
};
