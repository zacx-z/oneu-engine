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
 * @file Interface.h
 * @brief 接口类
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
//level 2
#include "OneUPreDef.h"
namespace OneU
{
	//Interface must derive from this class
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 接口
	 */
	/* ----------------------------------------------------------------------------*/
	class Interface
		: public AllocatedObject
	{
		Interface(const Interface&);
		Interface& operator=(const Interface&);
	public:
		virtual ~Interface(){}
	public:
		Interface(){}
	};

	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 引用接口
	 *
	 * 具有引用计数功能的接口。
	 */
	/* ----------------------------------------------------------------------------*/
	class InterfaceRef
		: public Interface
	{
		uint32 m_ref;
	public:
		InterfaceRef() : m_ref(0){}
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 添加引用计数
		* 
		* @returns 新的引用计数
		*/
		/* ----------------------------------------------------------------------------*/
		uint32 addRef(){
			return ++m_ref;
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 释放引用计数
		*
		* 如果引用计数释放后为0，则销毁对象。
		* 
		* @returns 新的引用计数
		*/
		/* ----------------------------------------------------------------------------*/
		uint32 release(){
			--m_ref;
			if(m_ref == 0){
				ONEU_DELETE this;
				return 0;
			}
			return m_ref;
		}
	};

	//主要用于容器 保证被正确删除
	//@todo 目前不支持InterfaceRef
	template<class T>
	class InterfacePtr
	{
		//这里使用Interface而不是T主要是保证T是Interface的派生类
	private:
		mutable Interface* m_h;
	public:
		InterfacePtr(T* h)
			: m_h(h)
		{}
		InterfacePtr(const InterfacePtr& rhs)
			: m_h(rhs.m_h)
		{
			rhs.m_h = NULL;
		}
		~InterfacePtr(){
			if(m_h != NULL) ONEU_DELETE m_h;
		}
		InterfacePtr& operator=(const InterfacePtr& rhs){
			m_h = rhs.m_h;
			rhs.m_h = NULL;
		}
		T* operator->(){return (T*)m_h; }
		T& operator*(){return *(T*)m_h;}
	};

	//Factory
	template<class klass>
	struct Factory
	{
		typedef klass* (*type)();
	};
}
