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
//处理像素格式
//可能有笔误
#include< d3d9types.h >

namespace OneU
{
	typedef unsigned __int8 Pixel8;

	typedef unsigned __int16 Pixel16;

	typedef struct _Pixel24
	{
		unsigned char c[3];
		_Pixel24( unsigned long rhs )
		{
			char * p = reinterpret_cast< char * > ( &rhs );
			c[ 0 ] = p[ 0 ];
			c[ 1 ] = p[ 1 ];
			c[ 2 ] = p[ 2 ];
		}
		operator unsigned long ()
		{
			return c[ 0 ] | ( c[ 1 ] << 8 ) | ( c[ 2 ] << 16 );
		}
	} Pixel24;

	typedef unsigned __int32 Pixel32;

	typedef unsigned long COLOR;//ARGB格式

	inline BYTE GetAlpha( COLOR Color )
	{
		return BYTE(( Color & 0xff000000 ) >> 24);
	}
	
	inline BYTE GetRed( COLOR Color )
	{
		return BYTE(( Color & 0xff0000 ) >> 16);
	}

	inline BYTE GetGreen( COLOR Color )
	{
		return BYTE(( Color & 0xff00 ) >> 8);
	}

	inline BYTE GetBlue( COLOR Color )
	{
		return BYTE(( Color & 0xff ));
	}

	inline uint32 SetRGB( uint32 Red, uint32 Green, uint32 Blue )
	{
		return 0xff000000 | ( static_cast< BYTE >( Red ) << 16 ) | ( static_cast< BYTE >( Green ) << 8 ) | static_cast< BYTE > ( Blue );
	}
	inline uint32 SetARGB( uint32 Alpha, uint32 Red, uint32 Green, uint32 Blue )
	{
		return ( static_cast< BYTE >( Alpha ) << 24 ) | ( static_cast< BYTE >( Red ) << 16 ) | ( static_cast< BYTE >( Green ) << 8 ) | static_cast< BYTE > ( Blue );
	}

	

	namespace DX
	{
		//@todo 可能未来会扩充深度部分 （长期）
		typedef enum _PXLFORMAT
		{
			PXLFMT_UNKNOWN = D3DFMT_UNKNOWN,

			PXLFMT_R8G8B8 = D3DFMT_R8G8B8,
			PXLFMT_A8R8G8B8 = D3DFMT_A8R8G8B8,
			PXLFMT_X8R8G8B8 = D3DFMT_X8R8G8B8,
			PXLFMT_R5G6B5 = D3DFMT_R5G6B5,
			PXLFMT_X1R5G5B5 = D3DFMT_X1R5G5B5,
			PXLFMT_A1R5G5B5 = D3DFMT_A1R5G5B5,
			PXLFMT_A4R4G4B4 = D3DFMT_A4R4G4B4,
			PXLFMT_R3G3B2 = D3DFMT_R3G3B2,
			PXLFMT_A8 = D3DFMT_A8,
			PXLFMT_A8R3G3B2 = D3DFMT_A8R3G3B2,
			PXLFMT_X4R4G4B4 = D3DFMT_X4R4G4B4,

			PXLFMT_D16_LOCKABLE = D3DFMT_D16_LOCKABLE,
			PXLFMT_D32 = D3DFMT_D32,
			PXLFMT_D15S1 = D3DFMT_D15S1,
			PXLFMT_D24S8 = D3DFMT_D24S8,
			PXLFMT_D16 = D3DFMT_D16,
			PXLFMT_D24X8 = D3DFMT_D24X8,
			PXLFMT_D24X4S4 = D3DFMT_D24X4S4
		} PXLFORMAT;

		//默认像素格式
		const PXLFORMAT DEFAULT_FORMAT = PXLFMT_A8R8G8B8;

