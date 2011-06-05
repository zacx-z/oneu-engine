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
#include "../DXGraphics/SpriteE.h"

namespace OneU
{
	class Label_Module
		: public video::IModule
	{
	public:
		DX::SpriteE sprite;
		Label_Module(){
			sprite.Create();
		}
	};
	static Label_Module* s_pModule = NULL;
	class Label_Impl
		: public ILabel
	{
		DX::FontE font;
		float m_Width, m_Height;
		String m_Text;
		color_t m_Color;
		uint32 m_Align;
	public:
		Label_Impl(float Width, float Height, uint32 fontSize, pcwstr fontName)
			: m_Width(Width), m_Height(Height), m_Color(255, 255, 255), m_Align(0)
		{
			if(!s_pModule)
				s_pModule = ONEU_NEW Label_Module;
			font.Create(fontSize, (uint32)(fontSize * 0.4f), 1, 1, 0, fontName);

			this->create2DTransform();
		}
		void setText(pcwstr text){
			m_Text = text;
		}
		pcwstr getText(){
			return m_Text.c_str();
		}
		void setColor(color_t color){ m_Color = color; }
		color_t getColor(){ return m_Color; }
		void setAlpha(ubyte alpha){ m_Color.setAlpha(alpha); }

		void setAlign(uint32 align){
			m_Align = align;
		}

		void paint(){
			if(m_Text == L"") return;
			GetVideo().setBlendMode(video::BM_NORMAL);
			uint32 flag = DT_EXPANDTABS;
			if(m_Align & T_RIGHT) flag |= DT_RIGHT;
			else if(m_Align & T_CENTER) flag |= DT_CENTER;
			if(m_Align & T_BOTTOM) flag |= DT_BOTTOM;
			else if(m_Align & T_VCENTER) flag |= DT_VCENTER;

			s_pModule->sprite.SetTransform((D3DMATRIX*)&GetVideo()._getTransform());
			s_pModule->sprite.Begin();
			font.DrawText(&s_pModule->sprite, m_Text.c_str(), (RECT*)&recti_t(0, 0, (int)m_Width, (int)m_Height), m_Color, flag);
			s_pModule->sprite.End();
		}
		void _describe(String& buffer, int depth){ buffer.append(L"<label>\n"); }
	};

	ONEU_API ILabel* Label_create( float Width, float Height, uint32 fontSize, pcwstr fontName /*= L"Arial"*/ ){
		return ONEU_NEW Label_Impl(Width, Height, fontSize, fontName);
	}

}