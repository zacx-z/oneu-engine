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

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3dx9.lib" )

#include "DXGraphics.h"
#include <d3dx9.h>
#include "VertexUP.h"

#include "Texture.h"

namespace OneU
{
	//图形模块对象
	namespace DX
	{
#ifdef ONEU_USE_DIRECT3D
		uint32 s_Filter = D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER;
		uint32 s_MipFilter = D3DX_FILTER_BOX;
#endif

		Graphics_t &Graphics = *GetGraphics();
		Graphics_t* Graphics_t::GetInstance()
		{
			static Graphics_t g;
			return &g;
		}

		//纹理过滤类型 用于StretchRect
		D3DTEXTUREFILTERTYPE _TexFilter = D3DTEXF_LINEAR;

		IDirect3D9 * _pD3D = 0;
		IDirect3DDevice9 * _pD3DDevice = 0;
		bool _Windowed = false;

		//注意：此时把D3DMULTISAMPLE_TYPE强制用dword表示
		//若DXSDK变化 从dword到D3DMULTISAMPLE_TYPE的强制转换有可能导致错误
		static uint32 g_MultiSampleType = 0;
		static uint32 g_MultiSampleQuality = 0;

		//初始化图形预处理 主要是为了检测硬件
		void Graphics_t::PreInit()
		{
			ONEU_ASSERT( _pD3D == 0 );
			_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
			if( _pD3D == 0 )
			{
				ONEU_LOG( L"创建Direct3D对象失败！" );
				RAISE_HRESULT(0);
			}
		}
		void Graphics_t::InitWindowed( uint32 nWidth, uint32 nHeight, HWND hWnd )
		{
			ONEU_ASSERT( _pD3D );
			ONEU_ASSERT( _pD3DDevice == 0 );

			m_Width = nWidth;m_Height = nHeight;

			_Windowed = true;

			HRESULT hr;
			//获取当前显示模式
			D3DDISPLAYMODE d3ddm;

			hr = _pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

			if( FAILED( hr ) )
			{
				ONEU_LOG( L"获取显示模式失败" );
				RAISE_HRESULT(hr);
			}

			//
			D3DPRESENT_PARAMETERS d3dpp;
			memset(&d3dpp, 0, sizeof(d3dpp));

			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = d3ddm.Format;

			d3dpp.BackBufferWidth = nWidth;
			d3dpp.BackBufferHeight = nHeight;
			d3dpp.BackBufferCount = 1;

			d3dpp.MultiSampleType = static_cast< D3DMULTISAMPLE_TYPE >( g_MultiSampleType );
			d3dpp.MultiSampleQuality = g_MultiSampleQuality;

			hr = _pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&_pD3DDevice );

			if( FAILED( hr ) )
			{
				ONEU_LOG(L"创建Direct3D设备失败");
				RAISE_HRESULT(hr);
			}

			InitParameters();

		}

		void Graphics_t::InitFullScreen( const DisplayMode *pDM, HWND hWnd )
		{
			ONEU_ASSERT( _pD3D );
			ONEU_ASSERT( _pD3DDevice == 0 );

			m_Width = pDM->Width;m_Height = pDM->Height;

			_Windowed = false;

			HRESULT hr;

			D3DPRESENT_PARAMETERS d3dpp;
			memset(&d3dpp, 0, sizeof(d3dpp));

			d3dpp.Windowed = FALSE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			d3dpp.BackBufferFormat = (D3DFORMAT)pDM->Format;

			d3dpp.BackBufferWidth = (D3DFORMAT)pDM->Width;
			d3dpp.BackBufferHeight = (D3DFORMAT)pDM->Height;
			d3dpp.BackBufferCount = 1;

			d3dpp.MultiSampleType = static_cast< D3DMULTISAMPLE_TYPE >( g_MultiSampleType );
			d3dpp.MultiSampleQuality = g_MultiSampleQuality;

			hr = _pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&_pD3DDevice);

