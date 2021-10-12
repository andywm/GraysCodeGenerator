/*------------------------------------------------------------------------------
	()      File:   GrayEncoder.cpp
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
#define SDL_MAIN_HANDLED

#include <iostream>
#include <cmath>
#include "SDL.h"
#include "SDL2_gfx.h"
#include "sdlgui/Screen.h"
#include "sdlgui/common.h"
#include "sdlgui/layout.h"
#include "sdlgui/window.h"
#include "sdlgui/label.h"
#include "sdlgui/Button.h"
#include "sdlgui/textbox.h"
#include "sdlgui/checkbox.h"
#include "sdlgui/slider.h"
#include <xutility>
#include <bitset>
#include <iomanip>

float mathRadToDeg(float radians)
{
	return radians * (180.0f / M_PI);
}

float mathDegToRad(float degrees)
{
	return degrees * (M_PI / 180.0f);
}

template<typename T>
float lerp(T v0, T v1, float t) 
{
	return (1 - t) * v0 + t * v1;
}

template<typename T>
float invLerp(T v0, T v1, float t) 
{
	return 1.0f - ((v1 - t) / (v1 - v0));
}

namespace resolution
{
	constexpr int width = 1920;
	constexpr int height = 1080;

	constexpr int centre_x = width / 2;
	constexpr int centre_y = height / 2;
}

namespace magic
{
	constexpr int concentric_spacing = 10;

	constexpr int min_radius = 100;
	constexpr int max_radius = 500;
	constexpr int min_n_factor = 1;
	constexpr int max_n_factor = 9;
	constexpr int default_n_factor = 2;
}

//IR linear photodiode array

class GrayEncoderApplication
{
public:
	constexpr static char s_bigEndianLabel[] = "Big Endian";
	constexpr static char s_littleEndianLabel[] = "Little Endian";

public:
	void Draw(SDL_Renderer* renderer)
	{
		if (m_bits.empty() || m_dirty)
		{
			m_bits.clear();
			Generate();
		}

		int segmentCount = pow(2, m_nFactor);
		float stepAngle = 360.0f / segmentCount;
		float beginAngle = 0;

		for( unsigned int segmentId = 0; segmentId < segmentCount; ++segmentId )
		{
			unsigned int grayCode = m_bits[segmentId];
			if( !m_bigEndian )
			{
				int clearMask = UINT32_MAX;
				for (int i = 0; i < m_nFactor; ++i)
				{
					clearMask &= ~(0x1 << i);
				}

				grayCode = ~grayCode;
				grayCode ^= clearMask;
			}
			
			float endAngle = beginAngle + stepAngle;

			for (int bit = 0; bit < m_nFactor; ++bit)
			{
				if( (grayCode & (0x1 << bit)) == 0 )
				{
					continue;
				}

				int localRadius = m_radius + magic::concentric_spacing * bit;

				int bAng = std::round(beginAngle);
				int eAng = std::round(endAngle);

				arcRGBA(renderer, resolution::centre_x, resolution::centre_y, localRadius, bAng, eAng, 0xff, 0xff, 0xff, 0xff);
				arcRGBA(renderer, resolution::centre_x, resolution::centre_y, localRadius + 1, bAng, eAng, 0xff, 0xff, 0xff, 0xff);
				arcRGBA(renderer, resolution::centre_x, resolution::centre_y, localRadius + 2, bAng, eAng, 0xff, 0xff, 0xff, 0xff);
				arcRGBA(renderer, resolution::centre_x, resolution::centre_y, localRadius + 3, bAng, eAng, 0xff, 0xff, 0xff, 0xff);
				arcRGBA(renderer, resolution::centre_x, resolution::centre_y, localRadius + 4, bAng, eAng, 0xff, 0xff, 0xff, 0xff);
			}
			beginAngle = endAngle;
		}

		//second pass to draw ontop.
		beginAngle = 0;
		for (unsigned int segmentId = 0; segmentId < segmentCount; ++segmentId)
		{
			float endAngle = beginAngle + stepAngle;
			float endPointAngRad = mathDegToRad(endAngle);
			int rad = m_radius + (magic::concentric_spacing * m_nFactor);

			int begX = resolution::centre_x + m_radius * cosf(endPointAngRad);
			int begY = resolution::centre_y + m_radius * sinf(endPointAngRad);

			int endX = resolution::centre_x + rad * cosf(endPointAngRad);
			int endY = resolution::centre_y + rad * sinf(endPointAngRad);

			aalineRGBA(renderer, begX, begY, endX, endY, 0x11, 0x11, 0x11, 0xff);
			beginAngle = endAngle;
		}

		for (int i = 0; i <= m_nFactor; ++i)
		{
			aacircleRGBA(renderer, resolution::centre_x, resolution::centre_y, m_radius + (magic::concentric_spacing * i) - 1, 0x11, 0x11, 0x11, 0xff);
			aacircleRGBA(renderer, resolution::centre_x, resolution::centre_y, m_radius + (magic::concentric_spacing * i) - 2, 0x11, 0x11, 0x11, 0xff);
		}
	}

	void SetRadius(float v)
	{
		m_radius = v;
	}

	void SetNFactor(int n)
	{
		if (n != m_nFactor)
		{
			m_nFactor = n;
			m_dirty = true;
		}
	}

	void InvertEndian()
	{
		m_bigEndian = !m_bigEndian;
	}

	bool GetEndianness()
	{
		return m_bigEndian;
	}

	void Generate()
	{
		unsigned int size = pow(2, m_nFactor);
		m_bits.reserve(size);
		for (int i = 0; i < size; i++)
		{
			m_bits.push_back(0);
		}

		m_bits[1] = 1;
		Grays(2);

		std::cout << "printing grays for n =: " << m_nFactor << std::endl;

		for (unsigned int gray : m_bits)
		{
			std::stringstream stream;
			stream << std::bitset<32>(gray) << std::endl;
			std::string str = stream.str();
			str.erase(0, std::min(str.find_first_not_of('0'), str.size() - (m_nFactor+1)));
			std::cout << str;
		}
		std::cout << "--------------" << std::endl;

		m_dirty = false;
	}

	void Grays(int n)
	{
		if (n <= m_nFactor)
		{
			int iterateCount = pow(2, n);
			int newElements = iterateCount / 2;

			int originals = newElements - 1;
			for (int element = newElements; element < iterateCount; ++element, --originals)
			{
				unsigned int newElement = m_bits[originals] | 0x1 << (n-1);
				m_bits[element] = newElement;
			}

			Grays(++n);
		}
	}

private:
	std::vector<unsigned int> m_bits;
	int m_nFactor = 1;
	bool m_dirty = true;
	bool m_bigEndian = true;
	float m_radius = 100.0f;

}g_grays;

template<typename T>
void AddTweakSlider(sdlgui::Widget& widget, const char* paramName, T low, T high, T defaultValue, std::function<void(T)> callback = nullptr )
{
	auto& box = widget.widget().boxlayout(sdlgui::Orientation::Vertical, sdlgui::Alignment::Fill, 0, 6);

	auto& sliderBox = box.label(paramName, "sans-bold")
		.withFixedWidth(80);

	auto& valueBox = box.textbox("");

	auto sdlguiCbk = [&valueBox, callback](sdlgui::Slider* s, float v)
	{
		float rawValue = lerp(s->range().first, s->range().second, v);
		T value = static_cast<T>(rawValue);

		std::stringstream streamer;

		streamer << std::fixed << std::setprecision(2) << value;

		((sdlgui::TextBox&)valueBox).setValue(streamer.str());
		((sdlgui::TextBox&)valueBox).setAlignment(sdlgui::TextBox::Alignment::Left);

		if (callback)
		{
			callback(value);
		}
	};

	float defaultRLerp = invLerp(low, high, defaultValue);

	auto& slider = box.slider(defaultRLerp, sdlguiCbk);
	slider.setRange({ static_cast<float>(low), static_cast<float>(high)});
	sdlguiCbk(&slider, defaultRLerp);
}

void InitTweaks(sdlgui::Screen& guiContext, SDL_Renderer* renderer)
{
	auto& guiWindow = guiContext.wdg<sdlgui::Window>("Grays Params")
		.withLayout<sdlgui::GroupLayout>()
		.withPosition({ 100,100 });

	AddTweakSlider<float>(guiWindow, "Radius", magic::min_radius, magic::max_radius, magic::min_radius, [](float v) {g_grays.SetRadius(v); });
	AddTweakSlider<int>(guiWindow, "N Gray", magic::min_n_factor, magic::max_n_factor, magic::default_n_factor, [](int v) {g_grays.SetNFactor(v); });

	auto& boxLayout = guiWindow.widget().boxlayout(sdlgui::Orientation::Horizontal, sdlgui::Alignment::Middle, 0, 14);
	auto& endianLabel = boxLayout.label( (g_grays.GetEndianness() ? g_grays.s_bigEndianLabel : g_grays.s_littleEndianLabel), "sans-bold");
	auto& endianToggle = boxLayout.button("Invert", [&endianLabel](bool state)
	{
		g_grays.InvertEndian();
		endianLabel.setCaption(g_grays.GetEndianness() ? g_grays.s_bigEndianLabel : g_grays.s_littleEndianLabel);

	}).withFlags(sdlgui::Button::ToggleButton | sdlgui::Button::PopupButton);
	
	guiContext.performLayout(renderer);
}

int main(int argc, char* argv[])
{
	std::cout << "Init SDL!\n";
	SDL_Renderer* renderer = NULL;

	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Fatal. SDL failed to init video, sdl error: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("Gray's Encoder v1.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution::width, resolution::height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);;
	if (window == nullptr)
	{
		std::cout << "Fatal. SDL failed to create window, sdl error: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_UpdateWindowSurface(window);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);

	sdlgui::Screen guiContext(window, { resolution::width, resolution::height }, "Gray's Encoder v1.0", false, false);
	InitTweaks(guiContext, renderer);

	bool quit = false;
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				std::cout << "Exit World!\n";
			}
			guiContext.onEvent(e);
		}

		SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xff);
		SDL_RenderClear(renderer);
		g_grays.Draw(renderer);
		guiContext.drawAll();

		// Render the rect to the screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}


// 	guiWindow.textbox("Radius")
// 		.withFixedSize(sdlgui::Vector2i(60, 25))
// 		;


// 	guiWindow.label("Push buttons", "sans-bold")._and()
// 		.widget().boxlayout(sdlgui::Orientation::Vertical, sdlgui::Alignment::Middle, 0, 6)
// 		.button("Button 1", []() { std::cout << "1 pushed!" << std::endl; })
// 		.withTooltip("This is plain button tips")._and()
// 		.button("Button 2", []() { std::cout << "2 pushed!" << std::endl; })
// 		.withTooltip("This is plain button tips")._and()
// 		.button("Button 2", []() { std::cout << "3 pushed!" << std::endl; })
// 		.withTooltip("This is plain button tips")
// 		;
// 
// 	guiWindow.label("Other Shit", "sans-bold")._and()
// 		.widget().boxlayout(sdlgui::Orientation::Vertical, sdlgui::Alignment::Middle, 0, 6)
// 		.button("Toggle", [](bool state) { std::cout << "Toggle button state: " << state << std::endl; })
// 		.withFlags(sdlgui::Button::ToggleButton)._and()
// 		.checkbox("Flag 1", [](bool state) { std::cout << "Check box 1 state: " << state << std::endl; })
// 		.withChecked(true)._and()
// 		.button("Radio")
// 		.withFlags(sdlgui::Button::RadioButton)
// 		;