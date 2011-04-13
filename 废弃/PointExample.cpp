
#include "WindowsEnc.h"
#include "GameLib.h"
#include <list>
//使用CVector3
#include "Feng\FengDef.h"
#include "Feng\Feng.h"
#include "Feng\FengMen.h"
#include "Feng\Screen.h"
#include "Feng\Polygon.h"

#include "Matrix.h"

using namespace OneU;

BOOL g_bActive = TRUE;
const int WIDTH = 800, HEIGHT = 600;
struct MAN
{
	float x,y;
	float hp;
} man = { 400.0f, 300.0f, 100.0f };
int Score = 0;

inline float random( float left, float right )
{
	return ( rand() * ( right - left ) ) / RAND_MAX + left;
}

void Update( float delta_time );
void UpdateMission( float delta_time );

enum STATUS{ BEGIN, MISSION, GAMEOVER } Status = BEGIN;
struct EnemyType
{
	typedef Polygon_t< VertexCt< DX::FVF_DIFFUSE | DX::FVF_XYZRHW > > PolygonType;
	PolygonType plg;
	float vx,vy,rx;
};
std::list< EnemyType > Ene;

namespace OneU
{
	using DX::Graphics;
	using DX::Input;

	DX::Mouse mouse;
	DX::FontE font;

	DX::Texture_RT RT,MotionBlur;

	void OneUPreInitWin()
	{
		OneU_InitWin.iX = 0;
		OneU_InitWin.iY = 0;
		OneU_InitWin.iWidth = 800;
		OneU_InitWin.iHeight = 600;
		OneU_InitWin.bPopUp = false;
		OneU_InitWin.pWndName = L"真的很SB一个东西~……";
	}
	void OneUInit()
	{
		Graphics.PreInit();

		DX::DisplayMode DM;

		DM.Format = DX::PXLFMT_X8R8G8B8;
		DM.Width = WIDTH;
		DM.Height = HEIGHT;
		DM.RefreshRate = 60;

		//Graphics.InitFullScreen( &DM, GetWindow() );

		Graphics.InitWindowed( WIDTH, HEIGHT, GetWindow() );

		RT.Create( 300, 100, DX::PXLFMT_A8R8G8B8 );
		MotionBlur.Create( WIDTH, HEIGHT, DX::PXLFMT_X8R8G8B8 );
		//MotionBlur.GetSurfaceLevel( 0 ).ColorFill( 0xff000000 );
		//原来 原后备缓冲跟前表面是不大一样的
		Graphics.ClearTarget( 0xff000000 );
		Graphics.Present();
		//Graphics.ClearTarget( 0xff000000 );
		
		//Graphics.Present();

		DX::FontE f1,f2;f1.Create( 70, 30, 1 );f2.Create( 70, 30, 30 );
		DX::Surface_RenderTarget old = Graphics.GetRenderTarget();
		Graphics.SetRenderTarget( RT.GetSurfaceLevel( 0 ) );
		Graphics.ClearTarget();
		{
			DX::Render rd;
			f2.DrawText( L"START", NULL, 0xffff2222 );
			f1.DrawText( L"START", &CRect_n( 2, 2, 300, 100 ), 0xffffff22 );
		}
		Graphics.SetRenderTarget( old );

		Input.Init();

		mouse.Create();
		mouse.Acquire();

		Feng::Screen::SetWorkScreen( 800, 600 );
		Feng::Screen::SetDeviceScreen( WIDTH, HEIGHT );

		srand( GetTime() );

		font.Create( 15, 7, 3 );

		ShowCursor( FALSE );

	}
	//后来会变成ProcessFrame
	void OneUMain()
	{
		if( !g_bActive ) return;
		static int Fps = 0;
		{
			static int Local = 0;
			static unsigned int LastTime = GetTime();
			Local++;
			if( GetTime() - LastTime >= 1000 )
			{
				Fps = Local;
				Local = 0;
				LastTime += 1000;
			}
		}
		if( Status == MISSION )
			Update( 1.0f );

		{MotionBlur.GetSurfaceLevel( 0 ).StretchRect( NULL, &Graphics.GetRenderTarget(), NULL );}
		Graphics.ClearTarget();
		{
			DX::Render rd;
			wchar_t dest[100];
			wsprintf( dest, L"FPS:%d", Fps );
			DX::FontE tempf;tempf.Create( 10, 7, 2 );tempf.DrawText( dest, &CRect_n( 0, 0, 200, 200 ), 0xffffffff );
			tempf.DrawText( L"Ladace", &CRect_n( 740, 580, 800, 600 ), 0xffffffff );

			if( Status == MISSION || Status == GAMEOVER )
			{
				//画分数
				wsprintf( dest, L"SCORE:%d", Score );
				tempf.DrawText( dest, &CRect_n( 0, 12, 200, 200 ), 0xffffffff );

				CPoint2 point;
				point.x = man.x;
				point.y = man.y;
				Feng::Drawer drawer( rd );
				drawer.SetWidth( 5.0f );
				DX::TStage( 0 ).DisableTexture();
				drawer.Draw( point, 0xff00ff00 );

				Graphics.SetFVF( DX::FVF_XYZRHW | DX::FVF_DIFFUSE );
				for( std::list< EnemyType >::iterator i = Ene.begin(); i != Ene.end(); ++i )
				{
					rd.RenderVertexUP( DX::Render::PT_TRIANGLEFAN, i->plg.Data(), 2 );
					drawer.Draw( i->plg, 0xffffffff );
				}

				CRect rc;
				rc.left = 0;rc.right = 300;rc.top = 553;rc.bottom = 600;
				font.DrawText( L"HP:", &CRect_n( rc ), 0xffffffff );
	
				rc.left = 40;
				rc.right = 40 + man.hp;
				rc.top = 555;
				rc.bottom = 565;
				drawer.Fill( rc, 0xffff0000 );
				drawer.SetWidth( 1 );
				rc.right = 140;
				drawer.Draw( rc, 0xff00ff00 );
			}
			{
				//Graphics.SetProjectionTransform( &matIMatrix );
				//Graphics.SetCullMode( 0 );
				//Feng::Paint( rd, RT, &CRect( 0, 1, 1, 0 ) );
				Feng::ScreenPainter pntr( rd );
				pntr.Paint( RT, CMatrix::E );
				if( Status == BEGIN )
				{
					static float scl = 1.0f;
					scl += 0.01f;
					DWORD Alpha = scl > 1.2f ? 255 + 1.2f * 640 - scl * 640 : 255;
		
					if( scl >= 1.59f ) Status = MISSION;
					pntr.Paint( RT, Translate2( -150, -50 ) * Scale2( scl, 1.0f ) * Translate2( 440, 300 )
																		, NULL, SetARGB( Alpha, 0xff, 0xff, 0xff ) );
						
				}
				if( Status == GAMEOVER )
				{
					DX::FontE tf;
					tf.Create( 70, 30, 1 );
					tf.DrawText( L"GAME OVER", &CRect_n( 215, 300, 800, 600 ), 0xffff0000 );
					wsprintf( dest, L"YOUR SCORE IS %d", Score );
					tempf.DrawText( dest, &CRect_n( 320, 380, 800, 600 ), 0xffffffff );
				}
				
				pntr.Paint( MotionBlur, CVector2( 0, 0 ), NULL, 0x9fffffff );
			}
		}
		Graphics.Present();
	}