			if( FAILED( hr ) )
			{
				ONEU_LOG( L"创建Direct3D设备失败" );
				RAISE_HRESULT(hr);
			}
			InitParameters();
		}

		void Graphics_t::_InitWithPtr(IDirect3D9* pD3D, IDirect3DDevice9* pDevice, uint32 nWidth, uint32 nHeight){
			_pD3D = pD3D;
			_pD3DDevice = pDevice;

			m_Width = nWidth;
			m_Height = nHeight;
		}

		void Graphics_t::ResetWindowed( uint32 nWidth, uint32 nHeight, HWND hWnd )
		{
			ONEU_ASSERT( _pD3D );
			ONEU_ASSERT( _pD3DDevice );

			OnLostDevice();
			
			m_Width = nWidth;m_Height = nHeight;
			
			_Windowed = true;

			HRESULT hr;
			//获取当前显示模式

			D3DDISPLAYMODE d3ddm;
			hr = _pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

			if( FAILED( hr ) )
			{
				ONEU_LOG( L"获取显示模式失败" );
				RAISE_HRESULT(hr);
			}

			//
			D3DPRESENT_PARAMETERS d3dpp;
			memset( &d3dpp, 0, sizeof( d3dpp ) );
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = d3ddm.Format;

			d3dpp.BackBufferWidth = nWidth;
			d3dpp.BackBufferHeight = nHeight;
			d3dpp.BackBufferCount = 1;

			d3dpp.MultiSampleType = static_cast< D3DMULTISAMPLE_TYPE >( g_MultiSampleType );
			d3dpp.MultiSampleQuality = g_MultiSampleQuality;

			hr = _pD3DDevice->Reset( &d3dpp );

			if( FAILED( hr ) )
			{
				ONEU_LOG(L"重置Direct3D设备失败");
				RAISE_HRESULT(hr);
			}

			OnResetDevice();

			InitParameters();
		}
		void Graphics_t::ResetFullScreen( const DisplayMode *pDM, HWND hWnd )
		{
			ONEU_ASSERT( _pD3D );
			ONEU_ASSERT( _pD3DDevice );

			OnLostDevice();

			m_Width = pDM->Width;m_Height = pDM->Height;

			_Windowed = false;

			HRESULT hr;

			D3DPRESENT_PARAMETERS d3dpp;
			memset( &d3dpp, 0, sizeof( d3dpp ) );
			d3dpp.Windowed = FALSE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			d3dpp.BackBufferFormat = (D3DFORMAT)pDM->Format;

			d3dpp.BackBufferWidth = pDM->Width;
			d3dpp.BackBufferHeight = pDM->Height;
			d3dpp.BackBufferCount = 1;

			d3dpp.MultiSampleType = static_cast< D3DMULTISAMPLE_TYPE >( g_MultiSampleType );
			d3dpp.MultiSampleQuality = g_MultiSampleQuality;

			hr = _pD3DDevice->Reset( &d3dpp );

			if( FAILED( hr ) )
			{
				ONEU_LOG(L"重置Direct3D设备失败");
				RAISE_HRESULT(hr);
			}

			OnResetDevice();

			InitParameters();
		}

		void Graphics_t::InitParameters()
		{
			SetCullMode( 1 );
			_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
			_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}


		void Graphics_t::OnLostDevice()
		{
		}
		void Graphics_t::OnResetDevice()
		{
		}

		DisplayMode * Graphics_t::GetAvailableDisplayMode( PXLFORMAT Format, int index )
		{
			static DisplayMode _DM;
			HRESULT hr;
			if( FAILED( hr = _pD3D->EnumAdapterModes( D3DADAPTER_DEFAULT, static_cast< D3DFORMAT > ( Format ), index, reinterpret_cast< D3DDISPLAYMODE* > ( &_DM ) ) ) )
				RAISE_HRESULT(hr);

			return &_DM;
		}

		Surface Graphics_t::GetRenderTarget() const
		{
			IDirect3DSurface9 * pRenderTarget;
			DXCHECK_RAISE( _pD3DDevice->GetRenderTarget( 0, &pRenderTarget ), L"获取后备渲染目标失败！" );
			return Surface( pRenderTarget );
		}

		void Graphics_t::SetRenderTarget( Surface &RenderTarget )
		{
			DXCHECK_RAISE( _pD3DDevice->SetRenderTarget( 0, RenderTarget._Obtain() ), L"设置后备渲染目标失败！" );
		}

		void Graphics_t::Destroy()
		{
			if( _pD3DDevice )
			{
				DXCHECK_RAISE( _pD3DDevice->Release(), L"图形设备释放失败！" );
				_pD3DDevice = 0;
			}
			if( _pD3D )
			{
				DXCHECK_RAISE( _pD3D->Release(), L"图形组件释放失败！" );
				_pD3D = 0;
			}
		}


		//测试用代码
		struct VertexStruct
		{
			float x, y, z;
			D3DCOLOR diffuse;
			VertexStruct()
				: z( 1.0f ){}
		};
		//void Graphics_t::Test( uint32 dwSeconds )
		//{
		//	VertexUP< FVF_XYZ | FVF_DIFFUSE | FVF_TEX1 > a[3];
		//	struct 
		//	{
		//		enum {Flag = FVF_XYZ| FVF_DIFFUSE |FVF_TEX1 };
		//		float x,y,z;
		//		D3DCOLOR Diff;
		//		float u,v;
		//		void SetPos( float ax, float ay, float az )
		//		{
		//			x = ax; y = ay; z = az;
		//		}
		//		void SetUV( float au, float av )
		//		{
		//			u = au; v = av;
		//		}
		//		D3DCOLOR & Diffuse()
		//		{
		//			return Diff;
		//		}
		//	} b[ 3 ];
		//	/*a[0].diffuse = D3DCOLOR_XRGB( 0xff, 0, 0 );
		//	a[1].diffuse = D3DCOLOR_XRGB( 0, 0xff, 0 );
		//	a[2].diffuse = D3DCOLOR_XRGB( 0, 0, 0xff );

		//	//测试
		//	a[0].x = -0.732f; a[0].y = 0.5f;
		//	a[1].x = 0.732f; a[1].y = 0.5f;
		//	//a[2].x = 0.0f;
		//	//a[2].y = -1.0f;
		//	p->X() = 0.0f;
		//	p->Y() = -1.0f;
		//	p->Diffuse() = 0;*/
		//	a[0].SetUV< 0 >( 0, 0 );
		//	a[1].SetUV< 0 >( 3, 0 );
		//	a[2].SetUV< 0 >( 0, 3 );
		//	float p = a[2].U< 0 >();
		//	a[0].Diffuse() = D3DCOLOR_XRGB( 0xff, 0, 0 );
		//	a[1].Diffuse() = D3DCOLOR_XRGB( 0, 0xff, 0 );
		//	a[2].Diffuse() = D3DCOLOR_XRGB( 0, 0, 0xff );

		//	a[0].SetPos( -0.732f, 0.5f, 1.0f );
		//	a[1].SetPos( 0.732f, 0.5f, 1.0f );
		//	a[2].SetPos( 0.0f, -1.0f, 1.0f );


		//	::memcpy( b, a, sizeof( a ) );


		//	SetFVF( a );
		//	
		//	_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
		//	//_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

		//	D3DXMATRIX mat;

		//	D3DXMatrixIdentity( &mat );

		//	//_pD3DDevice->SetTransform( D3DTS_WORLD, &mat );
		//	//_pD3DDevice->SetTransform( D3DTS_VIEW, &mat );

		//	D3DXMATRIX matProj;
		//	D3DXMatrixPerspectiveFovLH(&matProj,D3DX_PI / 4, 1.0f, 0.1f, 1000.0f);

		//	/*D3DLOCKED_RECT DDLR;
		//	__Surface< D3DPOOL_SYSTEMMEM > sf;
		//	sf.Create( 128, 128, PXLFMT_X8R8G8B8 );
		//	sf._ObtainSurface()->LockRect( &DDLR, NULL, D3DLOCK_DISCARD );
		//	for( int i = 0; i < 128 ; i++ )
		//		for( int j = 0; j < 128 * 4; j ++ )
		//		{
		//			unsigned char * a = reinterpret_cast< unsigned char * > ( DDLR.pBits );
		//			*( a + i * DDLR.Pitch + j ) = i + j;;
		//		}
		//	sf._ObtainSurface()->UnlockRect();
		//	Texture2D t;
		//	t.CreateFromSurface( sf );*/
		//	Texture_t< 0, D3DPOOL_SYSTEMMEM > t2;
		//	Texture_t< D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT > t3,t;
		//	t3.Create( 200, 200, PXLFMT_X8R8G8B8, 3 );
		//	CreateTextureFromFile( L"A.jpg", t2, 2, PXLFMT_X8R8G8B8 );
		//	RECT rc = { 0, 0, 200, 200 };
		//	t3.GetSurfaceLevel( 0 ).UpdateSurface( NULL, &( t2.GetSurfaceLevel( 1 ) ), &rc );
		//	t.Create( 200, 200, PXLFMT_X8R8G8B8, 1 );
		//	t.GetSurfaceLevel( 0 ).StretchRect( NULL, &(t3.GetSurfaceLevel( 0 ) ), NULL );


		//	_pD3DDevice->SetTexture( 0, t._Obtain() );

		//	ID3DXSprite * pSp = 0;
		//	D3DXCreateSprite( _pD3DDevice, &pSp );

		//	//_pD3DDevice->SetTransform( D3DTS_PROJECTION, &mat );

		//	int cur_time = 0;
		//	while( ++cur_time < dwSeconds * 1000 )
		//	{
		//		static float angle = 0.0f;
		//		angle += 0.01f;
		//		_pD3DDevice->GetTransform( D3DTS_WORLD, &mat );
		//		D3DXMatrixRotationZ( &mat , angle );
		//		_pD3DDevice->SetTransform( D3DTS_WORLD, &mat );

		//		_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0, 0.0f, 0 );
		//		_pD3DDevice->BeginScene();
		//		pSp->Begin( 0 );
		//		pSp->Draw( t._Obtain(), 0, 0, 0, 0xffffffff );
		//		pSp->End();
		//		_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, &a, sizeof( a[0] ) );
		//		_pD3DDevice->EndScene();


		//		Present();
		//	}
		//}
	}
}