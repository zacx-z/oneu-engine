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
 * @file EventDispatcher.h
 * @brief 事件派发器
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
#include "List.h"
#include "Table.h"

//(事件定义在event.h中）

namespace OneU
{
	class IEventDispatcher;
	class EventArgs{};

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
		virtual void onTrigger(pcwstr event, EventArgs* pArg, IEventDispatcher* source) = 0;
	};

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
					(*iter)->onTrigger(event, pArgs, this);
				}
			}
		}
	};
}
