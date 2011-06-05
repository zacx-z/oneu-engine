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
 * @file AtTuple.h
 * @brief ATOM Tuple值
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "AtValue.h"
#include <stdarg.h>
namespace OneU
{
	/**
	 * @addtogroup group_atom
	 */
	//@{
	namespace atom
	{
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief Tuple
		 */
		/* ----------------------------------------------------------------------------*/
		class tuple
		{
			value** ptr;
			uint32 _len;
			friend class value;
			friend value* makeTuple(uint32 l);
			friend value* makeTupleWithElem(uint32 l, ...);

			tuple(){}
			void init(uint32 l){
				_len = l;
				ptr = (value**)ONEU_ALLOC(l * sizeof(value*));
				for(uint32 i = 0; i < _len; ++i){
					ptr[i] = makeNil();
					ptr[i]->acquire();
				}
			}
			void initWithElem(uint32 l, va_list marker){
				_len = l;
				ptr = (value**)ONEU_ALLOC(l * sizeof(value*));

				for(uint32 i = 0; i < _len; ++i){
					ptr[i] = va_arg(marker, value*);
					if(ptr[i] == NULL || !ptr[i]->acquire())
					{
						ptr[i] = makeNil();
						ptr[i]->acquire();
					}
				}
			}
			tuple(const tuple&);
			tuple& operator=(const tuple&);
		public:
			~tuple(){
				for(uint32 i = 0; i < _len; ++i){
					ptr[i]->unacquire();
				}
				ONEU_DEALLOC(ptr);
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取长度
			 * 
			 * @returns 长度
			 */
			/* ----------------------------------------------------------------------------*/
			uint32 length(){ return _len;}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取第i个元素
			 * 
			 * @param i 元素索引，从0开始。
			 * 
			 * @returns 元素变量
			 */
			/* ----------------------------------------------------------------------------*/
			value* get(uint32 i){
				ONEU_ASSERT(i < _len);
				return ptr[i];
			}
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 创建Tuple
		 *
		 * 创建长度为l的Tuple。每个元素值为Nil。
		 * 
		 * @param l Tuple的长度
		 * 
		 * @returns Tuple
		 */
		/* ----------------------------------------------------------------------------*/
		inline value* makeTuple(uint32 l){
			value* ret = makeValue<tuple>();
			ret->get<tuple>()->init(l);
			return ret;
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 从元素创建Tuple
		 * 
		 * @param l Tuple的长度
		 * @param ... l个value*。 会向value获取控制权。
		 * 可将某个value*设为NULL，这样它将以Nil初始化
		 * @returns Tuple
		 */
		/* ----------------------------------------------------------------------------*/
		inline value* makeTupleWithElem(uint32 l, ...){
			va_list va_alist;
			va_start(va_alist, l);

			value* ret = makeValue<tuple>();
			ret->get<tuple>()->initWithElem(l, va_alist);

			return ret;
		}
	}
	//@}
}
