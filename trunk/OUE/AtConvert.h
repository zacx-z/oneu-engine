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
 * @file AtConvert.h
 * @brief ATOM变量转换函数。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "AtValue.h"

namespace OneU
{
	namespace atom
	{
		template<class T>
		inline bool to(value* v, T& val){
			T* ret = v->get<T>();
			if(ret == NULL) return false;
			else { val = *ret; return true; }
		}
		template<>
		inline bool to<int>(value* v, int& val){
			if(v->type() == T_FLOAT){
				val = (int)*v->get<float>();
				return true;
			}
			if(v->type() == T_STR){
				pcwstr str = v->get<String>()->c_str();
				val = _wtoi(str);
				return true;
			}
			int* ret = v->get<int>();
			if(ret == NULL) return false;
			else { val = *ret; return true; }
		}
		template<>
		inline bool to<float>(value* v, float& val){
			if(v->type() == T_INT){
				val = (float)*v->get<int>();
				return true;
			}
			if(v->type() == T_STR){
				pcwstr str = v->get<String>()->c_str();
				val = (float)_wtof(str);
				return true;
			}
			float* ret = v->get<float>();
			if(ret == NULL) return false;
			else { val = *ret; return true; }
		}
	}
}
