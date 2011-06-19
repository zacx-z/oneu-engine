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
* @file Stack.h
* @brief ’ª
* @author Ladace
* @version 1.0.0.1
* @date 2011-05-28
*/
#pragma once
#include "Base.h"
#include "List.h"

namespace OneU
{
	//T is primary type
	template<typename T, uint32 maxsize = 10>
	class Stack
	{
		T _contents[maxsize];
		uint32 _top;
	public:
		Stack()
			: _top(0){}
		T& top(){ ONEU_ASSERT(_top >= 1); return _contents[_top - 1];}
		void push(const T& v){ ONEU_ASSERT(_top < maxsize); _contents[_top++] = v; }
		T pop(){ ONEU_ASSERT(_top >= 1); return _contents[--_top];}
		uint32 size(){ return _top; }
		//extended
		T& at(uint32 index){ return _contents[index]; }
	};
	template<typename T>
	class Stack<T, -1>
	{
		List<T> _contents;
		uint32 n;
	public:
		Stack() : n(0){}
		//permit copying
		//...
		T& top(){ return _contents.back(); }
		void push(const T& v){ ++n; _contents.pushBack(v); }
		T pop(){ --n; T ret = _contents.back(); _contents.erase(_contents.end().prev()); return ret; }
		uint32 size(){ return n;}
	};

	//‘»Œ¡¥
	template<typename T>
	class Chain
	{
		List<T> _contents;
		uint32 n;
	public:
		Chain() : n(0){}
		//permit copying
		//...
		T& top(){ return _contents.front(); }
		void push(const T& v){ ++n; _contents.pushFront(v); }
		T pop(){ --n; T ret = _contents.front(); _contents.erase(_contents.begin()); return ret; }
		uint32 size(){ return n;}
		
		template<class functor>
		bool handle(functor f){
			for(List<T>::iterator it = _contents.begin(); it != _contents.end(); ++it){
				if(f(*it))
					return true;
			}
			return false;
		}
	};
}