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
 * @file DXGraphics.h
 * @brief 图形对象
 *
 * 接口和实现均为单后备缓冲的模式
 *
 * 关系到接口GetRenderTarget SetRenderTarget
 *
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-01-22
 */
#pragma once

#include "D3DDefs.h"

#include <d3d9.h>

#define __ONEU_DIRECT3D_VERSION DIRECT3D_VERSION
#include "PixelFormat.h"


namespace OneU
{
	namespace DX
	{
		class Surface;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 显示模式
		 */
		/* ----------------------------------------------------------------------------*/
		struct DisplayMode
		{
			uint32 Width;
			uint32 Height;
			uint32 RefreshRate;
			PXLFORMAT Format;
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief D3D顶级对象
		 * @internal
		 */
		/* ----------------------------------------------------------------------------*/
		extern IDirect3D9 * _pD3D;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief D3D顶级设备对象
		 * @internal
		 */
		/* ----------------------------------------------------------------------------*/
		extern IDirect3DDevice9 * _pD3DDevice;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 窗口化标志
		 */
		/* ----------------------------------------------------------------------------*/
		extern bool _Windowed;


		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 图形对象
		 */
		/* ----------------------------------------------------------------------------*/
		class  Graphics_t
		{
		public:
			static Graphics_t* GetInstance();
		private:
			uint32 m_Width, m_Height;
			Graphics_t( const Graphics_t& );
			Graphics_t& operator=( const Graphics_t& );
			Graphics_t()
				: m_Width( 0 ), m_Height( 0 )
			{
				ONEU_ASSERT( _pD3D == NULL );
			}
			//一般不需要用户调用
			//释放内部对象的显存资源，在丢失设备后或重置设备前调用。
			void OnLostDevice();
			//重置内部对象
			//在设备重置后调用。
			void OnResetDevice();
		public:

			~Graphics_t()
			{
				Destroy();
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 释放资源
			 */
			/* ----------------------------------------------------------------------------*/
			void Destroy();

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 初始化预处理
			 *
			 * 在初始化之前调用
			 * 主要是用于准备检测硬件
			 */
			/* ----------------------------------------------------------------------------*/
			void PreInit();
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 窗口化初始化
			 * 
			 * @param nWidth 窗口宽度
			 *
			 * 关乎到后备缓冲宽度
			 * @param nHeight 窗口高度
			 *
			 * 关乎到后备缓冲高度
			 * @param hWnd 窗口句柄
			 */
			/* ----------------------------------------------------------------------------*/
			void InitWindowed( uint32 nWidth, uint32 nHeight, HWND hWnd );
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 全屏初始化
			 * 
			 * 初始化之前应调用SetDisplayMode设置好显示格式
			 *
			 * @param pDM 显示模式
			 * @param hWnd 窗口句柄
			 *
			 * @sa SetDisplayMode
			 */
			/* ----------------------------------------------------------------------------*/
			void InitFullScreen( const DisplayMode *pDM, HWND hWnd );
			void _InitWithPtr(IDirect3D9* pD3D, IDirect3DDevice9* pDevice, uint32 Width, uint32 Height);

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 重置为窗口模式
			 *
			 * 改变窗口模式和全屏模式使用。
			 * 属于Reset简化版本。
			 * @attention 参数设定的情况无法保留，调用该函数后需要重新设定。
			 */
			/* ----------------------------------------------------------------------------*/
			void ResetWindowed( uint32 nWidth, uint32 nHeight, HWND hWnd );
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 重置为全屏模式
			 *
			 * 改变窗口模式和全屏模式使用。
			 * 属于Reset简化版本。
			 * @attention 参数设定的情况无法保留，调用该函数后需要重新设定。
			 */
			/* ----------------------------------------------------------------------------*/
			void ResetFullScreen( const DisplayMode *pDM, HWND hWnd );

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 获得设备宽度
			 * @return 宽度，以像素计。
			 */
			/* ----------------------------------------------------------------------------*/
			uint32 GetWidth() const
			{
				return m_Width;
			}
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 获得设备高度
			 * @return 高度，以像素计。
			 */
			/* ----------------------------------------------------------------------------*/
			uint32 GetHeight() const
			{
				return m_Height;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 初始化参数
			 *
			 * 把一些参数为2D化为初始值
			 * 被InitWindowed何InitFullScreen调用
			 *
			 * @sa InitWindowed InitFullScreen
			 */
			/* ----------------------------------------------------------------------------*/
			void InitParameters();
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 向后备页面填充颜色
			 * 
			 * @param crColor 32位颜色值
			 */
			/* ----------------------------------------------------------------------------*/
			void ClearTarget( COLOR crColor = 0 )
			{
				HRESULT hr;
				if( FAILED( hr = _pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, crColor, 0.0f, 0 ) ) )
					ONEU_LOG_DXERR( hr );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 翻页操作
			 */
			/* ----------------------------------------------------------------------------*/
			void Present()
			{
				HRESULT hr;
				if( FAILED( hr = _pD3DDevice->Present( NULL, NULL, NULL, NULL ) ) )
					RAISE_HRESULT(hr);
			}

#ifdef ONEU_USE_GRAPHICS_3D
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置世界变换矩阵
			 * 
			 * @param pMatrix 矩阵的值
			 * @param Level 被设置的世界变换矩阵级别
			 *
			 * 每个级别的矩阵默认为单位矩阵。
			 * 渲染时将矩阵按照级别顺序乘起来，作为顶点的变换矩阵。即可理解为从0级按顺序做变换。
			 */
			/* ----------------------------------------------------------------------------*/
			void SetWorldTransform( const MATRIX * pMatrix, BYTE Level = 0 )
			{
				DXCHECK_DEBUG( _pD3DDevice->SetTransform( D3DTS_WORLDMATRIX( Level ), pMatrix ), L"世界矩阵设置失败！" );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置视窗变换矩阵
			 * 
			 * @param pMatrix 矩阵的值
			 */
			/* ----------------------------------------------------------------------------*/
			void SetViewTransform( const MATRIX * pMatrix )
			{
				XV_DEBUG( _pD3DDevice->SetTransform( D3DTS_VIEW, pMatrix ) );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置投影变换矩阵
			 * 
			 * @param pMatrix 矩阵的值
			 */
			/* ----------------------------------------------------------------------------*/
			void SetProjectionTransform( const MATRIX * pMatrix )
			{
				XV_DEBUG( _pD3DDevice->SetTransform( D3DTS_PROJECTION, pMatrix ) );
			}
#endif
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置渲染用顶点标志位
			 * 
			 * @param FVF_Flag 顶点标志组合
			 */
			/* ----------------------------------------------------------------------------*/
			void SetFVF( uint32 FVF_Flag )
			{
				DXCHECK_DEBUG( _pD3DDevice->SetFVF( FVF_Flag ), L"FVF设置失败！" );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置渲染用顶点标志位
			 * 
			 * @tparam 顶点类型
			 */
			/* ----------------------------------------------------------------------------*/
			template< class VertexType >
			void SetFVF( VertexType * )
			{
				SetFVF( VertexType::Flag );
			}

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 设置三角面渲染方向
			 *
			 * @param Mode 整数
			 *
			 * 0表示全部渲染；
			 * 1表示隐藏顺时针面，逆时针顶点顺序的面会被渲染；
			 * -1表示隐藏逆时针面，顺时针顺序的面被渲染。
			 * 其余数字的结果不可预知。
			 */
			/* ----------------------------------------------------------------------------*/
			void SetCullMode( int Mode )
			{
				_pD3DDevice->SetRenderState( D3DRS_CULLMODE, Mode == 0 ? D3DCULL_NONE : Mode == 1 ? D3DCULL_CW : D3DCULL_CCW );
			}

			//检查显示格式
#define __ONEU_DISPLAYMODE_COUNT_INDEX //为给出count 通过index枚举模式
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获得指定像素格式的显示格式数目
			 * 
			 * @param Format 像素格式
			 * 
			 * @return 显示格式数目
			 *
			 * @sa GetAvailableDisplayMode
			 */
			/* ----------------------------------------------------------------------------*/
			uint32 GetAvailableDisplayCount( PXLFORMAT Format ){ return _pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, static_cast< D3DFORMAT > ( Format ) ); }
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获得显示格式
			 * 
			 * @param Format 显示格式的像素格式
			 * @param index 显示格式的索引
			 * 
			 * @return 指向显示格式结构体的指针
			 *
			 * @sa GetAvailableDisplayCount
			 */
			/* ----------------------------------------------------------------------------*/
			DisplayMode *GetAvailableDisplayMode( PXLFORMAT Format, int index );

#define __ONEU_DISPLAYMODE_CHECK //可检查显示模式
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 检查显示格式是否可用
			 * 
			 * @param pDM 显示格式结构体
			 * @param Windowed 窗口化标志位
			 * 
			 * @return 可用为TRUE，否则为FALSE
			 */
			/* ----------------------------------------------------------------------------*/
			bool CheckDisplayMode( const DisplayMode * pDM, bool Windowed )
			{
				if( _pD3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, static_cast< const D3DFORMAT >( pDM->Format ), static_cast< const D3DFORMAT >( pDM->Format ), Windowed ) == D3D_OK )
					return TRUE;
				return FALSE;
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取后备缓存页面（当前渲染对象页面）
			 * 
			 * @returns 渲染对象页面对象
			 */
			/* ----------------------------------------------------------------------------*/
			Surface GetRenderTarget() const;
			void SetRenderTarget( Surface &RenderTarget );

			//测试
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 测试
			 * 
			 * @param dwSeconds 持续时间
			 * 
			 * @internal
			 */
			/* ----------------------------------------------------------------------------*/
			void Test( uint32 dwSeconds );
		};

		//渲染结构
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 渲染结构体
		 *
		 * 在需要渲染的时候实例化，并对后备缓冲操作。实例析构时自动结束渲染。
		 */
		/* ----------------------------------------------------------------------------*/
		class  RenderManip
		{
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 渲染图形类型
			 */
			/* ----------------------------------------------------------------------------*/
			enum PRIMITIVETYPE{
				PT_POINTLIST = D3DPT_POINTLIST,
				PT_LINELIST = D3DPT_LINELIST,
				PT_LINESTRIP = D3DPT_LINESTRIP,
				PT_TRIANGLELIST = D3DPT_TRIANGLELIST,
				PT_TRIANGLEFAN = D3DPT_TRIANGLEFAN,
				PT_FORCE_DWORD = D3DPT_FORCE_DWORD
			};
			RenderManip()
			{
				HRESULT hr;
				if( FAILED( hr = _pD3DDevice->BeginScene() ) )
				{
					ONEU_LOG( L"渲染初始化失败！" );
					RAISE_HRESULT(hr);
				}
			}
			~RenderManip()
			{
				HRESULT hr;
				if( FAILED( hr = _pD3DDevice->EndScene() ) )
					ONEU_LOG( L"渲染结束失败！" );
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 渲染用户自定义顶点
			 * 
			 * @param PrimitiveType 渲染顶点类型
			 * @param pVertex 顶点数组
			 * @param nElem 顶点数目
			 */
			/* ----------------------------------------------------------------------------*/
			template< class VertexType >
			void RenderVertexUP( PRIMITIVETYPE PrimitiveType, VertexType * pVertex, int nElem )
			{
				XV_DEBUG( _pD3DDevice->DrawPrimitiveUP( static_cast< D3DPRIMITIVETYPE > ( PrimitiveType ), nElem, pVertex, sizeof( VertexType ) ) );
			}
		};//class Graphics_t

		inline Graphics_t* GetGraphics()
		{
			return Graphics_t::GetInstance();
		}
	}
}
