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
 * @file AtFunc.h
 * @brief ATOM函数值
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
		inline value* _convert_to_tuple(value* arg){
			if(arg == NULL) arg = makeTuple(0);
			else if(arg->type() != T_TUPLE) arg = makeTupleWithElem(1, arg);
			return arg;
		}

		class function
		{
		public:
			typedef value* (*addr_t)(value*);
		private:
			friend class value;
			friend value* makeFunc(value* (*addr)(value*));

			addr_t _addr;
			function(){
			}
			void init(addr_t ptr){
				_addr = (addr_t)ptr;
			}
		public:
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 调用函数
			 *
			 * @param arg 参数。可为NULL表示无参、tuple表示一组参数、其余类型value表示单参数。
			 *
			 * @return 
			 */
			/* ----------------------------------------------------------------------------*/
			value* call(value* arg){
				arg = _convert_to_tuple(arg);
				return _addr(arg);
			}
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 获取函数地址
			 *
			 * @return 函数地址
			 */
			/* ----------------------------------------------------------------------------*/
			void* _getAddr(){ return _addr; }
		};

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 产生函数变量
		 *
		 * @param addr 函数地址
		 *
		 * @return 变量
		 */
		/* ----------------------------------------------------------------------------*/
		inline value* makeFunc(value* (*addr)(value*)){
			value* ret = makeValue<function>();
			ret->get<function>()->init(addr);
			return ret;
		}
	}
}
