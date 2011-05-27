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
 * @file AtValue.h
 * @brief ATOM值
 *
 * ATOM的核心。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
#include "Atom.h"
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
		 * @brief 基本变量类型
		 */
		/* ----------------------------------------------------------------------------*/
		enum AT_TYPE{
			T_NIL = 0,
			T_BOOL,
			T_INT,
			T_FLOAT,
			T_STR,
			T_CPOINTER,
			T_TUPLE,
			T_FUNC,
			T_ENV,
			T_KLS,
			T_OBJ,
			T_OBJFUNC,
			T_REF = 0x80000000
			//T_TYPEDEF/*定义一个类型的符号*/,//???
			//T_USERTYPE/*保留符号，比该符号值大的均为用户定义的类型*///
		};
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取类型名称
		 * 
		 * @param t 表示类型的常量
		 * 
		 * @returns 类型名称字符串，以NULL结束。
		 */
		/* ----------------------------------------------------------------------------*/
		inline pcwstr typeName(dword t){
			switch(t)
			{
			case T_NIL:
				return L"none";
			case T_BOOL:
				return L"bool";
			case T_INT:
				return L"int";
			case T_FLOAT:
				return L"float";
			case T_STR:
				return L"string";
			case T_CPOINTER:
				return L"cpointer";
			case T_TUPLE:
				return L"tuple";
			case T_FUNC:
				return L"function";
			case T_ENV:
				return L"environment";
			case T_KLS:
				return L"klass";
			case T_OBJ:
				return L"object";
			case T_OBJFUNC:
				return L"object function";
			default:
				return L"<Error!>";
			}
			return NULL;
		}

		/*@cond INTERNAL_MPL*/
		template<typename T>
		struct _atom_traits{
			//enum{ type_id = ATT_NIL };
		};
		template<>
		struct _atom_traits<value*>{
			enum{ type_id = T_REF };
		};
		template<>
		struct _atom_traits<bool>{
			enum{ type_id = T_BOOL };
		};
		template<>
		struct _atom_traits<int>{
			enum{ type_id = T_INT };
		};
		template<>
		struct _atom_traits<float>{
			enum{ type_id = T_FLOAT };
		};
		template<>
		struct _atom_traits<String>{
			enum{ type_id = T_STR };
		};
		template<>
		struct _atom_traits<void*>{
			enum{ type_id = T_CPOINTER };
		};
		template<>
		struct _atom_traits<tuple>{
			enum{ type_id = T_TUPLE };
		};
		template<>
		struct _atom_traits<function>{
			enum{ type_id = T_FUNC };
		};
		template<>
		struct _atom_traits<env>{
			enum{ type_id = T_ENV };
		};
		template<>
		struct _atom_traits<klass>{
			enum{ type_id = T_KLS };
		};
		template<>
		struct _atom_traits<obj>{
			enum{ type_id = T_OBJ };
		};
		template<>
		struct _atom_traits<obj_function>{
			enum{ type_id = T_OBJFUNC };
		};
		/*@endcond*///INTERNAL_MPL
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief Atom变量类
		 */
		/* ----------------------------------------------------------------------------*/
		class value
		{
			//_type_id为T_REF时，该变量是另一个变量的引用。
			//_type_id最高位为1（T_REF即为最高位为1其它均为0）且其它位存在非零值时，去掉最高位即为该变量的类型。该变量对于所指内容不管理内存。（也是一种引用形式）
			//貌似设计得有点复杂了，第一代先这样吧。
			//如果想去掉该功能：删除makeValueFromPtr；将T_REF值定义得正常；去掉_type_id & ~T_REF后半部分；删除evalPtr；去掉_destroy_value前面的特殊判断；然后重新编译dll。
			dword _type_id;
			void* _ptr;

			//一下两个变量是GC相关的
			List<value*>::iterator gcID;
			bool b_hold;

			template<typename T>
			void _set_value(const T& v, dword type){
				if(_type_id != type){
					_destroy_value();
					_type_id = type;
					_ptr = ONEU_NEW_T(T(v));
				}
				else
					*((T*)_ptr) = v;
			}
			template<typename T>
			void _create_value(dword type){
				if(_type_id != type){
					_destroy_value();
					_ptr = ONEU_NEW_T(T);
					_type_id = type;
				}
			}
			inline void _destroy_value();

			friend class IAtom;

			value()
				:_type_id(T_NIL), _ptr(NULL), b_hold(false){
			}
			value(const value&);
			value& operator=(const value&);
		public:
			~value(){
				_destroy_value();
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取类型
			 * 
			 * @returns 类型
			 * @remarks 如果变量为引用则会返回其所引用的值的类型
			 */
			/* ----------------------------------------------------------------------------*/
			dword type() const {
				if(_type_id == T_REF) return (*(value**)_ptr)->type();
				else return _type_id & ~T_REF;
			}
			//copy v to self
			//仅允许用C++内置类型
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 赋值
			 * 
			 * @param v 值，只允许是C内置类型。
			 * 
			 * @returns 此变量的引用
			 */
			/* ----------------------------------------------------------------------------*/
			template<typename T>
			value& eval(const T& v){
				_set_value(v, _atom_traits<T>::type_id);
				return *this;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 以T类型初始化
			 * @tparam T 只允许是C内置类型。
			 * 
			 * @returns 此变量的引用
			 */
			/* ----------------------------------------------------------------------------*/
			template<typename T>
			value& eval(){
				_create_value<T>(_atom_traits<T>::type_id);
				return *this;
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			* @brief 以外部内存初始化
			* @param v 值，只允许是C内置类型。
			* 
			* @returns 此变量的引用
			*/
			/* ----------------------------------------------------------------------------*/
			template<typename T>
			value& evalPtr(T* v){
				dword type = _atom_traits<T>::type_id;
				_destroy_value();
				_type_id = type | T_REF;
				_ptr = v;
				return *this;
			}
			//自定义类型
			//template<typename T>
			//value& eval(const T& v, dword typeID){
			//	_set_value(v, typeID);
			//	return *this;
			//}
			//template<typename T>
			//value& eval(dword typeID){
			//	_create_value<T>(typeID);
			//	return *this;
			//}//

			//变成nil
			value& evalNil(){
				_destroy_value();
				_type_id = T_NIL;
				_ptr = NULL;
				return *this;
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取内容
			 * @tparam T 所要获取的类型
			 * 
			 * @returns 如果该变量的内容为此类型的或为此类型的引用，则返回内容的地址。否则返回NULL。
			 * @remarks 如果T为value*，则会返回其所引用的变量。
			 */
			/* ----------------------------------------------------------------------------*/
			template<typename T>
			T* get(){
				if(_atom_traits<T>::type_id == (_type_id & ~T_REF))
					return (T*)_ptr;
				else if(_type_id == T_REF)
					return (*(value**)_ptr)->get<T>();
				else
					return NULL;
			}
			template<typename T>
			const T* get() const{
				if(_atom_traits<T>::type_id == (_type_id & ~T_REF))
					return (T*)_ptr;
				else if(_type_id == T_REF)
					return (*(value**)_ptr)->get<T>();
				else
					return NULL;
			}


			//eval的特化
			template<>
			value& eval<pcwstr>(const pcwstr& v){
				//字符串
				eval<String>();
				(*(String*)_ptr) = v;
				return *this;
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取控制权
			 * 
			 * @returns 表示获取控制权成功或否
			 * @retval true 获取控制权成功
			 * @retval false 获取控制权失败
			 * @sa unacquire
			 */
			/* ----------------------------------------------------------------------------*/
			bool acquire(){ return GetAtom().acquireValue(this);}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 释放控制权
			 *
			 * 如果没有被获得控制权，不做任何操作。
			 */
			/* ----------------------------------------------------------------------------*/
			void unacquire(){ GetAtom().unacquireValue(this);}
			/* ----------------------------------------------------------------------------*/
			/** 
			* @brief 是否为引用
			*
			* @return 表示该变量是否为引用
			*/
			/* ----------------------------------------------------------------------------*/
			bool isRef(){ return _type_id == T_REF; }
		};

		void value::_destroy_value(){
			if((_type_id & T_REF) != 0 && (_type_id & ~T_REF) != 0)
				return;
			if(_ptr != NULL){
				switch(_type_id){
				case T_REF:
				case T_BOOL:
				case T_INT:
				case T_FLOAT:
				case T_CPOINTER:
					ONEU_DEALLOC(_ptr);
					break;
				case T_STR:
					ONEU_DELETE_T((String*)_ptr);
					break;
				case T_TUPLE:
					ONEU_DELETE_T((tuple*)_ptr);
					break;
				case T_FUNC:
					ONEU_DELETE_T((function*)_ptr);
					break;
				case T_ENV:
					ONEU_DELETE_T((env*)_ptr);
					break;
				case T_KLS:
					ONEU_DELETE_T((klass*)_ptr);
					break;
				case T_OBJ:
					ONEU_DELETE_T((obj*)_ptr);
					break;
				case T_OBJFUNC:
					ONEU_DELETE_T((obj_function*)_ptr);
					break;
				default:
					ONEU_ENSURE(0);
				}
			}
		}

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 创建C内置类型变量
		 * @tparam T 类型，限制为C内置类型
		 * @returns 变量
		 */
		/* ----------------------------------------------------------------------------*/
		template<typename T>
		inline value* makeValue(){ return &GetAtom()._make_value()->eval<T>(); }
		template<typename T>
		inline value* makeValue(const T&v){ return &GetAtom()._make_value()->eval(v); }
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 创建空变量
		 * @returns 变量
		 */
		/* ----------------------------------------------------------------------------*/
		inline value* makeNil(){ return GetAtom()._make_value(); }
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 创建引用
		 * @param v 被引用的变量
		 * @return 引用v的变量
		 * @attention 在目前的版本中，必须保证被引用的变量在该引用的生存期中一直存在。
		 */
		/* ----------------------------------------------------------------------------*/
		inline value* makeRef(value* v){ return makeValue(v); }

		/* ----------------------------------------------------------------------------*/
		/**
		* @brief 创建外部内存变量
		* @param v 被引用的内存
		* @return 引用外部内存v的变量
		* @remarks 该函数提供一个创建一个使用外部内存的变量的方法。变量销毁后变量所用的值（v所指的内存）不会被销毁，其生存期独立于变量。
		*/
		/* ----------------------------------------------------------------------------*/
		template<class T>
		inline value* makeValueFromPtr(T* v){ return &GetAtom()._make_value()->evalPtr(v); }
	}

	/*@cond INTERNAL*/
	atom::value* IAtom::_make_value(){ atom::value* ret = ONEU_NEW_T(atom::value); unacquireValue(ret); return ret;}
	/*@endcond*/

	List<atom::value*>::iterator& IAtom::__value_gcID(atom::value* v){ return v->gcID;}
	bool& IAtom::__value_b_hold(atom::value* v){ return v->b_hold; }

	//@}
}

#include "AtTuple.h"
#include "AtEnv.h"
#include "AtFunc.h"
#include "AtKlass.h"
