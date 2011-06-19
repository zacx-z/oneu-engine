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
//封装可变顶点格式的类
//目前支持标志位还不完善
#pragma once
#include <d3d9.h>
#include "D3DDefs.h"

namespace OneU
{
	namespace DX
	{
		//FVF标志定义
		enum FVF_FLAG
		{
			FVF_XYZ = D3DFVF_XYZ,
			FVF_XYZRHW = D3DFVF_XYZRHW,
			FVF_NORMAL = D3DFVF_NORMAL,
			FVF_PSIZE = D3DFVF_PSIZE,
			FVF_DIFFUSE = D3DFVF_DIFFUSE,
			FVF_SPECULAR = D3DFVF_SPECULAR,
			FVF_TEX1 = D3DFVF_TEX1
		};

		/* ----------------------------------------------------------------------------*/
		/*@cond INTERNAL_MPL*/
		/* ----------------------------------------------------------------------------*/
//内部使用宏定义
#define FVF_TEX FVF_TEX1
#define __FVF_LAST_FLAG 0x8000
#define __FVF_OVER_TEX 0x1000



		/////////////////////////////////
		//该模板类检测是否含在特定mask下有标志位
		template< uint32 flag_mask, uint32 flag >
		struct __FVF_Flag_Traits
		{
			enum{ bHas = ( flag_mask & flag ) ? 1 : 0 };
			typedef typename __Bool_Category< bHas >::Category Has_Category;
		};

		template< uint32 num_of_tex, uint32 flag >
		struct __FVF_Flag_Tex_Traits
		{
			enum{ Num = ( ( flag & D3DFVF_TEXCOUNT_MASK ) >> D3DFVF_TEXCOUNT_SHIFT ), bHas = Num >= num_of_tex };
			typedef typename __Bool_Category< bHas >::Category Has_Category;
		};
		///////////////////////////////////
		//计算特定mask表示的标志所表示的FVF成员的空间大小
		//////////////////////////////////
		template< uint32 flag_mask >
		struct __FVF_Mask_Traits
		{
			enum{ Size = 0 };
		};
		template<>
		struct __FVF_Mask_Traits< FVF_XYZ >
		{
			enum{ Size = 3 * sizeof( float ) };
		};
		template<>
		struct __FVF_Mask_Traits< FVF_XYZRHW >
		{
			enum{ Size = 4 * sizeof( float ) };
		};
		template<>
		struct __FVF_Mask_Traits< FVF_NORMAL >
		{
			enum{ Size = 3 * sizeof( float ) };
		};
		template<>
		struct __FVF_Mask_Traits< FVF_PSIZE >
		{
			enum{ Size = sizeof( float ) };
		};
		template<>
		struct __FVF_Mask_Traits< FVF_DIFFUSE >
		{
			enum{ Size = sizeof( D3DCOLOR ) };
		};
		template<>
		struct __FVF_Mask_Traits< FVF_SPECULAR >
		{
			enum{ Size = sizeof( D3DCOLOR ) };
		};
		template<>
		struct __FVF_Mask_Traits< FVF_TEX >
		{
		};
		/////////////////////////////////
		
		//用于递推找寻FVF成员的偏移地址的仿函数
		template< uint32 flag_mask , uint32 flag , int pos = 0 >
		class __FVF_FindValue
		{
		public:
			enum {
				_size = __FVF_Flag_Traits< flag_mask, flag >::bHas ? __FVF_Mask_Traits< flag_mask >::Size : 0,
				_offset = __FVF_FindValue< ( flag_mask >> 1 ), flag, pos >::_end,
				_end = _offset + _size
			};
		private:
			void * _Get( char * p, __True_Category )
			{
				return &p[ _offset + pos ];
			}
		public:
			void * operator() ( void * p )
			{
				return _Get(
					reinterpret_cast< char * >( p ),
					__FVF_Flag_Traits< flag_mask, flag >::Has_Category()
					);
			}
		};

