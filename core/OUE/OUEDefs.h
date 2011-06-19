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
 * @file OUEDefs.h
 * @brief 定义文件
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "Base.h"
#include <windows.h>

//前置声明
namespace OneU
{
	class IVideo;
	class IControl;
	class IScene;
	class IRendererScene;
	class IGame;

	//类型声明
	class image_t;
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 颜色
	 */
	/* ----------------------------------------------------------------------------*/
	class color_t
	{
		uint32 c;
	public:
		color_t(){}
		color_t(uint32 c) : c(c){}
		color_t(ubyte red, ubyte green, ubyte blue, ubyte alpha = 255)
			: c(alpha << 24 | red << 16 | green << 8 | blue)
		{}
		operator uint32(){ return c; }
		ubyte getAlpha(){ return (ubyte)(c >> 24 & 0xff); }
		ubyte getRed(){ return (ubyte)(c >> 16 & 0xff); }
		ubyte getGreen(){ return (ubyte)(c >> 8 & 0xff); }
		ubyte getBlue(){ return (ubyte)(c & 0xff); }

		void setAlpha(ubyte alpha){ c =  0xffffff & c | alpha << 24; }
		void setRed(ubyte red){ c = 0xff00ffff & c | red << 16; }
		void setGreen(ubyte green){ c = 0xffff00ff & c | green << 8; }
		void setBlue(ubyte blue){ c = 0xffffff00 & c | blue; }
	};
}

#include "Vector.h"