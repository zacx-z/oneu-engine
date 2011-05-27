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
 * @file Event.h
 * @brief 事件系统
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
namespace OneU
{
	class IEventDispatcher;
	class EventArgs{};
	namespace event
	{
		//无参数事件
		//广播事件
		//广播对象接收到的事件
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 进入一帧的时候被广播
		 *
		 * 在场景更新之前。
		 */
		/* ----------------------------------------------------------------------------*/
		extern const wchar* ENTER_FRAME;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 窗口被激活时被广播
		 */
		/* ----------------------------------------------------------------------------*/
		extern const wchar* WINDOW_ACTIVE;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 窗口失去焦点时被广播
		 */
		/* ----------------------------------------------------------------------------*/
		extern const wchar* WINDOW_DEACTIVE;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 游戏对象被销毁时被广播
		 *
		 * 在场景销毁之后，各种系统销毁之前。
		 * @sa section_run
		 */
		/* ----------------------------------------------------------------------------*/
		extern const wchar* DESTROY;
	}
}
#include "Table.h"
#include "List.h"

namespace OneU
{

	/* ----------------------------------------------------------------------------*/
	/** 
	* @brief 事件监听器
	*
	* 继承该接口以实现一个事件监听器。
	*/
	/* ----------------------------------------------------------------------------*/
	class IEventListener
		: public Interface
	{
	public:
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 触发事件
		* 
		* @param event 事件名称
		* @param pArg 事件参数，可为NULL。
		* @param source 被触发的事件调度器
		* @sa @ref event
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void onEvent(pcwstr event, EventArgs* pArg, IEventDispatcher* source) = 0;
	};

	/*
	template<class T>
	class ISEventListener{
	public:
		virtual void onEvent(T& event) = 0;
	};
	template<class T>
	class ISEventReceiver
		: public Interface
	{
	private:
		typedef InterfacePtr<ISEventListener<T>> HandlerHolder;
		typedef List<HandlerHolder> EventList_t;
		EventList_t m_event_list;
	public:
		virtual void addListener(IEventListener* listener){
			m_event_list.pushBack(listener);
		}
		virtual void trigger(T& event){
			for(EventList_t::iterator iter = m_event_list.begin(); iter != m_event_list.end(); ++iter){
				(*iter)->onEvent(event);
			}
		}
	};*/

	/* ----------------------------------------------------------------------------*/
	/** 
	* @brief 事件调度器
	* 
	* 继承该接口可以使类可处理事件
	*/
	/* ----------------------------------------------------------------------------*/
	class IEventDispatcher
		: public Interface
	{
	private:
		typedef InterfacePtr<IEventListener> HandlerHolder;
		typedef List<HandlerHolder> EventList_t;
		Table<EventList_t> m_event_table;
	public:
		virtual ~IEventDispatcher(){}

		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 添加事件监听器
		* 
		*
		* @param event 事件名称
		* @param eventhandler 事件监听器
		*
		* @remarks 监听器的生存期会被此类管理。
		* @sa @ref event
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void addEventListener(pcwstr event, IEventListener* eventhandler){
			m_event_table[event].pushBack(eventhandler);
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 发送事件 
		* 
		* @param event 事件名称
		* @param pArgs 事件参数，可为NULL。
		*
		* @remarks 监听器会立即被触发。（在函数体内执行）
		* @sa @ref event
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void sendEvent(pcwstr event, EventArgs* pArgs = NULL){
			EventList_t* l = m_event_table.find(event);
			if(l != NULL){
				for(EventList_t::iterator iter = l->begin(); iter != l->end(); ++iter){
					(*iter)->onEvent(event, pArgs, this);
				}
			}
		}
	};
}

namespace OneU
{
	class CharEvent
		: public EventArgs
	{
		wchar charCode;
	public:
		CharEvent(wchar code)
			: charCode(code){}
		wchar getCode(){ return charCode;}
	};
	class KeyEvent
	{
		dword scancode;
		bool bRelease;//若为true是Release，否则为Press
	public:
		KeyEvent(dword scancode, bool bRelease)
			: scancode(scancode), bRelease(bRelease){}
		dword getCode(){ return scancode;}
		bool isRelease(){ return bRelease; }
		bool isPress(){ return !bRelease; }
	};
	class IInputReceiver
		: public Interface
	{
		bool m_bWillPropagate;
	public:
		void propagate(bool bPropagate){//reserved
			m_bWillPropagate = bPropagate;
		};
		bool willPropagate(){ return m_bWillPropagate; }
		virtual void onChar(CharEvent& event){}
		virtual void onKey(KeyEvent& event){}
	};
}