		//对于纹理情况的特殊处理，偏特化版本
		//pos为偶数指向第pos / 2 + 1个纹理的U坐标
		//pos为奇数指向第pos / 2 + 1个纹理的V坐标
		template< uint32 flag, int pos >
		class __FVF_FindValue< FVF_TEX, flag, pos >
		{
		public:
			enum{ 
				_size =  __FVF_Flag_Tex_Traits< pos /2 + 1, flag >::Num * 2 * sizeof( float ),
				_offset = __FVF_FindValue< ( FVF_TEX >> 1 ), flag, pos >::_end,
				_end = _offset + _size
			};
		private:
			void * _Get( char * p, __True_Category )
			{
				return &p[ _offset + pos * 4 ];
			}
		public:
			void * operator() ( void * p )
			{
				return _Get(
					reinterpret_cast< char * >( p ),
					__FVF_Flag_Tex_Traits< pos / 2 + 1, flag >::Has_Category()
					);
			}
		};

		//对于在纹理mask后一个的mask的特殊处理
		template< uint32 flag, int pos >
		class __FVF_FindValue< __FVF_OVER_TEX, flag, pos >
		{
		public:
			enum{
				_size = __FVF_Flag_Traits< D3DFVF_LASTBETA_UBYTE4, flag >::bHas ? __FVF_Mask_Traits< D3DFVF_LASTBETA_UBYTE4 >::Size : 0,
				_offset =	__FVF_FindValue< ( FVF_TEX ), flag, pos >::_end,
				_end = _size + _offset
			};
		private:
			void * _Get( char * p, __True_Category )
			{
				return &p[ _offset + pos ];
			}
		public:
			void * operator() ( void * p )
			{
				return _Get(
					reinterpret_cast< char * >( p ),
					__FVF_Flag_Tex_Traits< pos / 2 + 1, flag >::Has_Category()
					);
			}
		};
		//////////////////

		//起始的找寻地址仿函数 递推终点
		template< uint32 flag, int pos >
		class __FVF_FindValue< 0, flag, pos >
		{
		public:
			enum{ _end = 0 };
		};

		/*@endcond*///INTERNAL_MPL
		/* ----------------------------------------------------------------------------*/

		
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 用户可变顶点
		 *
		 * @tparam flag FVF标志位
		 */
		/* ----------------------------------------------------------------------------*/
		template< uint32 flag >
		class VertexUP
		{
			
		public:
			enum{
				Flag = flag,
				Size = __FVF_FindValue< __FVF_LAST_FLAG, Flag >::_end
			};
		private:
			//懒得去弄每个const成员函数
			//这个只是内部问题
			mutable char buffer[ Size ];

