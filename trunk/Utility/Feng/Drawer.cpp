#include "FengAlgo.h"
#include "Drawer.h"
#include "..\..\DXGraphics\VertexUP.h"
#include "..\..\DXGraphics\TStage.h"

namespace OneU
{
	namespace Feng
	{
		//默认线条宽度值

		float ScreenDrawer::DefaultWidth = 2.0f;
		int ScreenDrawer::ClassNum = 0;

		void ScreenDrawer::Draw( const CPoint2 &p, COLOR crColor )
		{
			using DX::LineE;

			static DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE | DX::FVF_PSIZE > v;

			v.SetPos( p.x, p.y );
			v.RhW() = 1.0;
			v.Diffuse() = crColor;
			v.PSize() = m_Width;

			//设定矩阵变换
			if(m_UseMatrix) DX::Graphics.SetWorldTransform( &m_Mat );
			//else DX::Graphics.SetWorldTransform( NULL );

			DX::Graphics.SetFVF( &v );
			m_Rd.RenderVertexUP( DX::RenderManip::PT_POINTLIST, &v, 1 );
		}

		void ScreenDrawer::Draw( const CLine2 &line, COLOR crColor )
		{
			using DX::LineE;
			if(m_UseMatrix)
			{
				CLine2 l = line;
				l << m_Mat;
				LineE::I.Draw(reinterpret_cast<const CPoint2*>(&l), 2, crColor);
			}
			else
				LineE::I.Draw(reinterpret_cast<const CPoint2*>(&line), 2, crColor);

		}

		void ScreenDrawer::Draw( const CRect &rc, COLOR crColor )
		{
			using DX::LineE;
			if(!m_UseMatrix)
			{
				static CPoint2 point[5];				
				point[0] = CPoint2( ( float )rc.left, ( float )rc.top );
				point[1] = CPoint2( ( float )rc.left, ( float )rc.bottom );
				point[2] = CPoint2( ( float )rc.right,( float )rc.bottom );
				point[3] = CPoint2( ( float )rc.right,( float )rc.top );
				point[4] = CPoint2( ( float )rc.left, ( float )rc.top );

				LineE::I.Draw( point, 5, crColor );
			}
			else
			{
				static CPoint2 point[5];
				point[0] = CPoint2( ( float )rc.left, ( float )rc.top ) << m_Mat;
				point[1] = CPoint2( ( float )rc.left, ( float )rc.bottom ) << m_Mat;
				point[2] = CPoint2( ( float )rc.right,( float )rc.bottom ) << m_Mat;
				point[3] = CPoint2( ( float )rc.right,( float )rc.top ) << m_Mat;
				point[4] = CPoint2( ( float )rc.left, ( float )rc.top ) << m_Mat;
				
				LineE::I.Draw(point, 5, crColor);
			}

		}

		void ScreenDrawer::Fill( const CRect &rc, COLOR crColor )
		{
			static DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE > v[4];
			v[0].RhW() = v[1].RhW() = v[2].RhW() = v[3].RhW() = 1.0f;
			v[0].Z() = v[1].Z() = v[2].Z() = v[3].Z() = 0.0f;
			v[0].X() = rc.left; v[0].Y() = rc.top;    v[0].Diffuse() = crColor;
			v[1].X() = rc.left; v[1].Y() = rc.bottom; v[1].Diffuse() = crColor;
			v[2].X() = rc.right;v[2].Y() = rc.bottom; v[2].Diffuse() = crColor;
			v[3].X() = rc.right;v[3].Y() = rc.top;    v[3].Diffuse() = crColor;

			DX::Graphics.SetFVF( v );
			using DX::TStage;
			TStage( 0 ).DisableTexture();

			//设定矩阵变换
			if( m_UseMatrix ) DX::Graphics.SetWorldTransform( &m_Mat );
			//else DX::Graphics.SetWorldTransform( NULL );

			m_Rd.RenderVertexUP( DX::RenderManip::PT_TRIANGLEFAN, v, 2 );
		}
	}
}