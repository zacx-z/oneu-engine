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
 * @file AtEnv.h
 * @brief ATOM环境值
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "Table.h"
#include "AtValue.h"

namespace OneU
{
	/**
	 * @addtogroup group_atom
	 */
	//@{
	namespace atom
	{
		class value;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 环境类
		 * @sa makeEnv
		 */
		/* ----------------------------------------------------------------------------*/
		class env
		{
			env* _father;
			Table<value*> _table;

			env(const env&);
			env& operator=(const env&);

			friend class value;
			friend class klass;
			env()
				: _father(NULL){}
		public:
			~env(){
				for(Table<value*>::iterator it = _table.begin(); it != _table.end(); ++it){
					it.getValue()->unacquire();
				}
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置父表
			 * 
			 * @param father 父表指针
			 */
			/* ----------------------------------------------------------------------------*/
			void setFather(env* father){
				_father = father;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 创建符号
			 * 
			 * 在表中创建符号，设为变量v。如果已存在键位symbol的符号，则会创建失败。
			 *
			 * @param symbol 符号
			 * @param v 变量
			 * 
			 * @returns 表示创建符号成功与否
			 * @retval true 成功
			 * @retval false 失败
			 * @remarks 会向v获取控制权
			 */
			/* ----------------------------------------------------------------------------*/
			bool createSymbol(pcwstr symbol, value* v = NULL){
				if(v == NULL) v = makeNil();
				if(!v->acquire()) return false;
				value** p;
				if(!(p =_table.insert(symbol))){
					v->unacquire();
					return false;
				}
				*p = v;
				return true;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 删除符号
			 *
			 * 在表中删除符号。如果表中不存在该符号，则返回NULL。
			 * 
			 * @param symbol 符号
			 * 
			 * @returns 所删除符号对应的变量
			 * @remarks 会释放变量的控制权
			 */
			/* ----------------------------------------------------------------------------*/
			value* removeSymbol(pcwstr symbol){
				value** p = _table.find(symbol);
				if(!p) return NULL;

				value* ret = *p;
				_table.erase(_table.getLastItem());
				ret->unacquire();
				return ret;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取符号对应的变量
			 * 
			 * @param symbol 符号
			 * 
			 * @returns 变量
			 */
			/* ----------------------------------------------------------------------------*/
			value* getSymbol(pcwstr symbol){
				value** p = _table.find(symbol);
				if(!p){
					if(!_father) return NULL;
					return _father->getSymbol(symbol);
				}
				return *p;
			}

			template<class T>
			void traversal(T& functor){//functor(pcwstr, value*)
				for(Table<value*>::iterator it = _table.begin(); it != _table.end(); ++it){
					functor(it._getKey().c_str(), it.getValue());
				}
			}
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 创建环境表
		 * 
		 * @returns 环境表
		 */
		/* ----------------------------------------------------------------------------*/
		inline value* makeEnv(){ return makeValue<env>(); }
	}
	//@}
}
