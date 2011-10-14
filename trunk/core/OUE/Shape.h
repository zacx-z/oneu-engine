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
 * @file Shape.h
 * @brief 形状
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
	 * @brief 形状
	 *
	 * 用于显示形状
	 */
	/* ----------------------------------------------------------------------------*/
	class IShape
		: public video::INode
	{
	public:
		virtual void setColor(color_t color) = 0;
		virtual color_t getColor() = 0;

		virtual void setMode(bool bBorder) = 0;
		virtual bool getMode() = 0;
	};

	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 创建矩形形状显示元素
	 *
	 * @param rc 所显示的矩形
	 *
	 * @return 形状接口
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API IShape* Shape_rect(const rect& rc);
}
