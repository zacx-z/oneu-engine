/*
This source file is part of OneU Engine.
Copyright (c) 2011 Ladace

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "Label.h"
#include "Game.h"
#include "../DXGraphics/FontE.h"

namespace OneU
{
	class Label_Impl
		: public ILabel
	{
		DX::FontE font;
		float m_Width, m_Height, m_X, m_Y;
		String m_Text;
		color_t m_Color;
		dword m_Align;
	public:
		Label_Impl(float Width, float Height, uint fontSize, pcwstr fontName)
			: m_Width(Width), m_Height(Height), m_X(0.0f), m_Y(0.0f), m_Color(255, 255, 255), m_Align(0)
		{
			font.Create(fontSize, (uint)(fontSize * 0.4f), 1, 1, 0, fontName);
		}
		void setX(float x){ m_X = x; }
		float getX(){ return m_X; }
		void setY(float y){ m_Y = y; }
		float getY(){ return m_Y; }
		void setText(pcwstr text){
			m_Text = text;
		}
		pcwstr getText(){
			return m_Text.c_str();
		}
		void setColor(color_t color){ m_Color = color; }
		color_t getColor(){ return m_Color; }
		void setAlpha(ubyte alpha){ m_Color.setAlpha(alpha); }

		void setAlign(dword align){
			m_Align = align;
		}

		void paint(){
			if(m_Text == L"") return;
			GetVideo().setBlendMode(video::BM_NORMAL);
			dword flag = DT_EXPANDTABS;
			if(m_Align & T_RIGHT) flag |= DT_RIGHT;
			else if(m_Align & T_CENTER) flag |= DT_CENTER;
			if(m_Align & T_BOTTOM) flag |= DT_BOTTOM;
			else if(m_Align & T_VCENTER) flag |= DT_VCENTER;
			font.DrawText(m_Text.c_str(), (RECT*)&recti_t((int)m_X, (int)m_Y, (int)(m_X + m_Width), (int)(m_Y + m_Height)), m_Color, flag);
		}
		void describe(String& buffer, int depth){ buffer.append(L"<label>\n"); }
	};

	ONEU_API ILabel* Label_create( float Width, float Height, uint fontSize, pcwstr fontName /*= L"Arial"*/ ){
		return ONEU_NEW(Label_Impl(Width, Height, fontSize, fontName));
	}

}