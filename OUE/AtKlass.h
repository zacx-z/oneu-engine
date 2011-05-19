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
 * @file AtKlass.h
 * @brief ATOM类值
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "AtEnv.h"
#include "AtFunc.h"

namespace OneU{
	/**
	 * @addtogroup group_atom
	 */
	//@{
	namespace atom
	{
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 对象
		 * @sa makeObj
		 */
		/* ----------------------------------------------------------------------------*/
		class obj
		{
			klass* _klass;
			void (*_destroyer)(void*);

			friend class value;
			friend class obj_function;
			friend value* makeObj(klass*, value*);

			obj()
				: _pObj(NULL){}
			inline void init(klass* kls, value* arg);//should be called immediately after contructed

			void* _pObj;

			obj(const obj&);
			obj& operator=(const obj&);
		public:
			inline ~obj();
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取对象所属类
			 * 
			 * @returns 类地址
			 */
			/* ----------------------------------------------------------------------------*/
			klass* getKlass(){ return _klass;}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 调用object function
			 * 
			 * @param func 函数地址
			 * @param arg 传入参数，若参数为NULL或非Tuple会自动转化为Tuple。
			 * 
			 * @returns 函数返回的值（不加任何修饰）
			 */
			/* ----------------------------------------------------------------------------*/
			inline value* call(obj_function* func, value* arg);
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取内部对象的地址
			 * 
			 * @returns 对象地址
			 * @remarks 所谓对象地址就是@ref makeKlass中（重载版本一）模板参数的类的对象地址、（重载版本二）创建函数返回的地址。
			 * @sa @ref section_export_cooperation
			 */
			/* ----------------------------------------------------------------------------*/
			inline void* getAddr(){ return _pObj; }
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 类
		 * @sa makeKlass
		 */
		/* ----------------------------------------------------------------------------*/
		class klass
			: public env
		{
		public:
			typedef void* (*creator_t)(value*);
			typedef void (*destroyer_t)(void*);
		private:
			friend class obj;
			friend class value;
			creator_t _creator;
			destroyer_t _destroyer;

			klass()
				: _creator(NULL), _destroyer(NULL)
			{}
			void init(creator_t c, destroyer_t d){//should be called immediately after contructed
				_creator = c; _destroyer = d;
			}
			friend value* makeKlass_(creator_t c, destroyer_t d);
		public:
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief object function
		 * @sa makeObjFunc
		 */
		/* ----------------------------------------------------------------------------*/
		class obj_function
		{
		public:
			typedef value* (exported::*addr_t)(value*);
		private:
			friend class value;
			template<class T>
			friend value* makeObjFunc(klass* kls, value* (T::*ptr)(value*));

			klass* _klass;

			addr_t _addr;

			obj_function(){}
			void init(klass* kls, addr_t ptr){
				_klass = kls;
				_addr = (addr_t)ptr;
			}
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取所属类
			 * 
			 * @returns 类地址
			 */
			/* ----------------------------------------------------------------------------*/
			klass* getKlass(){ return _klass;}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 调用函数
			 * 
			 * @param o 被调用的对象
			 * @param arg 传入参数，若参数为NULL或非Tuple会自动转化为Tuple。
			 * 
			 * @returns 函数返回的值（不加任何修饰）
			 */
			/* ----------------------------------------------------------------------------*/
			value* call(obj* o, value* arg){//如果arg为NULL，接收者会得到一个空的Tuple。如果arg不为tuple接受者会得到只有一个元素的tuple。
				ASSERT(o->getKlass() == this->getKlass());
				arg = _convert_to_tuple(arg);
				exported* _obj_ = (exported*)(o->_pObj);
				return (_obj_->*_addr)(arg);
			}
		};


		inline void obj::init(klass* kls, value* arg){
			_klass = kls;
			_pObj = _klass->_creator(arg);
			_destroyer = _klass->_destroyer;//避免obj析构时klass已经析构（这样子避免了klass和obj在同一张表里当表析构时klass先于obj析构的问题）
		}
		inline obj::~obj(){
			if(_pObj != NULL)//有可能在对象还没构造完就终止程序，此时_pObj为NULL
				_destroyer(_pObj);
		}
		inline value* obj::call(obj_function* func, value* arg){
			return func->call(this, arg);
		}

		//前置声明
		template<class T>
		T* __klass_creator(value* arg){
			return ONEU_NEW T(arg);
		}
		template<class T>
		void __klass_destroyer(T* p){
			ONEU_DELETE p;
		}

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 创建类
		 *
		 * @tparam T 被创建的类，应当派生自atom::exported。
		 * 
		 * @returns 类变量
		 */
		/* ----------------------------------------------------------------------------*/
		template<class T>
		inline value* makeKlass(){
			return makeKlass(__klass_creator<T>, __klass_destroyer<T>);
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 从工厂函数创建类
		 * 
		 * @param c 工厂函数
		 * @param d 销毁函数
		 * 
		 * @returns 类变量
		 */
		/* ----------------------------------------------------------------------------*/
		template<class T>
		inline value* makeKlass(T* (*c)(value*), void (*d)(T*)){
			return makeKlass_((klass::creator_t)c, (klass::destroyer_t)d);
		}
		inline value* makeKlass_(void* (*c)(value*), void (*d)(void*)){
			value* ret = makeValue<klass>();
			ret->get<klass>()->init(c, d);
			return ret;
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 创建对象
		 * 
		 * @param kls 对象所属类
		 * @param arg 创建参数
		 * 
		 * @returns 对象变量
		 */
		/* ----------------------------------------------------------------------------*/
		inline value* makeObj(klass* kls, value* arg){
			arg = _convert_to_tuple(arg);
			value* ret = makeValue<obj>();
			ret->get<obj>()->init(kls, arg);
			return ret;
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 创建object function
		 * 
		 * @param kls 所属类
		 * @param ptr 成员函数指针。一定是派生自atom::exported的类的成员函数。
		 * 
		 * @returns object function变量
		 */
		/* ----------------------------------------------------------------------------*/
		template<class T>
		inline value* makeObjFunc(klass* kls, value* (T::*ptr)(value*)){
			T* __ = NULL;exported* ___ = __;//此语句检查你有没有把T从exported派生
			value* ret = makeValue<obj_function>();
			int memptr = *(int*)&ptr;
			ret->get<obj_function>()->init(kls,*(obj_function::addr_t*)(&memptr));
			return ret;
		}
	}
	//@}
}
