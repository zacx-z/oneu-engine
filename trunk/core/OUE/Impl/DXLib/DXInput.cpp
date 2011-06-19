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
#pragma comment( lib, "dinput8.lib" )

#include "DXInput.h"

namespace OneU
{
	namespace DX
	{
		IDirectInput8 *_pDInput = 0;

		static HWND s_hWnd = NULL;

		Input_cls* const GetInput()
		{
			static Input_cls instance;
			return &instance;
		}

		Input_cls& Input = *GetInput();

		void Input_cls::Init(HINSTANCE hInstance, HWND hWnd)
		{
			DXCHECK_THROW( DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast< void ** >( &_pDInput ), NULL ), L"初始化输入失败！" );
			s_hWnd = hWnd;
		}

		void InputDevice::Create(uint32 BufferSize)
		{
			ONEU_ASSERT(m_pDIDevice == NULL);

			Init(BufferSize);

			DXCHECK_THROW(m_pDIDevice->SetCooperativeLevel( s_hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE ), L"设置输入设备协作级别失败！");
				
			if(BufferSize)
			{
				//设置缓冲区大小 使设备能够用缓冲模式工作
				DIPROPDWORD dp;
				dp.diph.dwSize = sizeof(DIPROPDWORD);
				dp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
				dp.diph.dwObj = 0;
				dp.diph.dwHow = DIPH_DEVICE;
				dp.dwData = BufferSize;
				DXCHECK(m_pDIDevice->SetProperty(DIPROP_BUFFERSIZE, &dp.diph), L"设置输入设备缓冲区大小失败！");
			}
		}
		void InputDevice::Read( void *DataBuffer, uint32 BufferSize )
		{
			while( TRUE )
			{
				HRESULT hr;

				if( SUCCEEDED( hr = m_pDIDevice->GetDeviceState( BufferSize, DataBuffer ) ) )
					break;

				if( hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED )
					THROW_HRESULT(hr);

				if( FAILED( m_pDIDevice->Acquire() ) )
					THROW_HRESULT(hr);
			}
		}
		bool InputDevice::GetData( DataType *Buffer, uint32& dwElements )
		{
			while( TRUE )
			{
				HRESULT hr;
				if( SUCCEEDED( hr= m_pDIDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), Buffer, &dwElements, 0 ) ) )
				{
					if( hr == DI_BUFFEROVERFLOW ) return TRUE;
					else return FALSE;
				}

				// 发生了一个错误 保留
				// 这个错误有可能是 DI_BUFFEROVERFLOW 缓冲区溢出错误
				// 但不管是哪种错误，都意味着同输入设备的联系被丢失了

				// 这种错误引起的最严重的后果就是如果你按下一个键后还未松开时
				// 发生了错误，就会丢失后面松开该键的消息。这样一来，你的程序
				// 就可能以为该键尚未被松开，从而发生一些意想不到的情况

				// 现在这段代码并未处理该错误

				// 解决该问题的一个办法是，在出现这种错误时，就去调用一次
				// GetDeviceState()，然后把结果同程序最后所记录的状态进行
				// 比较，从而修正可能发生的错误

				if( hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED )
					THROW_HRESULT(hr);

				if( FAILED( m_pDIDevice->Acquire() ) )
					THROW_HRESULT(hr);

			}
		}

		void InputDevice::HandleData( DataHandler Fn )
		{
			uint32 dwElements = DATA_BUFFER_SIZE;
			static DataType Buffer[ DATA_BUFFER_SIZE ];
			bool bOverFlow;
			do
			{
				bOverFlow = GetData( Buffer, dwElements );
				for( unsigned long i = 0; i < dwElements; i++ )
				{
					( *Fn )( Buffer[i].dwOfs, Buffer[i].dwData, Buffer[i].dwTimeStamp, Buffer[i].dwSequence );
				}
			}while( bOverFlow );
		}

		void KeyBoard::Init( uint32 BufferSize )
		{
			DXCHECK_THROW( _pDInput->CreateDevice( GUID_SysKeyboard, &m_pDIDevice, NULL ), L"创建键盘输入设备失败！" );

			DXCHECK_THROW( m_pDIDevice->SetDataFormat( &c_dfDIKeyboard ), L"设置键盘输入设备数据格式失败！" );

		}

		void Mouse::Init( uint32 BufferSize )
		{
			DXCHECK_THROW( _pDInput->CreateDevice( GUID_SysMouse, &m_pDIDevice, NULL ), L"创建鼠标输入设备失败！" );

			DXCHECK_THROW( m_pDIDevice->SetDataFormat( &c_dfDIMouse ), L"设置鼠标输入设备数据格式失败！" );
		}
	}
}