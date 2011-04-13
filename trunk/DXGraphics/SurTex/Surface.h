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
#pragma once
#include "STDef.h"
namespace OneU
{
	namespace DX
	{
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 表面基类型
		 *
		 * 把所有的DirectX调用封装到各个函数里 并为各种表面提供一个基类。
		 * 这些类没有多态性，接口不同仅仅是为了实现DirectX的编译期检查。
		 *
		 */
		/* ----------------------------------------------------------------------------*/
		class ONEU_DXGRAPHICS_API Surface_Base
		{
		private:
		
			IDirect3DSurface9 * m_pSurface;

#ifdef __ONEU_USE_CLASS_NUM

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 类的生成实例数目
			 */
			/* ----------------------------------------------------------------------------*/
			static int s_Num;
			static void IncClassNum(){ s_Num++; }
			static void DecClassNum(){ s_Num--; }
		public:
			static int GetClassNum(){ return s_Num; }
#endif
		protected:
			void _Create( uint nWidth, uint nHeight, PXLFORMAT Format, D3DPOOL Pool )
			{
				ASSERT( m_pSurface == NULL );
				DXCHECK_THROW( _pD3DDevice->CreateOffscreenPlainSurface( nWidth, nHeight, static_cast< D3DFORMAT > ( Format ), Pool, &m_pSurface, NULL ),
					L"表面创建失败！" );
			}
			void _CreateRenderTarget( uint nWidth, uint nHeight, PXLFORMAT Format, dword MultiSample, dword MultisampleQuality, bool Lockable )
			{
				ASSERT( m_pSurface == NULL );
				DXCHECK_THROW(
					_pD3DDevice->CreateRenderTarget( nWidth, nHeight, static_cast< D3DFORMAT > ( Format ),
					static_cast< D3DMULTISAMPLE_TYPE > ( MultiSample ), MultisampleQuality, Lockable, &m_pSurface, NULL ),
					L"渲染目标表面创建失败！" );
			}
			void _CreateDepthSurface( uint nWidth, uint nHeight, PXLFORMAT Format, dword MultiSample, dword MultisampleQuality, bool Discard )
			{
				ASSERT( m_pSurface == NULL );
				DXCHECK_THROW(
					_pD3DDevice->CreateDepthStencilSurface( nWidth, nHeight, static_cast< D3DFORMAT > ( Format ),
					static_cast< D3DMULTISAMPLE_TYPE > ( MultiSample ), MultisampleQuality, Discard, &m_pSurface, NULL ),
					L"深度表面创建失败！" );
			}
		public:
			typedef D3DSURFACE_DESC INFO;

			Surface_Base()
				: m_pSurface( NULL )
			{
				//保证构造函数的顺序
				GetGraphics();
				//拷贝构造函数和赋值函数不需要 因为那样必有Surface构造Graphics必然构造
#ifdef __ONEU_USE_CLASS_NUM
				IncClassNum();
#endif
			}
			explicit Surface_Base( IDirect3DSurface9 * pSurface )
				: m_pSurface( pSurface )
			{
#ifdef __ONEU_USE_CLASS_NUM
				IncClassNum();
#endif
			}
			Surface_Base( Surface_Base & rhs )
				: m_pSurface( rhs.m_pSurface )
			{
#ifdef __ONEU_USE_CLASS_NUM
				IncClassNum();
#endif
				rhs.m_pSurface = NULL;
			}
			Surface_Base & operator = ( Surface_Base & rhs )
			{
				ASSERT( !m_pSurface );
					m_pSurface = rhs.m_pSurface;
				rhs.m_pSurface = NULL;
				return ( * this );
			}
			~Surface_Base()
			{
#ifdef __ONEU_USE_CLASS_NUM
				DecClassNum();
#endif
				Destroy();
			}
		
			IDirect3DSurface9 * _Obtain() const { return m_pSurface; }
			D3DSURFACE_DESC & _GetDesc() const
			{
				static D3DSURFACE_DESC s_SD = {};
				DXCHECK( m_pSurface->GetDesc( &s_SD ), L"获取表面信息失败" );
					return s_SD;
			}

