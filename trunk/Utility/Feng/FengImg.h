#pragma once
#include "..\..\DXGraphics\SurTex\STLock.h"
#include "Polygon.h"
#include "FengDef.h"

namespace OneU
{
	namespace Feng
	{
		//绝对值 需要在GetImageBound
		template< typename T >
		inline T _abs( T v )
		{
			return v >= 0 ? v : -v;
		}

		//获得图像边界
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 获得图像边界
		 *
		 * 获得图像边界，并以多边形的形式存在polygon里。边界方向为逆时针。
		 *
		 * @tparam Format 像素格式
		 * @param Texture 保存图像的纹理
		 * @param Level 纹理的一个级别，将以这个级别的表面来计算边界。
		 * @param prc 图像所在的矩形，若为NULL则将全部纹理都作为图像。
		 * @param polygon 最后输出的多边形
		 * @param minAlpha 认为像素存在的最小透明度，设为0即无效。
		 * @param crKeyColor 关键色，默认为0
		 */
		/* ----------------------------------------------------------------------------*/
		template< DX::PXLFORMAT Format, class Contents >
		void GetImageBound( DX::Texture_Lk& Texture, uint Level, const RECT *prc, Polygon_t< Contents, MultiPolygon_Impl >& polygon, dword minAlpha, COLOR crKeyColor = 0 )
		{
			static COLOR crKey;
			static dword min;
			crKey = crKeyColor;
			min = minAlpha;
			struct ColorFunctor
			{
				bool operator()( COLOR crColor )
				{
					if( crColor == crKey )
						return FALSE;
					if( GetAlpha( crColor ) < min )
						return FALSE;
					return TRUE;
				}
			};

			DX::SurfaceLock< Format > lk( &Texture.GetSurfaceLevel(Level), prc );

			_GetImageBound< ColorFunctor >( lk, polygon );
		}

