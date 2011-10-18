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

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 事件调度器
	 * 
	 * 继承该接口可以使类可处理事件。
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
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 字符事件
	 */
	/* ----------------------------------------------------------------------------*/
	class CharEvent
	{
		wchar charCode;
	public:
		CharEvent(wchar code)
			: charCode(code){}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 获取字符编码
		 * @returns 字符编码
		 */
		/* ----------------------------------------------------------------------------*/
		wchar getChar() const{ return charCode;}
	};
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 键盘按键事件
	 */
	/* ----------------------------------------------------------------------------*/
	class KeyEvent
	{
		uint32 scancode;
		bool bPress;//若为true是Press，否则为Release
	public:
		KeyEvent(uint32 scancode, bool bPress)
			: scancode(scancode), bPress(bPress){}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 获取按键扫描码
		 * @returns 扫描码
		 */
		/* ----------------------------------------------------------------------------*/
		uint32 getCode(){ return scancode;}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 判断是否是释放按键事件 
		 */
		/* ----------------------------------------------------------------------------*/
		bool isRelease(){ return !bPress; }
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 判断是否是按下按键事件
		 */
		/* ----------------------------------------------------------------------------*/
		bool isPress(){ return bPress; }
	};

	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 鼠标按键事件
	 */
	/* ----------------------------------------------------------------------------*/
	class MouseEvent
	{
		uint32 buttonID;
		bool bPress;//若为true是Release，否则为Press
	public:
		MouseEvent(uint32 buttonID, bool bPress)
			: buttonID(buttonID), bPress(bPress){}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 获取鼠标按键ID
		 * @returns ID 0表示左键，1表示右键，2表示中键。
		 */
		/* ----------------------------------------------------------------------------*/
		uint32 getButtonID(){ return buttonID;}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 判断是否是释放按键事件 
		 */
		/* ----------------------------------------------------------------------------*/
		bool isRelease(){ return !bPress; }
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 判断是否是按下按键事件
		 */
		/* ----------------------------------------------------------------------------*/
		bool isPress(){ return !bPress; }
	};

	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 输入事件接收器
	 *
	 * 接收输入事件的类。派生自该类可以接收输入事件。
	 * @remarks 需要调用@ref IScene::pushInputFocus()，将其注册到栈中才能接收到输入事件。
	 * @sa IGame::pushInputFocus()
	 */
	/* ----------------------------------------------------------------------------*/
	class IInputReceiver
		: public Interface
	{
	public:
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 字符事件响应
		 *
		 * @retval true 事件不向下传递
		 * @retval false 事件向下传递
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool onChar(const CharEvent& event){ return true; }
		/* ----------------------------------------------------------------------------*/
		/**
		* @brief 键盘按键事件响应
		* 
		* @retval true 事件不向下传递
		* @retval false 事件向下传递
		*/
		/* ----------------------------------------------------------------------------*/
		virtual bool onKey(const KeyEvent& event){ return true; }
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 鼠标按键事件响应
		 * 
		 * @retval true 事件不向下传递
		 * @retval false 事件向下传递
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool onMouse(const MouseEvent& event){ return true; }
	};
	/*@cond INTERNAL_MPL*/
	//用于责任链
	struct __CharFunctor
	{
		const CharEvent& m_event;
		__CharFunctor(const CharEvent& event) : m_event(event){}
		bool operator()(Handle<IInputReceiver> p){
			return SAFE_H(p)->onChar(m_event);
		}
	};
	struct __KeyFunctor
	{
		const KeyEvent& m_event;
		__KeyFunctor(const KeyEvent& event) : m_event(event){}
		bool operator()(Handle<IInputReceiver> p){
			return SAFE_H(p)->onKey(m_event);
		}
	};
	struct __MouseFunctor
	{
		const MouseEvent& m_event;
		__MouseFunctor(const MouseEvent& event) : m_event(event){}
		bool operator()(Handle<IInputReceiver> p){
			return SAFE_H(p)->onMouse(m_event);
		}
	};
	/*@endcond*///INTERNAL_MPL
}