			void Create( uint nWidth, uint nHeight, PXLFORMAT Format )
			{
				_Create( nWidth, nHeight, Format, D3DPOOL_DEFAULT );
			}
			void Destroy()
			{
				SAFE_RELEASE( m_pSurface );
			}
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 判断表面是否被创建
			 *
			 * @return true表面已创建，false表面未创建或已释放。
			 */
			/* ----------------------------------------------------------------------------*/
			bool IsCreated() const { return m_pSurface != 0; }
		
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取表面信息句柄
			 *
			 * 当要获取多个表面信息的时候，先获得此句柄，
			 * 再将此句柄传入获取信息函数会快一些。参见GetWidth，GetHeight。
			 * 
			 * @return 信息句柄
			 *
			 * @sa GetWidth GetHeight
			 */
			/* ----------------------------------------------------------------------------*/
			//表面信息句柄
			const INFO& GetInfo() const
			{
				return _GetDesc();
			}
		
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取宽度
			 *
			 * 获取表面的宽度，以像素计。
			 * 
			 * @param Info 表面信息句柄
			 *
			 * 句柄由GetInfo成员函数获得。
			 * 
			 * @return 表面宽度，以像素计。
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			//获取表面信息
			uint GetWidth( const INFO& Info ) const
			{
				return Info.Width;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取宽度
			 *
			 * 获取表面的宽度，以像素计。
			 * 如果在一处使用多个获取信息函数的话。
			 * 请使用GetInfo。
			 * 
			 * @return 表面宽度，以像素计。
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			uint GetWidth() const
			{
				return GetWidth( GetInfo() );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取高度
			 *
			 * 获取表面的高度，以像素计。
			 * 
			 * @param Info 表面信息句柄
			 *
			 * 句柄由GetInfo成员函数获得。
			 * 
			 * @return 表面高度，以像素计。
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			uint GetHeight( const INFO& Info ) const
			{
				return Info.Height;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取高度
			 *
			 * 获取表面的高度，以像素计。
			 * 如果在一处使用多个获取信息函数的话。
			 * 请使用GetInfo。
			 * 
			 * @return 表面高度，以像素计。
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			uint GetHeight() const
			{
				return GetHeight( GetInfo() );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取像素格式
			 *
			 * 获取表面的像素格式。
			 * 
			 * @param Info 表面信息句柄
			 *
			 * 句柄由GetInfo成员函数获得。
			 * 
			 * @return 表面格式
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			PXLFORMAT GetFormat( const INFO& Info ) const
			{
				return static_cast< PXLFORMAT > ( Info.Format );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取像素格式
			 *
			 * 获取表面的像素格式。
			 * 如果在一处使用多个获取信息函数的话。
			 * 请使用GetInfo。
			 * 
			 * @return 表面格式
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			PXLFORMAT GetFormat() const
			{
				return GetFormat( GetInfo() );
			}
		
			//以下是会被继承类掩盖掉的函数
			//没有静态检查的DXAPI函数
			void UpdateSurface( POINT * pDtnPoint, const Surface_Base * pSrcSurface, RECT * pSrcRect )
			{
				DXCHECK( _pD3DDevice->UpdateSurface( pSrcSurface->m_pSurface, pSrcRect, m_pSurface, pDtnPoint ),
					L"拷贝表面失败！" );
			}
			void StretchRect( RECT * pDtnRect, const Surface_Base * pSrcSurface, RECT * pSrcRect )
			{
				DXCHECK( _pD3DDevice->StretchRect( pSrcSurface->m_pSurface, pSrcRect, m_pSurface, pDtnRect, _TexFilter ),
					L"（无放缩）拷贝表面失败！" );
			}
			void ColorFill( COLOR crColor, RECT * pRect = NULL )
			{
				HRESULT hr = _pD3DDevice->ColorFill( m_pSurface, pRect, crColor );
				if( FAILED( hr ) )
				{
					ONEU_LOG( L"填充表面失败！");
					ONEU_LOG_DXERR( hr );
				}
			}
			//RenderTarget Surface专用
			void GetData( Surface_Base * pDestSurface )
			{
				DXCHECK( _pD3DDevice->GetRenderTargetData( m_pSurface, pDestSurface->_Obtain() ), L"获取渲染目标表面数据失败！" );
			}
		};
		
		//前置声明
		template< uint _Usage, D3DPOOL Pool >
		class Surface_Texture;
		
		//__Surface_Base的Wrapper
		//根据Pool提供接口
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 普通离屏表面类
		 *
		 * 使用时，调用Create来创建表面
		 *
		 * @tparam _Pool内存池类型
		 */
		/* ----------------------------------------------------------------------------*/
		template< D3DPOOL _Pool >
		class Surface_t
			: public Surface_Base
		{
		private:
			void _ColorFill( COLOR crColor, RECT *pRect, __True_Category )
			{
				Surface_Base::ColorFill( crColor, pRect );
			}
			void _UpdateSurface( POINT * pDtnPoint, const Surface_t< D3DPOOL_SYSTEMMEM > * pSrcSurface, RECT * pSrcRect, __True_Category )
			{
				Surface_Base::UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect );
			}
			void _CopyRect( POINT * pDtnPoint, const Surface_t< D3DPOOL_DEFAULT > * pSrcSurface, RECT * pSrcRect, __True_Category )
			{
				RECT DtnRect = {
					pDtnPoint->x, pDtnPoint->y,
					pDtnPoint->x + pSrcRect->right-pSrcRect->left,
					pDtnPoint->y + pSrcRect->bottom - pSrcRect->top
				};
				Surface_Base::StretchRect( &DtnRect, pSrcSurface, pSrcRect );
			}
		public:
			enum{ Pool = _Pool };
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 创建表面
			 *
			 * 每个离屏表面必须要通过该方法创建表面才能使用。
			 * 
			 * @param nWidth 表面宽度，以像素计
			 * @param nHeight 表面高度，以像素计
			 * @param Format 表面像素格式
			 */
			/* ----------------------------------------------------------------------------*/
			void Create( uint nWidth, uint nHeight, PXLFORMAT Format )
			{
				_Create( nWidth, nHeight, Format, _Pool );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 填充颜色
			 *
			 * 将表面填充成crColor的颜色。
			 * 
			 * @param crColor 填充的颜色值
			 * @param pRect 填充矩形的地址
			 *
			 * 如果参数为NULL，整个表面会被填充。
			 */
			/* ----------------------------------------------------------------------------*/
			void ColorFill( COLOR crColor, RECT *pRect = NULL )
			{
				_ColorFill( crColor, pRect, __Surface_Pool_Traits< _Pool >::ColorFill_Category );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 更新表面
			 *
			 * 从另一个表面拷贝内容到该表面。
			 * 源表面Pool类型必须为D3DPOOL_SYSTEMMEM（Surface_Lk）。目的表面Pool类型必须为D3DPOOL_DEFAULT（Surface）。
			 * 表面必须使用同样的像素格式，
			 * 且不能为Depth Stencil Format。
			 * @todo（长期，不知道什么概念，应该不需要考虑这点。用Surface_Depth Surface_Stencil来单独管理那种像素格式的表面）
			 *  
			 * @param pDtnPoint 目的表面（该表面）被拷贝的矩形的左上角点地址
			 *
			 * 如果参数为NULL，整个表面都会被拷贝。
			 * @param pSrcSurface 源表面的地址。
			 * @param pSrcRect 源表面的矩形地址
			 *
			 * 如果参数为NULL，整个表面都会被拷贝。
			 */
			/* ----------------------------------------------------------------------------*/
			void UpdateSurface( POINT * pDtnPoint, const Surface_t< D3DPOOL_SYSTEMMEM > * pSrcSurface, RECT * pSrcRect )
			{
				_UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect, __Surface_Pool_Traits< _Pool >::UpdateSurfaceDestination_Category );
			}
			//同上
			template< uint Usage_, D3DPOOL Pool_ >
			void UpdateSurface( POINT * pDtnPoint, const Surface_Texture< Usage_, Pool_ > * pSrcSurface, RECT * pSrcRect )
			{
				_UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect, __Texture_UgPl_Traits< Usage_, Pool_ >::UpdateSurfaceSource_Category );
			}
			//同上
			void UpdateSurface( POINT * pDtnPoint, const Surface_t< D3DPOOL_DEFAULT > * pSrcSurface, RECT * pSrcRect )
			{
				_CopyRect( pDtnPoint, pSrcSurface, pSrcRect, __Surface_Pool_Traits< _Pool >::StretchRectDestination_Category );
			}
		
		};
		
		
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 渲染目标表面
		 *
		 * 可调用Create创建表面。
		 * 可调用Graphics.GetRenderTarget获得后备渲染目标表面。
		 */
		/* ----------------------------------------------------------------------------*/
		class ONEU_DXGRAPHICS_API Surface_RenderTarget
			: public Surface_Base
		{
		private:
			void _UpdateSurface( POINT * pDtnPoint, const Surface_Base * pSrcSurface, RECT * pSrcRect, __True_Category )
			{
				Surface_Base::UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect );
			}
		public:
			Surface_RenderTarget()
				: Surface_Base(){}
			Surface_RenderTarget( IDirect3DSurface9 * pRenderTarget )
				: Surface_Base( pRenderTarget ){}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 创建渲染目标表面
			 * 
			 * @param nWidth 表面宽度，以像素计
			 * @param nHeight 表面高度，以像素计
			 * @param Format 表面像素格式
			 * @param MultiSample 采样数量，0为默认不适用多重采样，1为使用采样质量参数
			 * @param MultisampleQuality 采样质量
			 *
			 * 合法值在0和CheckDeviceMultiSampleType返回值减一之间。
			 *
			 * @param Lockable 锁定参数
			 *
			 * 设定为TRUE可使表面能够被锁定。
			 *
			 * @sa Graphics_t::CheckDeviceMultiSampleType
			 */
			/* ----------------------------------------------------------------------------*/
			void Create( uint nWidth, uint nHeight, PXLFORMAT Format, dword MultiSample, dword MultisampleQuality, bool Lockable )
			{
				_CreateRenderTarget( nWidth, nHeight, Format, MultiSample, MultisampleQuality, Lockable );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 更新渲染目标表面
			 *
			 * 源表面Pool类型为D3DPOOL_SYSTEMMEM。
			 * 源表面为离屏表面。
			 * 如果设备支持，源表面可以为普通纹理表面
			 * @todo（已屏蔽此功能）（长期）
			 * 
			 * @param pDtnPoint 目的表面（该表面）被拷贝的矩形的左上角点地址
			 *
			 * 如果参数为NULL，整个表面都会被拷贝。
			 * @param pSrcSurface 源表面的地址。
			 * @param pSrcRect 源表面的矩形地址
			 *
			 * 如果参数为NULL，整个表面都会被拷贝。
			 */
			/* ----------------------------------------------------------------------------*/
			void UpdateSurface( POINT * pDtnPoint, const Surface_t< D3DPOOL_SYSTEMMEM > * pSrcSurface, RECT * pSrcRect )
			{
				Surface_Base::UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect );
			}
			//template< uint Usage_, D3DPOOL Pool_ >
			//void UpdateSurface( POINT * pDtnPoint, const __Surface_Texture< Usage_, Pool_ > * pSurface, RECT * pSrcRect )
			//{
			//	_UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect, __Texture_UgPl_Traits< Usage_, Pool_ >::UpdateSurfaceSource_Category );
			//}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 拉伸拷贝渲染目标表面
			 *
			 * 源表面Pool类型必须为D3DPOOL_DEFAULT。
			 * 像素格式不可以是Compressed Texture Formats
			 * 可以设定Stretch Filter。
			 * 可以对多重采样渲染目标操作。
			 * 更高级问题……
			 * 
			 * @param pDtnRect 目的表面（该表面）被拷贝的矩形地址
			 *
			 * 如果参数为NULL，会拷贝到整个表面上。
			 * @param pSrcSurface 源表面的地址。
			 * @param pSrcRect 源表面的矩形地址。
			 *
			 * 如果参数为NULL，整个表面都会被拷贝。
			 */
			/* ----------------------------------------------------------------------------*/
			void StretchRect( RECT * pDtnRect, const Surface_t< D3DPOOL_DEFAULT > * pSrcSurface, RECT * pSrcRect )
			{
				Surface_Base::StretchRect( pDtnRect, pSrcSurface, pSrcRect );
			}
			void StretchRect( RECT * pDtnRect, const Surface_RenderTarget * pSrcSurface, RECT * pSrcRect )
			{
				Surface_Base::StretchRect( pDtnRect, pSrcSurface, pSrcRect );
			}
			template< uint _Usage, D3DPOOL _Pool >
			void StretchRect( RECT * pDtnRect, const Surface_Texture< _Usage, _Pool > * pSurface, RECT * pSrcRect )
			{
				_StretchRect( pDtnRect, pSrcSurface, pSrcRect, __Texture_UgPl_Traits< _Usage, _Pool >::StretchRectSource );
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获得渲染目标表面数据
			 *
			 * 将此表面的内容拷贝到目标表面上。
			 * 目标表面必须Pool类型必须为D3DPOOL_SYSTEMMEM。
			 * 渲染目标表面不可以是多重采样的。
			 * 二者像素格式必须相同，尺寸必须相同。
			 * 
			 * @param pDestSurface 目标表面地址
			 */
			/* ----------------------------------------------------------------------------*/
			void GetData( Surface_t< D3DPOOL_SYSTEMMEM > * pDestSurface )
			{
				Surface_Base::GetData( pDestSurface );
			}
			template< uint Usage_ >
			void GetData( Surface_Texture< Usage_, D3DPOOL_SYSTEMMEM > * pDestSurface )
			{
				Surface_Base::GetData( pDestSurface );
			}
		};
		//深度缓冲页面
		//@todo（长期）
		class ONEU_DXGRAPHICS_API Surface_Depth
			: Surface_Base
		{
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 纹理表面基类
		 *
		 * 作为所有纹理表面的基类提供指针
		 * 能够用此类的指针来指向纹理的表面
		 * 封装特殊操作（目前还没有）
		 */
		/* ----------------------------------------------------------------------------*/
		class ONEU_DXGRAPHICS_API Surface_Texture_Base
			: public Surface_Base
		{
		public:
			explicit Surface_Texture_Base( IDirect3DSurface9 * pSurface )
				: Surface_Base( pSurface ){}
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 纹理表面类
		 *
		 * @tparam _Usage 对应纹理的Usage
		 * @tparam _Pool 对应纹理的Pool
		 *
		 * 该模板类具现化的类与__Texture具现化的类一一对应
		 */
		/* ----------------------------------------------------------------------------*/
		template< uint _Usage, D3DPOOL _Pool >
		class Surface_Texture
			: public Surface_Texture_Base
		{
		public:
			explicit Surface_Texture( IDirect3DSurface9 * pSurface )
				: Surface_Texture_Base( pSurface )
			{
			}
		private:
			void _ColorFill( COLOR crColor, RECT * pRect, __True_Category )
			{
				Surface_Base::ColorFill( crColor, pRect );
			}
			void _UpdateSurface( POINT * pDtnPoint, const Surface_Base * pSrcSurface, RECT * pSrcRect, __True_Category )
			{
				Surface_Base::UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect );
			}
			void _StretchRect( RECT * pDtnRect, const Surface_Base * pSrcSurface, RECT * pSrcRect, __True_Category )
			{
				Surface_Base::StretchRect( pDtnRect, pSrcSurface, pSrcRect );
			}
			void _GetData( Surface_Base * pDestSurface )
			{
				Surface_Base::GetData( pDestSurface );
			}
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 填充纹理表面
			 *
			 * 表面Pool类型必须为D3DPOOL_DEFAULT。
			 * 
			 * @param crColor 填充颜色值
			 * @param pRect 填充矩形地址
			 *
			 * 如果参数为NULL，整个表面会被填充。
			 */
			/* ----------------------------------------------------------------------------*/
			void ColorFill( COLOR crColor, RECT * pRect = NULL )
			{
				_ColorFill( crColor, pRect, __Texture_UgPl_Traits< _Usage, _Pool >::ColorFill_Category() );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 更新纹理表面
			 *
			 * 源表面Pool类型必须为D3DPOOL_SYSTEMMEM。
			 * 目的表面Pool类型必须为D3DPOOL_DEFAULT。
			 * 
			 * @param pDtnPoint 目的表面（该表面）被拷贝的矩形地址
			 *
			 * 如果参数为NULL，会拷贝到整个表面上。
			 * @param pSrcSurface  源表面的地址。
			 * @param pSrcRect 源表面的矩形地址
			 *
			 * 如果参数为NULL，整个表面都会被拷贝。
			 */
			/* ----------------------------------------------------------------------------*/
			void UpdateSurface( POINT * pDtnPoint, const Surface_t< D3DPOOL_SYSTEMMEM > * pSrcSurface, RECT * pSrcRect )
			{
				_UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect, __Texture_UgPl_Traits< _Usage, _Pool >::UpdateSurfaceDestination_Category );
			}
			template< uint Usage_, D3DPOOL Pool_ >
			void UpdateSurface( POINT * pDtnPoint, const Surface_Texture< Usage_, Pool_ > * pSrcSurface, RECT * pSrcRect )
			{
				_UpdateSurface( pDtnPoint, pSrcSurface, pSrcRect,
					__Bool_Category<
					__Texture_UgPl_Traits< Usage_, Pool_ >::bUpdateSurfaceSource && __Texture_UgPl_Traits< _Usage, _Pool >::bUpdateSurfaceDestination
					>::Category());
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 拉伸拷贝渲染目标纹理表面
			 *
			 * 仅限于渲染目标纹理使用。
			 * 源表面Pool类型必须为D3DPOOL_DEFAULT。
			 * 像素格式不可以是Compressed Texture Formats
			 * 可以设定Stretch Filter。
			 * 可以对多重采样渲染目标操作。
			 * 更高级问题……
			 * 
			 * @param pDtnRect 目的表面（该表面）被拷贝的矩形地址
			 *
			 * 如果参数为NULL，会拷贝到整个表面上。
			 * @param pSrcSurface 源表面的地址。
			 * @param pSrcRect 源表面的矩形地址。
			 *
			 * 如果参数为NULL，整个表面都会被拷贝
			 */
			/* ----------------------------------------------------------------------------*/
			void StretchRect( RECT * pDtnRect, const Surface_t< D3DPOOL_DEFAULT > * pSrcSurface, RECT * pSrcRect )
			{
				_StretchRect( pDtnRect, pSrcSurface, pSrcRect, __Texture_UgPl_Traits< _Usage, _Pool >::StretchRectDestination_Category() );
			}
			void StretchRect( RECT * pDtnRect, const Surface_RenderTarget * pSrcSurface, RECT * pSrcRect )
			{
				_StretchRect( pDtnRect, pSrcSurface, pSrcRect, __Texture_UgPl_Traits< _Usage, _Pool >::StretchRectDestination_Category() );
			}
			template< uint Usage_, D3DPOOL Pool_ >
			void StretchRect( RECT * pDtnRect, const Surface_Texture< Usage_, Pool_ > * pSrcSurface, RECT * pSrcRect )
			{
				_StretchRect( pDtnRect, pSrcSurface, pSrcRect,
					__Bool_Category<
					__Texture_UgPl_Traits< _Usage, _Pool >::bStretchRectDestination && __Texture_UgPl_Traits< Usage_, Pool_ >::bStretchRectSource
							>::Category() );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取渲染目标纹理表面数据
			 * 
			 * 将此表面的内容拷贝到目标表面上。
			 * 目标表面必须Pool类型必须为D3DPOOL_SYSTEMMEM。
			 * 渲染目标表面不可以是多重采样的。
			 * 二者像素格式必须相同，尺寸必须相同。
			 * 
			 * @param pDestSurface 目标表面地址
			 */
			/* ----------------------------------------------------------------------------*/
			void GetData( Surface_t< D3DPOOL_SYSTEMMEM > * pDestSurface )
			{
				_GetData( pDestSurface, __Bool_Category< _Usage == D3DUSAGE_RENDERTARGET >::Category() );
			}
			template< uint Usage_ >
			void GetData( Surface_Texture< Usage_, D3DPOOL_SYSTEMMEM > * pDestSurface )
			{
				_GetData( pDestSurface, __Bool_Category< _Usage == D3DUSAGE_RENDERTARGET >::Category() );
			}
		};		
	}
}
