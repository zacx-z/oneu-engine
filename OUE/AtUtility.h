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
 * @file AtUtility.h
 * @brief ATOM实用辅助库
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
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @defgroup group_atom_utility 实用库
		 * @ingroup group_atom
		 * @{
		 */
		/* ----------------------------------------------------------------------------*/
		template<class T>
		inline T& uget(value* v){
			T* retval = v->get<T>();
			if(retval == NULL)
				ATOM_RAISE(String().format(L"type mismatch, expected %s, got %s",
				typeName(_atom_traits<T>::type_id), typeName(v->type())).c_str());
			return *retval;
		}

		template<class T>
		inline T uto(value* v){
			T retval;
			if(!to<T>(v, retval))
				ATOM_RAISE(String().format(L"can't convert %s to %s",
					typeName(v->type()), typeName(_atom_traits<T>::type_id)).c_str());
			return retval;
		}
		inline uint uArgN(value* v){
			return v->get<tuple>()->length();
		}
		template<class T>
		inline T& ugetArg(value* v, uint i){
			if(i >= v->get<tuple>()->length())
				ATOM_RAISE(L"too few arguments");
			T* ret = v->get<tuple>()->get(i)->get<T>();
			if(ret == NULL)
				ATOM_RAISE(String().format(L"argument %d expected %s, got %s", i,
					typeName(_atom_traits<T>::type_id), typeName(v->get<tuple>()->get(i)->type())).c_str());
			return *ret;
		}

		template<class T>
		inline T utoArg(value* v, uint i){
			if(i >= v->get<tuple>()->length())
				ATOM_RAISE(L"too few arguments");
			T ret;
			if(!to<T>(v->get<tuple>()->get(i), ret))
				ATOM_RAISE(String().format(L"argument %d expected %s, got %s", i,
				typeName(_atom_traits<T>::type_id), typeName(v->get<tuple>()->get(i)->type())).c_str());
			return ret;
		}

		inline void ucreateSymbol(env* e, pcwstr symbol, value* v){
			if(!e->createSymbol(symbol, v))
				ATOM_RAISE(String().format(L"cannot create symbol:%s", symbol).c_str());
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 获取符号对应变量的内容
		* 
		* @tparam 内容类型
		*
		* @param e 变量
		* @param symbol 符号
		* 
		* @returns 变量内容
		*/
		/* ----------------------------------------------------------------------------*/
		template<class T>
		inline T& ugetSymbol(env* e, pcwstr symbol){
			value* ret = e->getSymbol(symbol);
			if(ret == NULL){
				ATOM_RAISE(String().format(L"can't get symbol %s", symbol).c_str());
			}
			T* retval = ret->get<T>();
			if(retval == NULL)
				ATOM_RAISE(String().format(L"symbol %s type mismatch, expected %s, got %s",
					symbol, typeName(_atom_traits<T>::type_id), typeName(ret->type())).c_str());
			return *retval;
		}//utility
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 获取函数
		* 
		* @param k 类
		* @param methodname 函数符号
		* 
		* @returns object function地址，若为NULL获取失败。
		*/
		/* ----------------------------------------------------------------------------*/
		inline obj_function* ugetMethod(klass* k, pcwstr methodname){//utility
			return &ugetSymbol<obj_function>(k, methodname);
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 导出函数
		*
		* 便利函数，创建object function加入表中。
		* 
		* @param k 类
		* @param methodname 函数符号
		* @param ptr 成员函数指针（必须为exported派生类的成员函数）
		* 
		* @returns 表示是否成功，若已存在该符号，则导出失败。
		*/
		/* ----------------------------------------------------------------------------*/
		template<class T>
		inline void uexportMethod(klass* k, pcwstr methodname, value* (T::*ptr)(value*)){//utility
			if(!k->createSymbol(methodname, makeObjFunc(k, ptr)))
				ATOM_RAISE(String().format(L"can't export method %s", methodname).c_str());
		}
		inline value* ucall(obj* o, pcwstr funcname, value* arg){
			obj_function* f = ugetMethod(o->getKlass(), funcname);
			return o->call(f, arg);
		}
		//@}
	}
}