		/*@cond INTERNAL_MPL*/
		template< class ColorFunctor, class T, class Contents >
		void _GetImageBound( T& Lock, Polygon_t< Contents, MultiPolygon_Impl >& polygon )
		{
			const int MAX_SIZE = 5000;
			const float PRECISION = 50.0f;
			struct _Struct
			{
				_Struct()
					: top( 0 )
				{}
				struct 
				{
					struct
					{
						uint left, right;
					};
				} v[ MAX_SIZE ];
				int a;//哨兵 访问Line[-1]会访问到该值 设为0
				int Line[ 1500 ];
				uint top;
			} obj;
			obj.a = 0;
			for( uint j = 0; j < Lock.GetLockHeight(); j++ )
			{
				int k = -1;//工作段下标 -1表示无效
				for( uint i = 0; i < Lock.GetLockWidth(); i++ )//将X方向做内循环是一个好习惯，让Lock的读写连续可能会提高效率
				{
					if( ColorFunctor()( Lock.GetPixel( i, j ) ) )
					{
						if( k != -1 )
						{
							obj.v[ k ].right = i;
						}
						else
						{
							obj.v[ obj.top ].left = obj.v[ obj.top ].right = i;
							k = obj.top;
							obj.top++;
						}
					}
					else
						k = -1;
				}
				obj.Line[ j ] = obj.top;
				ASSERT( obj.top < MAX_SIZE - 1000 );
			}


			
			Lock.Unlock();
			//将后文GetLockHeight函数调用改成一常量
			//优化用的数字
			float weight = 0.0f;
			bool b[ MAX_SIZE * 2 ] = {};
			polygon.Clear( 0 );

			int i = 0, y = 0;
			for( ; y < Lock.GetLockHeight(); y++ )
				for( ; i < obj.Line[y]; i++ )
				{
					if( !b[i] )
						goto end;
				}
end:
			while( i != obj.top )
			{
				int dir = 1;//方向 1表示左侧 向下进行 0表示右侧 向上进行
				while( TRUE ) 
				{
					//向下进行的情况下
					if( dir == 1 )
					{
						//终结条件
						if( b[i] )goto nextseg;

						b[i] = true;
						if( polygon.PolyEnd( polygon.NoP() - 1 ) - polygon.PolyBegin( polygon.NoP() - 1 ) >= 2 )
						{
							//优化顶点
							float area = ( polygon.P( polygon.Size() - 2 ) >> polygon.P( polygon.Size() - 1 ) )
								% ( polygon.P( polygon.Size() - 1 ) >> CPoint2( ( float )obj.v[i].left, ( float )y + 0.5f ) );
							if( _abs( area + weight ) <= PRECISION
								)//近似判定有困难，不算太好。保留
							{
								weight += area;
								polygon.X( polygon.Size() - 1 ) = ( float )obj.v[i].left + 1.0f;
								polygon.Y( polygon.Size() - 1 ) = ( float )y + 0.5f;
								goto noPush1;
							}
						}
						weight = 0;
						polygon.PushBack( ( float )obj.v[i].left + 1.0f, ( float )y + 0.5f );//坐标修正 y轴上有一定误差 保留
noPush1:
						if( y + 1 == Lock.GetLockHeight() )
						{
							//转移到右侧 向上进行 i,y不变
							dir = 0;
							continue;
						}

						uint oldy = y;
						for( int k = obj.Line[y]; k < obj.Line[y + 1]; k++ )
						{
							if( obj.v[k].right >= obj.v[i].left && obj.v[k].left <= obj.v[i].right )
							{
								if(((i - 1) >= obj.Line[y - 1]) && (obj.v[i - 1].right >= obj.v[k].left))
								{
									i = i - 1;
									//y不变
									dir = 0;
									break;
								}
								else
								{
									y++;
									i = k;
									//dir不变
									break;
								}
							}
						}
						if( k == obj.Line[oldy + 1])
							//转移到右侧
							dir = 0;
					}
					//向上进行的情况下
					else
					{
						//终结条件
						if( b[i + MAX_SIZE] )goto nextseg;
						b[i + MAX_SIZE] = true;

						if( polygon.PolyEnd( polygon.NoP() - 1 ) - polygon.PolyBegin( polygon.NoP() - 1 ) >= 2 )
						{	
							float area = ( polygon.P( polygon.Size() - 2 ) >> polygon.P( polygon.Size() - 1 ) )
								% ( polygon.P( polygon.Size() - 1 ) >> CPoint2( ( float )obj.v[i].right, ( float )y + 0.5f ) );
							if( _abs(area + weight) <= PRECISION
								)//保留
							{
								weight += area;
								polygon.X( polygon.Size() - 1 ) = ( float )obj.v[i].right + 1.0f;
								polygon.Y( polygon.Size() - 1 ) = ( float )y + 0.5f;
								goto noPush2;
							}
						}
						weight = 0.0f;
						polygon.PushBack( ( float )obj.v[i].right, ( float )y + 0.5f );//坐标修正 保留
noPush2:
						if( y == 0 )
						{
							//转移到左侧 向下进行 i,y不变
							dir = 1;
							continue;
						}

						uint oldy = y;

						for( int k = obj.Line[y - 1] - 1; k >= obj.Line[y - 2]; k-- )
						{
							if( obj.v[k].left <= obj.v[i].right && obj.v[k].right >= obj.v[i].left )
							{
								if( i + 1 < obj.Line[y] && obj.v[i + 1].left <= obj.v[k].right )
								{
									i = i + 1;
									//y不变
									dir = 1;
									break;
								}
								else
								{
									y--;
									i = k;
									//dir不变
									break;
								}
							}
						}

						if( k == obj.Line[oldy - 2] - 1 )
							dir = 1;

					}

				}
				//while循环结束

nextseg:

				i = 0; y = 0;
				for( ; y < Lock.GetLockHeight(); y++ )
					for( ; i < obj.Line[y]; i++ )
					{
						if( !b[i] )
							goto end2;
					}
end2:

				if( i != obj.top )
					polygon.NewPoly( 0 );
			}//while
		}
		/*@endcond*//* INTERNAL_MPL*/
	}//namespace Feng
}//namespace OneU