		/* ----------------------------------------------------------------------------*/
		/*@cond INTERNAL_MPL*/
		/* ----------------------------------------------------------------------------*/
		template < uint32 Format >
		struct __PixelFormat_Traits
		{
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_R8G8B8 >
		{
			enum{
				ABits = 0,
				RBits = 8,
				GBits = 8,
				BBits = 8,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel24 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_A8R8G8B8 >
		{
			enum{
				ABits = 8,
				RBits = 8,
				GBits = 8,
				BBits = 8,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel32 Pixel;
		};

		template <>
		struct __PixelFormat_Traits< PXLFMT_X8R8G8B8 >
		{
			enum{
				ABits = 0,
				RBits = 8,
				GBits = 8,
				BBits = 8,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel32 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_R5G6B5 >
		{
			enum{
				ABits = 0,
				RBits = 5,
				GBits = 6,
				BBits = 5,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel16 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_X1R5G5B5 >
		{
			enum{
				ABits = 0,
				RBits = 5,
				GBits = 5,
				BBits = 5,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel16 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_A1R5G5B5 >
		{
			enum{
				ABits = 1,
				RBits = 5,
				GBits = 5,
				BBits = 5,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel16 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_A4R4G4B4 >
		{
			enum{
				ABits = 4,
				RBits = 4,
				GBits = 4,
				BBits = 4,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel16 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_R3G3B2 >
		{
			enum{
				ABits = 0,
				RBits = 3,
				GBits = 3,
				BBits = 2,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel8 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_A8 >
		{
			enum{
				ABits = 8,
				RBits = 0,
				GBits = 0,
				BBits = 0,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel8 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_A8R3G3B2 >
		{
			enum{
				ABits = 8,
				RBits = 3,
				GBits = 3,
				BBits = 2,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel16 Pixel;
		};
		template <>
		struct __PixelFormat_Traits< PXLFMT_X4R4G4B4 >
		{
			enum{
				ABits = 0,
				RBits = 4,
				GBits = 4,
				BBits = 4,
				AShift = ABits ? ( RBits + GBits + BBits ) : 0,
				RShift = BBits + GBits,
				GShift = BBits,
				BShift = 0
			};
			typedef Pixel16 Pixel;
		};
		/*@endcond*///INTERNAL_MPL
		/* ----------------------------------------------------------------------------*/

		template < uint32 Format >
		struct PixelFormat
		{
			enum{
				ABits = __PixelFormat_Traits< Format >::ABits,
				RBits = __PixelFormat_Traits< Format >::RBits,
				GBits = __PixelFormat_Traits< Format >::GBits,
				BBits = __PixelFormat_Traits< Format >::BBits,
				AShift = __PixelFormat_Traits< Format >::AShift,
				RShift = __PixelFormat_Traits< Format >::RShift,
				GShift = __PixelFormat_Traits< Format >::GShift,
				BShift = __PixelFormat_Traits< Format >::BShift,
			};
			typedef typename __PixelFormat_Traits< Format >::Pixel Pixel;

			static Pixel GetPixel( COLOR RGB )
			{
				COLOR Alpha = RGB >> 24;
				COLOR Red = ( RGB & 0xff0000 ) >> 16;
				COLOR Green = ( RGB & 0xff00 ) >> 8;
				COLOR Blue = ( RGB & 0xff );

				Alpha >>= 8 - ABits;
				Red >>= 8 - RBits;
				Green >>= 8 - GBits;
				Blue >>= 8 - BBits;

				return ( Alpha << AShift ) | ( Red << RShift ) | ( Green << GShift ) | ( Blue << BShift );
			}
			static COLOR GetColor( Pixel RGB )
			{
				BYTE t[4];

				//与字节顺序 即主机顺序有关
				t[ 3 ] = RGB >> AShift;
				t[ 2 ] = RGB >> RShift;
				t[ 1 ] = RGB >> GShift;
				t[ 0 ] = RGB >> BShift;

				t[ 3 ] <<= 8 - ABits;
				t[ 2 ] <<= 8 - RBits;
				t[ 1 ] <<= 8 - GBits;
				t[ 0 ] <<= 8 - BBits;

				return ( * reinterpret_cast< COLOR * > ( t ) );//与COLOR的定义紧耦合
			}
		};
		//如果是PXLFMT_UNKNOWN则什么成员和函数也没有
		template<>
		struct PixelFormat< PXLFMT_UNKNOWN >
		{
		};
	}
}