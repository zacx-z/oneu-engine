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
 * @file Game.h
 * @brief 游戏主系统
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
#include "Stack.h"
#include "Event.h"
#include "Scene.h"

namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @defgroup group_game 游戏主模块
	 */
	/* ----------------------------------------------------------------------------*/
	//@{
	class IGame;
	class IVideo;
	class IAudio;
	class IControl;
	class IScene;
	class IBroadcast;
	class IInputReceiver;

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取游戏对象
	 * 
	 * @returns 游戏对象接口
	 * @sa Game_create, Game_build, Game_destroy
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API IGame& GetGame();//interface

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 游戏接口
	 */
	/* ----------------------------------------------------------------------------*/
	class IGame
		: public Interface
	{
	protected:
		IBroadcast* m_pBroadcast;
		IVideo* m_pVideo;
		IAudio* m_pAudio;
		IControl* m_pControl;
		IScene* m_pScene;
	public:
		IGame()
			: m_pBroadcast(NULL), m_pVideo(NULL), m_pAudio(NULL), m_pControl(NULL), m_pScene(NULL)
		{}
		~IGame(){
			ONEU_ASSERT(m_pScene == NULL);
			ONEU_ASSERT(m_pControl == NULL);
			ONEU_ASSERT(m_pAudio == NULL);
			ONEU_ASSERT(m_pVideo == NULL);
			ONEU_ASSERT(m_pBroadcast == NULL);
		}

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取广播对象
		 * 
		 * @returns 广播对象
		 */
		/* ----------------------------------------------------------------------------*/
		virtual IBroadcast& getBroadcast(){ return *m_pBroadcast; }
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取图形系统对象
		 * 
		 * @returns 图形系统对象
		 */
		/* ----------------------------------------------------------------------------*/
		virtual IVideo& getVideo(){ return *m_pVideo; }
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取音响系统对象
		 * 
		 * @returns 音响系统对象
		 */
		/* ----------------------------------------------------------------------------*/
		virtual IAudio& getAudio(){ return *m_pAudio;}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取控制系统对象
		 * 
		 * @returns 控制系统对象
		 */
		/* ----------------------------------------------------------------------------*/
		virtual IControl& getControl(){ return *m_pControl;}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取活动场景对象
		 * 
		 * @returns 活动场景对象
		 */
		/* ----------------------------------------------------------------------------*/
		virtual IScene& getScene(){ return *m_pScene; }

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 替换场景
		 *
		 * 将当前活动场景替换成新场景。
		 * 
		 * @param newscene 新场景地址
		 * 
		 * @returns 当前活动场景地址。
		 *
		 * @remarks 活动场景会自动在游戏对象销毁时被删除。
		 */
		/* ----------------------------------------------------------------------------*/
		//替换掉场景栈栈顶场景 返回旧场景
		virtual IScene* replaceScene(IScene* newscene) = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 初始化游戏
		 * 
		 * @param WindowName 窗口的名称
		 * @param width 窗口宽度
		 * @param height 窗口高度
		 * @param bWindowed 是否窗口化
		 * 
		 * @returns 表示是否初始化成功
		 * @retval true 初始化成功
		 * @retval false 初始化失败
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void init(pcwstr WindowName, uint32 width, uint32 height, bool bWindowed) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 运行游戏
		 *
		 * 进入游戏主循环。函数退出时Game对象会自动销毁。
		 * 
		 * @returns 程序返回值。一般将该返回值作为程序返回值返回。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual int run() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 退出函数
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void quit() = 0;

		//控制
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 处理每一帧
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void onFrame() = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 在窗口得到或失去焦点时执行
		 *
		 * @param bActive 表示是得到还是失去焦点。为TRUE是得到焦点；为FALSE是失去焦点。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void onActiveWindow(bool bActive) = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 设置窗口名称
		 * 
		 * @param title 窗口名称
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void setWindowTitle(pcwstr title) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取窗口位置
		 * 
		 * @return 位置
		 */
		/* ----------------------------------------------------------------------------*/
		virtual vector2i_t getWindowPos() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 设置窗口位置
		 * 
		 * @param newPos 新位置
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void setWindowPos(const vector2i_t& newPos) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 设置鼠标显示状态
		* 
		* @param bShow 为TRUE鼠标显示；为FALSE鼠标隐藏。
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void showCursor(bool bShow) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取当前FPS
		 * 
		 * @returns FPS
		 */
		/* ----------------------------------------------------------------------------*/
		virtual float getFPS() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取当前帧与前一帧的时间间隔
		 * 
		 * @returns 时间间隔，以秒计算。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual float getTimeInterval() = 0;

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 传输字符事件。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void onChar(const CharEvent& event){
			if(m_pScene) m_pScene->onChar(event);
		}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 传输键盘按键事件。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void onKey(const KeyEvent& event){
			if(m_pScene) m_pScene->onKey(event);
		}
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 传输鼠标按键事件。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void onMouse(const MouseEvent& event){
			if(m_pScene) m_pScene->onMouse(event);
		}

		//tools
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 运行Shell指令
		 *
		 * @param command 指令字符串
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void runShell(pcwstr command) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 向输出流信息
		 *
		 * 输出流默认为游戏内置Shell。
		 *
		 * @param data 信息字符串
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void output(pcwstr data) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 游戏输出流
		 *
		 * 调用output来输出。
		 */
		/* ----------------------------------------------------------------------------*/
		class Stream
			: public IOStreamMaker
		{
			IGame* m_pTarget;
		public:
			Stream(IGame* pTarget)
				: m_pTarget(pTarget){}
			void output(pcwstr data){
				m_pTarget->output(data);
			}
		};
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 获取游戏输出流
		 *
		 * 调用output来输出。
		 *
		 * @return 输出流
		 * @sa output
		 */
		/* ----------------------------------------------------------------------------*/
		virtual Stream stream(){
			return Stream(this);
		}

		//不太重要的
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 向控制台输出游戏相关信息
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void showInfo() = 0;
	};

	ONEU_API void Aux_GameInit(pcwstr WindowName, uint32 width, uint32 height, bool bWindowed);

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 创建游戏单态
	 * 
	 * @sa Game_destroy, GetGame
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API void Game_build();
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 销毁游戏单态
	 * 
	 * @sa Game_build, GetGame
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API void Game_destroy();

	
	//@}

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取广播对象
	 * 
	 * @returns 广播对象
	 */
	/* ----------------------------------------------------------------------------*/
	inline IBroadcast& GetBroadcast(){ return GetGame().getBroadcast(); }
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取图形系统对象
	 * 
	 * @returns 图形系统对象
	 */
	/* ----------------------------------------------------------------------------*/
	inline IVideo& GetVideo(){ return GetGame().getVideo(); }
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取音响系统对象
	 * 
	 * @returns 音响系统对象
	 */
	/* ----------------------------------------------------------------------------*/
	inline IAudio& GetAudio(){ return GetGame().getAudio(); }
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取控制系统对象
	 * 
	 * @returns 控制系统对象
	 */
	/* ----------------------------------------------------------------------------*/
	inline IControl& GetControl(){ return GetGame().getControl(); }
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取活动场景对象
	 * 
	 * @returns 活动场景对象
	 */
	/* ----------------------------------------------------------------------------*/
	inline IScene& GetScene(){ return GetGame().getScene(); }
}
