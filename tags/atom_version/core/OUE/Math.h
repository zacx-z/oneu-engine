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
#pragma once

#define ONEU_EPS 1e-6

namespace OneU
{
	inline bool FloatEqual(float a, float b){
		if(a >= b - ONEU_EPS && a <= b + ONEU_EPS)
			return true;
		return false;
	}
	/*@cond INTERNAL_MPL*/
	template<typename T>
	struct _scalar_traits{
		static bool isEqual(T& a, T& b){
			return a == b;
		}
	};
	template<>
	struct _scalar_traits<float>{
		static bool isEqual(float& a, float& b){
			return FloatEqual(a, b);
		}
	};

	/*@endcond*///INTERNAL_MPL
}