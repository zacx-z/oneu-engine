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
 * @file Label.h
 * @brief 标签
 *
 * 渲染元素。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#include "Video.h"

namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 标签
	 *
	 * 用于显示文字。
	 */
	/* ----------------------------------------------------------------------------*/
	class ILabel
		: public video::INode
	{
	public:
		virtual void setX(float x) = 0;
		virtual float getX() = 0;
		virtual void setY(float y) = 0;
		virtual float getY() = 0;
		virtual void setText(pcwstr text) = 0;
		virtual pcwstr getText() = 0;
		virtual void setColor(color_t color) = 0;
		virtual color_t getColor() = 0;
	};
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 标签工厂
	 *
	 * @param Width 显示宽度
	 * @param Height 显示高度
	 * @param fontSize 文字大小
	 * @param fontName 字体名称
	 *
	 * @return 标签
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API ILabel* Label_create(float Width, float Height, uint fontSize, pcwstr fontName = L"Arial");
}