			template < int pos >
			struct _FindXYZ
			{
				void * operator()( void * p_buffer, __True_Category )//3D情况下
				{
					return __FVF_FindValue< FVF_XYZ, Flag, pos >()( p_buffer );
				}
				void * operator()( void * p_buffer, __False_Category )//2D情况下
				{
					return __FVF_FindValue< FVF_XYZRHW, Flag, pos >()( p_buffer );
				}
			};
		public:

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 清零
			 *
			 * 将内部成员置零
			 */
			/* ----------------------------------------------------------------------------*/
			void Zero()
			{
				memset( buffer, 0, sizeof( buffer ) );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 初始化
			 *
			 * 所做的也仅仅是清零
			 */
			/* ----------------------------------------------------------------------------*/
			void Initialize()
			{
				Zero();
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief X坐标
			 * 
			 * @return X坐标浮点数的引用
			 */
			/* ----------------------------------------------------------------------------*/
			float & X()
			{
				return *reinterpret_cast< float * >(
					_FindXYZ< 0 >()( buffer, __FVF_Flag_Traits< FVF_XYZ, Flag >::Has_Category() )
					);
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获得X坐标
			 * 
			 * @return X坐标浮点数的引用
			 */
			/* ----------------------------------------------------------------------------*/
			const float & X() const
			{
				return *reinterpret_cast< const float * >(
					_FindXYZ< 0 >()( buffer, __FVF_Flag_Traits< FVF_XYZ, Flag >::Has_Category() )
					);
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief Y坐标
			 * 
			 * @return Y坐标浮点数的引用
			 */
			/* ----------------------------------------------------------------------------*/
			float & Y()
			{
				return *reinterpret_cast< float * >(
					_FindXYZ< sizeof( float ) >()( buffer, __FVF_Flag_Traits< FVF_XYZ, Flag >::Has_Category() )
					);
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获得Y坐标
			 * 
			 * @return Y坐标浮点数的引用
			 */
			/* ----------------------------------------------------------------------------*/
			const float & Y() const
			{
				return *reinterpret_cast< float * >(
					_FindXYZ< sizeof( float ) >()( buffer, __FVF_Flag_Traits< FVF_XYZ, Flag >::Has_Category() )
					);
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief Z坐标
			 * 
			 * @return Z坐标浮点数的引用
			 */
			/* ----------------------------------------------------------------------------*/
			float & Z()
			{
				return *reinterpret_cast< float * >(
					_FindXYZ< 2 * sizeof( float ) >()( buffer, __FVF_Flag_Traits< FVF_XYZ, Flag >::Has_Category() )
					);
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获得Z坐标
			 * 
			 * @return Z坐标浮点数的引用
			 */
			/* ----------------------------------------------------------------------------*/
			const float & Z() const
			{
				return *reinterpret_cast< float * >(
					_FindXYZ< 2 * sizeof( float ) >()( buffer, __FVF_Flag_Traits< FVF_XYZ, Flag >::Has_Category() )
					);
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获得Rhw
			 * 
			 * @return Rhw符点数的引用
			 */
			/* ----------------------------------------------------------------------------*/
			float & RhW()
			{
				return *reinterpret_cast< float * >(
					__FVF_FindValue< FVF_XYZRHW, Flag, 3 * sizeof( float ) >()( buffer )
					);
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获得Rhw
			 * 
			 * @return Rhw符点数的引用
			 */
			/* ----------------------------------------------------------------------------*/
			const float & RhW() const
			{
				return *reinterpret_cast< float * >(
					__FVF_FindValue< FVF_XYZRHW, Flag, 3 * sizeof( float ) >()( buffer )
					);
			}
			float & PSize()
			{
				return *reinterpret_cast< float * >(
					__FVF_FindValue< FVF_PSIZE, Flag >()( buffer )
					);
			}
			const float & PSize() const
			{
				return *reinterpret_cast< float * >(
					__FVF_FindValue< FVF_PSIZE, Flag >()( buffer )
					);
			}
			COLOR & Diffuse()
			{
				return *reinterpret_cast< COLOR * >(
					__FVF_FindValue< FVF_DIFFUSE, Flag >()( buffer )
					);
			}
			const COLOR & Diffuse() const
			{
				return *reinterpret_cast< COLOR * >(
					__FVF_FindValue< FVF_DIFFUSE, Flag >()( buffer )
					);
			}

			COLOR & Specular()
			{
				return *reinterpret_cast< COLOR * >(
					__FVF_FindValue< FVF_SPECULAR, Flag >()( buffer )
					);
			}
			const COLOR & Specular() const
			{
				return *reinterpret_cast< COLOR * >(
					__FVF_FindValue< FVF_SPECULAR, Flag >()( buffer )
					);
			}

			template< int index >
			float & U()
			{
				return *reinterpret_cast< float * >(
					__FVF_FindValue< FVF_TEX, Flag, index * 2 >()( buffer ) );
			}
			template< int index >
			const float & U() const
			{
				return *reinterpret_cast< float * >(
					__FVF_FindValue< FVF_TEX, Flag, index * 2 >()( buffer ) );
			}
			template< int index >
			float & V()
			{
				return *reinterpret_cast< float * >(
					__FVF_FindValue< FVF_TEX, Flag, index * 2 + 1 >()( buffer ) );
			}
			template< int index >
			const float & V() const
			{
				return *reinterpret_cast< float * >(
					__FVF_FindValue< FVF_TEX, Flag, index * 2 + 1 >()( buffer ) );
			}

			//便捷函数
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置3D位置
			 * 
			 * @param x 新的X坐标
			 * @param y 新的Y坐标
			 * @param z 新的Z坐标
			 */
			/* ----------------------------------------------------------------------------*/
			void SetPos( float x, float y, float z )
			{
				X() = x; Y() = y; Z() = z;
			}

			//便捷函数
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置2D位置
			 * 
			 * @param x 新的X坐标
			 * @param y 新的Y坐标
			 */
			/* ----------------------------------------------------------------------------*/
			void SetPos( float x, float y )
			{
				X() = x; Y() = y;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设置UV坐标
			 * 
			 * @tparam index 坐标索引数
			 * @param u 新的U坐标
			 * @param v 新的V坐标
			 */
			/* ----------------------------------------------------------------------------*/
			template< int index >
			void SetUV( const float u, const float v )
			{
				U< index >() = u;
				V< index >() = v;
			}
		};
	}
}
