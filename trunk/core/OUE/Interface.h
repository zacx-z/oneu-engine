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

		template<class T>
		friend class Handle;
		struct _HandleInfo
		{
			int ref;
			bool isvalid;
			Interface* pI;
		} *_hinfo;
	public:
		virtual ~Interface(){
			if(_hinfo){
				_hinfo->isvalid = false;
				if(_hinfo->ref == 0)
					ONEU_DELETE_T(_hinfo);
			}
		}
	public:
		Interface() : _hinfo(NULL){}
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

	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 句柄
	 *
	 * @remarks该句柄类是用于解决外部引用安全性的问题的。
	 * 针对于Hold-a一个引用，并不会管理对象生存期，不会尝试销毁对象。仅能用于Interface。
	 */
	/* ----------------------------------------------------------------------------*/
	//动机是来自于脚本中有些变量被回收掉（但在主程序中持有引用）引发程序崩溃而无法追踪错误的问题。
	template<class T>
	class Handle
	{
		Interface::_HandleInfo* m_hInfo;

		static Interface::_HandleInfo* _getInfo(T* v){
			if(!v->_hinfo){
				v->_hinfo = ONEU_NEW_T(Interface::_HandleInfo);
				v->_hinfo->ref = 0;
				v->_hinfo->isvalid = true;
				v->_hinfo->pI = v;
			}
			return v->_hinfo;

		}
		void _releaseInfo(){
			if(!m_hInfo) return;
			if(--m_hInfo->ref == 0 && !m_hInfo->isvalid)//此时接口对象已删除
				ONEU_DELETE_T(m_hInfo);
		}

	public:
		Handle(T* v){
			if(!v) {m_hInfo = NULL; return;}

			m_hInfo = _getInfo(v);
			++m_hInfo->ref;
		}
		Handle(const Handle& rhs) : m_hInfo(rhs.m_hInfo){
			if(m_hInfo) ++m_hInfo->ref;
		}
		Handle& operator=(const Handle<T>& rhs){
			if(m_hInfo == rhs.m_hInfo) return;
			_releaseInfo();

			m_hInfo = rhs.m_hInfo;
			if(m_hInfo) ++m_hInfo->ref;

			return *this;
		}
		Handle& operator=(T* v){
			_releaseInfo();
			if(!v){ m_hInfo = NULL; return *this;}
			m_hInfo = _getInfo(v);
			++m_hInfo->ref;

			return *this;
		}
		~Handle(){
			_releaseInfo();
		}

		bool isValid(){
			return (!m_hInfo) || (m_hInfo && m_hInfo->isvalid);
		}

		T* get(){
			if(!isValid()) ONEU_RAISE(L"句柄已失效！");
			return m_hInfo ? (T*)m_hInfo->pI : NULL;
		}
		const T* get() const{
			if(!isValid()) ONEU_RAISE(L"句柄已失效！");
			return m_hInfo ? (const T*)m_hInfo->pI : NULL;
		}

		T* operator->(){ return this->get(); }
		const T* operator->() const{ return this->get;}
		T& operator*(){ return *this->get(); }
		const T& operator*() const{ return *this->get(); }

		T* get(const char* srcname, int line, const char* function){
			if(!isValid()) ONEU_RAISE(L"句柄已失效！\n%s(%d)\nFunction:%s", srcname, line, function);
			return m_hInfo ? (T*)m_hInfo->pI : NULL;
		}
		const T* get(const char* srcname, int line, const char* function) const{
			if(!isValid()) ONEU_RAISE(L"句柄已失效！\n%s(%d)\nFunction:%s", srcname, line, function);
			return m_hInfo ? (const T*)m_hInfo->pI : NULL;
		}
	};
#define SAFE_H(handle) (handle.get(__FILE__, __LINE__, __FUNCTION__))

	//Utilities
	//主要用于容器 保证被正确删除
	//不支持InterfaceRef
	template<class T>
	class InterfacePtr
	{
		//这里使用Interface而不是T主要是保证T是Interface的派生类
	private:
		mutable Handle<Interface> m_h;
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
			if(m_h.isValid()) ONEU_DELETE m_h.get();
		}
		InterfacePtr& operator=(const InterfacePtr& rhs){
			m_h = rhs.m_h;
			rhs.m_h = NULL;
		}
		T* operator->(){return (T*)SAFE_H(m_h); }
		T& operator*(){return *(T*)SAFE_H(m_h);}
	};

	//Factory
	template<class klass>
	struct Factory
	{
		typedef klass* (*type)();
	};
}
