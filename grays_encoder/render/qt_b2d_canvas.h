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
#include <cmath>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include <QtGui>
#include <QtWidgets>
#include <functional>
#include "utility/types_helper.h"
#include "qtmetamacros.h"
#include "QObject"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forwards
//------------------------------------------------------------------------------
class RelativeMouseMove;
class RelativeMouseWheelMove;
// namespace canvas_internal
// {
// 	struct Mouse
// 	{
// 		enum ButtonState
// 		{
// 			LMB = Qt::MouseButton::LeftButton,
// 			RMB = Qt::MouseButton::RightButton,
// 			MMB = Qt::MouseButton::MiddleButton,
// 			Scroll
// 		};
// 		uint32_t previous_button_state;
// 		uint32_t current_button_state;
// 		BLPoint previous_absolute_pos;
// 		BLPoint current_absolute_pos;
// 	};
// }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// QT_B2D_Canvas
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class QT_B2D_Canvas final : public QWidget
{
	Q_OBJECT;

public:
	QT_B2D_Canvas();
	~QT_B2D_Canvas() = default;

public:
	//qt
	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent( QWheelEvent* event );

public:
	//interface
	template<typename T, void (T::*fun_ptr)(BLContext&)>
	void SetRenderFunction(T& obj)
	{
		BindFunction<T, BLContext, fun_ptr>(obj, fn_onRenderB2D);
	}	
	
	template<typename T, void (T::*fun_ptr)(RelativeMouseMove&)>
	void SetRelativeMouseMoveCallback(T& obj)
	{
		BindFunction<T, RelativeMouseMove, fun_ptr>(obj, fn_relativeMouseMove);
	}	
	
	template<typename T, void (T::* fun_ptr)(RelativeMouseWheelMove&)>
	void SetMouseWheelMoveCallback( T& obj )
	{
		BindFunction<T, RelativeMouseWheelMove, fun_ptr>( obj, fn_scrollWheelMove );
	}

	inline const BLImage& GetImageBuffer() const;
	inline void SetNumberOfRenderThreads( uint32_t threads );
	inline uint32_t GetRenderThreads() const;
	inline float GetFPS() const;

	void OnUpdateCanvas(bool force=false);

private:
	template<typename ObjType, class ParamType, void (ObjType::*fun_ptr)(ParamType&)>
	void BindFunction( ObjType& obj, std::function<void(ParamType&)>& fnstd )
	{
 		void (ObjType::*fn)(ParamType&) = fun_ptr;
		auto bindme = [&obj, fn](ParamType& param)
		{
			(obj.*fn)(param);
		};

		fnstd = bindme;
	}

private:
	void OnResize();
	void OnRender();
	void OnPostRender();

public:
	QImage m_qtImageBuffer;
	BLImage m_blImageBuffer;

	QElapsedTimer m_elapsedTimer;
	BLPoint m_mousePrevious;
	BLPoint m_mouseCurrent;;

	//canvas_internal::Mouse m_mouseState;
	uint32_t m_renderThreads = 16;
	uint32_t m_frameCount = 0;
	bool m_dirty = true;
	float m_fps = 0.0f;

private:
	std::function<void(BLContext&)> fn_onRenderB2D;
	std::function<void(RelativeMouseMove&)> fn_relativeMouseMove;
	std::function<void(RelativeMouseWheelMove&)> fn_scrollWheelMove;
};

//------------------------------------------------------------------------------
// Inline for QT_B2D_Canvas
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
inline const BLImage& QT_B2D_Canvas::GetImageBuffer() const
{
	return m_blImageBuffer;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
inline void QT_B2D_Canvas::SetNumberOfRenderThreads( uint32_t threads )
{
	m_renderThreads = threads;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
inline uint32_t QT_B2D_Canvas::GetRenderThreads() const
{
	return m_renderThreads;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
inline float QT_B2D_Canvas::GetFPS() const
{
	return m_fps; 
}
