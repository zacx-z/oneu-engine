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
/**
 * @file Sprite.h
 * @brief 精灵
 *
 * 渲染元素。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "Video.h"

namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 精灵接口
	 * @sa Sprite_create
	 */
	/* ----------------------------------------------------------------------------*/
	class ISprite
		: public video::INode
	{
	public:
		virtual void setImage(image_t img) = 0;
		virtual image_t getImage() = 0;
		//已被归一化
		virtual void setCenterH(float nx) = 0;
		virtual float getCenterH() const = 0;
		virtual void setCenterV(float ny) = 0;
		virtual float getCenterV() const = 0;

		virtual void setColor(color_t color) = 0;
		virtual color_t getColor() = 0;
		virtual void setAlpha(ubyte alpha) = 0;
		ubyte getAlpha(){ return getColor().getAlpha(); }

		virtual void setSrcRect(rect rc) = 0;
		virtual rect getSrcRect() = 0;

		virtual float getWidth() = 0; //计算了SrcRect 可能为负
		virtual float getHeight() = 0;//计算了SrcRect 可能为负

		virtual void setBlendMode(video::BLENDMODE mode) = 0;
		virtual video::BLENDMODE getBlendMode() = 0;
		virtual void setColorBlendMode(uint32 mode) = 0;
		virtual uint32 getColorBlendMode() = 0;
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 精灵工厂
	 * 
	 * @param img 图形文件
	 * 
	 * @returns 精灵接口
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API ISprite* Sprite_create(image_t& img);
}
