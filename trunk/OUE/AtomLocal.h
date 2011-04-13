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
 * @file AtomLocal.h
 * @brief 引擎本地ATOM函数
 *
 * 按照OneU引擎对Atom表的约定，抽取出的使用ATOM组件的既定代码。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "Atom.h"

namespace OneU
{
	namespace atom
	{
		template<class T>
		inline T* GetVoidPtr(pcwstr symbol){
			value* s = GetAtom().getSystemEnv()->getSymbol(symbol);
			if(s)
				return *(T**)s->get<void*>();
			else return NULL;
		}
		//如果当前环境下有Scene_定义 则获得IScene接口 否则返回NULL。
		//当C++一个Scene对象企图用脚本初始化时 lua怎么获悉该对象？（知道该对象后才知道Sprite要加在哪个RenderScene中）
		//目前的解决方法是Scene对象配置好Scene_符号，再调用脚本。该过程被execWithScene封装。
		inline IScene* GetLocalScene(){
			return GetVoidPtr<IScene>(L"Scene_");
		}
		inline void* GetD3D9(){
			return GetVoidPtr<void>(L"Direct3D9_");
		}
		inline void* GetD3DDevice9(){
			return GetVoidPtr<void>(L"Direct3DDevice9_");
		}

		//导出Shell命令
		inline bool ExportShellCommand(pcwstr command, value* (*addr)(value*)){
			return ugetSymbol<env>(GetAtom().getEnv(), L"shell").createSymbol(command, atom::makeFunc(addr));
		}
	}
}
