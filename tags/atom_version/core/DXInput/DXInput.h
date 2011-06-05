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
 * @file DXInput.h
 * @brief 输入系统头文件
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-02-10
 */
#pragma once
#include "../Base/Base.h"

//在定义了Base.h之后该头文件才会包含进来
#ifdef __ONEU_USE_DIRECTINPUT

#include "../Base/Internal/DXDefs.h"
#include "InputConst.h"//include <dinput.h>

//#ifdef ONEU_DLL
#ifdef ONEU_EXPORTS
#define ONEU_DXINPUT_API __declspec(dllexport)
#else
#define ONEU_DXINPUT_API __declspec(dllimport)
#endif
//#else
//#define ONEU_DXINPUT_API
//#endif

namespace OneU
{
	namespace DX
	{
		/*@cond INTERNAL_IMPL*/
		extern IDirectInput8 *_pDInput;
		/*@endcond*///INTERNAL_IMPL

		//前置声明
		class Input_cls;
		Input_cls * const GetInput();
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 全局唯一输入模块对象
		*/
		/* ----------------------------------------------------------------------------*/
		extern Input_cls& Input;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 输入系统
		 */
		/* ----------------------------------------------------------------------------*/
		class Input_cls
		{
		public:
			Input_cls()
			{
				ONEU_ASSERT( _pDInput == NULL );
			}
			~Input_cls()
			{
				Destroy();
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 初始化输入系统
			 */
			/* ----------------------------------------------------------------------------*/
			void Init(HINSTANCE hInstance, HWND hWnd);
			void Destroy()
			{
				SAFE_RELEASE( _pDInput );
			}
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 输入设备
		 */
		/* ----------------------------------------------------------------------------*/
		class InputDevice
		{
		protected:
			IDirectInputDevice8 *m_pDIDevice;
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 初始化
			 *
			 * 用于派生类覆盖，定制初始化动作
			 * 一般不需要客户调用。
			 * 
			 * @param BufferSize 缓冲区大小
			 */
			/* ----------------------------------------------------------------------------*/
			virtual void Init( uint32 BufferSize = 0 ) = 0;
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 数据模式
			 *
			 * 存储单个事件的结构体。应声明一个DataType数组来开辟一个缓冲区。
			 */
			/* ----------------------------------------------------------------------------*/
			typedef DIDEVICEOBJECTDATA DataType;
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 缓冲数据处理函数
			 * 
			 * @param dword 表示对应键位的ID
			 * @param dword 表示键位的状态
			 * @param dword 表示事件触发的时间
			 * @param dword 表示事件在一个事件序列中的位置
			 *
			 */
			/* ----------------------------------------------------------------------------*/
			typedef void ( *DataHandler )( uint32, uint32, uint32, uint32 );
			static const int DATA_BUFFER_SIZE = 128;
			InputDevice()
				: m_pDIDevice( NULL )
			{
				//让Input先构造
				GetInput();
			}
			~InputDevice()
			{
				Destroy();
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 创建设备
			 *
			 * 在内部调用了Init。
			 * 
			 * @param BufferSize 缓冲区大小，决定了Data的大小。DXSDK未说明以什么单位计量。
			 *
			 * 若为0则表示不使用缓冲模式。
			 * @remarks 如果BufferSize设定超过设备支持的大小，则会使用设备最多能支持的大小。
			 */
			/* ----------------------------------------------------------------------------*/
			void Create( uint32 BufferSize = 0 );
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取输入设备
			 *
			 * 在设定好了设备的所有参数执行，使用设备。
			 */
			/* ----------------------------------------------------------------------------*/
			void Acquire()
			{
				DXCHECK_THROW( m_pDIDevice->Acquire(), L"获取输入设备失败！" );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 轮询设备
			 *
			 * 只有游戏杆需要该操作，在获取立即模式数据之前应先轮询。
			 * 一般不需要客户使用。
			 */
			/* ----------------------------------------------------------------------------*/
			void Poll()
			{
				m_pDIDevice->Poll();
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 读取当前状态
			 *
			 * 立即模式操作方式
			 * 
			 * @param DataBuffer 缓冲区
			 * @param BufferSize 缓冲区大小 以字节计
			 *
			 * 一般不需要客户调用。派生类里有重载的方法。
			 */
			/* ----------------------------------------------------------------------------*/
			void Read( void *DataBuffer, uint32 BufferSize );
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取缓冲模式数据
			 *
			 * 一般不要客户直接调用。
			 * 
			 * @param Buffer 时间缓冲区
			 * @param dwElements 传入缓冲区大小（缓冲数组的大小，不是字节数），返回获得的事件个数。
			 * 
			 * @returns 缓冲区是否溢出。若缓冲区溢出，返回TRUE；否则返回FALSE。
			 */
			/* ----------------------------------------------------------------------------*/
			bool GetData( DataType *Buffer, uint32& dwElements );
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 处理缓冲模式数据
			 *
			 * 用回调函数处理缓冲模式数据。
			 * 
			 * @param Fn 处理函数
			 *
			 * @sa DataHandler
			 */
			/* ----------------------------------------------------------------------------*/
			void HandleData( DataHandler Fn );
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 释放设备
			 */
			/* ----------------------------------------------------------------------------*/
			void Destroy()
			{
				if( m_pDIDevice ) m_pDIDevice->Unacquire();
				SAFE_RELEASE( m_pDIDevice );
			}
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 键盘输入设备
		 */
		/* ----------------------------------------------------------------------------*/
		class KeyBoard
			: public InputDevice
		{
		private:
			void Init( uint32 BufferSize = 0 );
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 状态缓冲
			 *
			 * 用于在立即模式下读取状态
			 */
			/* ----------------------------------------------------------------------------*/
			class State
			{
				char m[256];
				friend class KeyBoard;
			public:
				//TRUE表示按下
				bool GetState( uint32 KeyCode ) const
				{
#pragma warning(push)
#pragma warning(disable : 4800)
					return m[KeyCode] & 0x80;
#pragma warning(pop)
				}
			};

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 读当前状态
			 *
			 * 立即模式操作方式
			 * 
			 * @param KeyBoardState 状态缓冲
			 */
			/* ----------------------------------------------------------------------------*/
			void Read( State &KeyBoardState )
			{
				InputDevice::Read( KeyBoardState.m, 256 );
			}
		};

		class Mouse
			: public InputDevice
		{
		private:
			void Init( uint32 BufferSize = 0 );
		public:
			class State
			{
				DIMOUSESTATE m;
				friend class Mouse;
			public:
				//TRUE表示按下
				bool GetState( uint32 KeyCode ) const
				{
#pragma warning(push)
#pragma warning(disable : 4800)
					return (bool)(m.rgbButtons[ KeyCode ] & 0x80);
#pragma warning(pop)
				}
				LONG GetlX() const
				{
					return m.lX;
				}
				LONG GetlY() const
				{
					return m.lY;
				}
				LONG GetlZ() const
				{
					return m.lZ;
				}
			};
			void Read( State &MouseState )
			{
				InputDevice::Read( &MouseState.m, sizeof( MouseState ) );
			}
		};
	}
}
#endif