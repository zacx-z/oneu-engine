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

		matrix m_Trans;
		float m_X, m_Y, m_Rotation, m_ScaleX, m_ScaleY, m_CenterX, m_CenterY;
		bool m_bTransLatest;//矩阵是否为最新 优化 避免每次都算矩阵
		video::BLENDMODE m_Mode;
		dword m_CMode;
	public:
		Sprite_Impl(image_t& img)
			: m_Img(img), color(0, 0, 0), m_X(0.0f), m_Y(0.0f), m_Rotation(0.0f),
			m_ScaleX(1.0f), m_ScaleY(1.0f), m_CenterX(0.0f), m_CenterY(0.0f), m_bTransLatest(false),
			m_Mode(video::BM_NORMAL), m_CMode(video::CBM_ADD){}
		void destroy(){ ONEU_DELETE(this);}

		void setImage(image_t img){ m_Img = img; }
		image_t getImage(){ return m_Img; }
		void setX(float nx){ m_X = nx; m_bTransLatest = false;}
		float getX() const { return m_X;}
		void setY(float ny){ m_Y = ny; m_bTransLatest = false;}
		float getY() const { return m_Y;}
		void setRotation(float nr){ m_Rotation = nr; m_bTransLatest = false;}
		float getRotation() const { return m_Rotation; }
		void setScaleX(float ns){ m_ScaleX = ns; m_bTransLatest = false;}
		float getScaleX() const { return m_ScaleX; }
		void setScaleY(float ns){ m_ScaleY = ns; m_bTransLatest = false;}
		float getScaleY() const { return m_ScaleY; }
		void setCenterX(float nx){ m_CenterX = nx; m_bTransLatest = false;}
		float getCenterX() const { return m_CenterX; }
		void setCenterY(float ny){ m_CenterY = ny; m_bTransLatest = false;}
		float getCenterY() const { return m_CenterY; }

		void setBlendMode(video::BLENDMODE mode){ m_Mode = mode;}
		video::BLENDMODE getBlendMode(){ return m_Mode; }
		void setColorBlendMode(dword mode){ m_CMode = mode; }
		dword getColorBlendMode(){ return m_CMode; }


		void setColor(color_t c){ color = c; }
		color_t getColor(){ return color; }
		void setAlpha(ubyte alpha){ color.setAlpha(alpha); }

		void paint();
		void describe(String& buffer, int depth){ buffer.append(L"<sprite>\n"); }
	};

	void Sprite_Impl::paint(){
		IVideo& Vi = GetVideo();
		vector2u_t ss = Vi.getDeviceSize();
		if(!m_bTransLatest){
			float w = (float)m_Img.get()->getWidth(), h = (float)m_Img.get()->getHeight();
			m_Trans = 
				matrix().setTranslation(vector3(-m_CenterX, -m_CenterY, 0.0f))
				* matrix().setScale(vector3(w * m_ScaleX, h * m_ScaleY, 1.0f))
				* matrix().setRotation(vector3(0.0f, 0.0f, m_Rotation))
				* matrix().setTranslation(vector3(m_X, m_Y, 0.0f))
				* matrix().setScale(vector3(2.0f / ss.x, -2.0f / ss.y, 1.0f))
				* matrix().setTranslation(vector3(-1.0f, 1.0f, 0.0f));
			m_bTransLatest = true;
		}
		Vi.setBlendColor(m_CMode, color);
		Vi.setBlendMode(m_Mode);
		Vi.setTransform(m_Trans);
		Vi.renderImage(*m_Img.get());
	}

	ONEU_API ISprite* Sprite_create(image_t& img)
	{
		return ONEU_NEW(Sprite_Impl(img));
	}
}