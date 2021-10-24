/*------------------------------------------------------------------------------
	()      File:   qt_b2d_canvas.h
	/\      Copyright (c) 2021 Andrew Woodward-May
   //\\
  //  \\    Description:
				Canvas for managing rendering a blend2d canvas onto a qt canvas.
				Heavily based on the blend2d sample code.
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
#include <blend2d.h>
#include <QObject>
#include <QImage>
#include <QWidget>
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forwards
//------------------------------------------------------------------------------
namespace actions
{
	class RenderAction;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// QT_B2D_Canvas
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class Blend2DRenderWidget final : public QWidget
{
	Q_OBJECT;

public:
	Blend2DRenderWidget();
	~Blend2DRenderWidget() = default;

	void Invalidation();

	inline void SetRenderFunction( actions::RenderAction& render );
	inline void SetNumberOfRenderThreads( uint32_t threads );
	inline uint32_t GetRenderThreads() const;

protected:
	//qt
	void resizeEvent( QResizeEvent* event ) override;
	void paintEvent( QPaintEvent* event ) override;
	void mousePressEvent( QMouseEvent* event ) override;
	void mouseMoveEvent( QMouseEvent* event ) override;
	void wheelEvent( QWheelEvent* event ) override;

private:
	void UpdateRenderBuffer();

private:
	//render
	actions::RenderAction* m_renderer = nullptr;
	uint32_t m_renderThreads = 16;
	QImage m_renderBuffer;
	BLImage m_b2dRenderTarget;

	//user interaction
	BLPoint m_mousePrevious {0,0};
	BLPoint m_mouseCurrent{ 0,0 };
	BLPoint m_userPosition{ 0,0 };
	float m_zoomLevel{ 1.0f };
};

//------------------------------------------------------------------------------
// Inline for QT_B2D_Canvas
//------------------------------------------------------------------------------

inline void Blend2DRenderWidget::SetRenderFunction( actions::RenderAction& render )
{
	m_renderer = &render;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
inline void Blend2DRenderWidget::SetNumberOfRenderThreads( uint32_t threads )
{
	m_renderThreads = threads;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
inline uint32_t Blend2DRenderWidget::GetRenderThreads() const
{
	return m_renderThreads;
}