	void ActiveWindow( BOOL bActive )
	{
		g_bActive = bActive;
	}
	void OneUEnd()
	{
	}
}

void Update( float delta_time )
{
	UpdateMission( delta_time );
}
void UpdateMission( float delta_time )
{
	//
	Score++;

	DX::Mouse::State ms;
	mouse.Read( ms );
	man.x += ms.GetX() / 2.0f;
	man.y += ms.GetY() / 2.0f;


	static int time = 0;
	time++;
	if( time == 15000000 ) time = 0;
	if( time % 3 == 0 )
	{
		Ene.push_front( EnemyType() );

		EnemyType &e = *Ene.begin();

		float b = random( 0.0f, 2.0f ), val = random( 0.0f, 600.0f );
		int x = b > 1.5f ? 800 : ( b > 1.0f ? -100 : val / 600.0f * 800.0f ),
			y = b < 0.5f ? 600 : ( b < 1.0f ? -50 : val );

		CRect rc;
		rc.left = x; rc.top = y; rc.right = x + 100; rc.bottom = y + 50;

		e.plg = rc;
		e.vx = random( -10.0f, 10.0f );e.vy = random( -10.0f, 10.0f );

		e.rx = random( -0.1f, 0.1f );

		DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE >* pV = e.plg.Data();
		for( int i = 0; i < e.plg.Size(); i++ )
		{
			pV[i].RhW() = 1.0f;
			pV[i].Diffuse() = 0xaaffff00;
		}
	}

	//if( time % 15 == 0 )
	//{
	//	Ene.pop_back();
	//}
	for( std::list< EnemyType >::iterator i = Ene.begin(); i != Ene.end(); ++i )
	{
beginloop:
		using Feng::Screen;
		using Feng::floatX;
		using Feng::floatY;
		using Feng::STRETCH;
		i->plg<< Rotate2c( ( ( i->plg[0].X( 0 ) + i->plg[0].X( 2 ) ) / 2 ), ( ( i->plg[0].Y( 0 ) + i->plg[0].Y( 2 ) ) / 2 ), i->rx * delta_time );
		i->plg<< Translate2( ( i->vx ), ( i->vy ) );
		EnemyType::PolygonType& pol = i->plg;
		if( pol.X( 0 ) >= 1000.0f || pol.X( 0 ) <= -100.0f ||pol.Y( 0 ) <= -100.0f ||pol.Y( 0 ) >= 700.0f )
		{
			std::list< EnemyType >::iterator j = i;
			++i;
			Ene.erase( j );
			if( i == Ene.end() ) break;
			goto beginloop;
		}
		//i->vx += ( man.x - pol.X( 0 ) ) / 400.0f;
		//if( i->vx > 1.0f ) i->vx = 1.0f;
		//if( i->vx < -1.0f ) i->vx = -1.0f;
		//i->vy += ( man.y - pol.Y( 0 ) ) / 400.0f;
		//if( i->vy > 1.0f ) i->vy = 1.0f;
		//if( i->vy < -1.0f ) i->vy = -1.0f;

		CPoint2 p;
		p.x = man.x;p.y = man.y;
		if( p ^ i->plg )
		{
			man.hp -= 1.0f;
			if( man.hp < 0.0f )
				man.hp = 0.0f;
		}
	}
	if( man.hp <= 0.000001f ) Status = GAMEOVER;
}