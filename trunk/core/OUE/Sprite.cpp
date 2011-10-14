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
#include "Sprite.h"
#include "Game.h"

namespace OneU
{
	class Sprite_Impl
		: public ISprite
	{
		image_t m_Img;
		color_t color;

		float m_CenterH, m_CenterV;
		video::BLENDMODE m_Mode;
		uint32 m_CMode;
		rect m_SrcRc;
	public:
		Sprite_Impl(image_t& img)
			: m_Img(img), color(0, 0, 0), m_CenterH(0.0f), m_CenterV(0.0f),
			m_Mode(video::BM_NORMAL), m_CMode(video::CBM_ADD),
			m_SrcRc(0.0f, 0.0f, 1.0f, 1.0f){
			this->create2DTransform();
		}
		~Sprite_Impl(){
		}

		void setImage(image_t img){ m_Img = img; }
		image_t getImage(){ return m_Img; }
		void setCenterH(float nx){ m_CenterH = nx;}
		float getCenterH() const { return m_CenterH; }
		void setCenterV(float ny){ m_CenterV = ny;}
		float getCenterV() const { return m_CenterV; }

		void setColor(color_t c){ color = c; }
		color_t getColor(){ return color; }
		void setAlpha(ubyte alpha){ color.setAlpha(alpha); }

		virtual void setSrcRect(rect rc){ m_SrcRc = rc; }
		virtual rect getSrcRect(){ return m_SrcRc; }

		virtual float getWidth(){ return getImage()->getWidth() * (m_SrcRc.right - m_SrcRc.left); }
		virtual float getHeight(){ return getImage()->getHeight() * (m_SrcRc.bottom - m_SrcRc.top); }

		void setBlendMode(video::BLENDMODE mode){ m_Mode = mode;}
		video::BLENDMODE getBlendMode(){ return m_Mode; }
		void setColorBlendMode(uint32 mode){ m_CMode = mode; }
		uint32 getColorBlendMode(){ return m_CMode; }

		void paint();
		void _describe(String& buffer, int depth){ buffer.append(L"<sprite>\n"); }
	};

	void Sprite_Impl::paint(){
		IVideo& Vi = GetVideo();
		vector2i_t ss = Vi.getDeviceSize();

		Vi.setBlendColor(m_CMode, color);
		Vi.setBlendMode(m_Mode);
		Vi.selectImageSourceRect(m_SrcRc);
		float w = (float)m_Img.get()->getWidth() * (m_SrcRc.right - m_SrcRc.left), h = (float)m_Img.get()->getHeight() * (m_SrcRc.bottom - m_SrcRc.top);
		Vi.renderImage(*m_Img.get(), rect(-m_CenterH * w, -m_CenterV * h, (1 - m_CenterH) * w, (1 - m_CenterV) * h));
	}

	ONEU_API ISprite* Sprite_create(image_t& img)
	{
		return ONEU_NEW Sprite_Impl(img);
	}
}