#include "..\DXGraphics\Include.h"
#include "../DXInput/DXInput.h"
#include "..\Utility\Feng\Painter.h"
#include "..\Utility\Feng\Drawer.h"
#include "..\Utility\Feng\FengMen.h"
#include "..\Utility\Feng\Polygon.h"
#include "..\Utility\Resource\Graphics\Image.h"
#include "..\Utility\Resource\Graphics\GraphicsSaver.h"
#include "..\Utility\Resource\File\OPackFile.h"
#include "..\Utility\Resource\File\IFile.h"
#include "../Utility/List.h"
#include "../Utility/Feng/FengImg.h"
#include <list>
#include "..\Utility\Matrix.h"
namespace OneU
{
	using namespace Lib;
	struct MySprite
	{
		SImage img;
		float x,y;
	};
	//该变量可能会后于GraphicsResMgr_cls析构
	//OneU里调用clear
	MySprite plane;
	List< CRect > rlist;
	KeyBoard KB;
	KeyBoard::State KB_State;
	MPolygon SpPol;
	float hp;
	float ay;
	float score;
	FontE g_f;
	enum {GAMING, GAMEOVER} status;
	using DX::Graphics;
	void NewGame();
	void ActiveWindow( BOOL ){}
	void OneUPreInitWin()
	{
		OneU_InitWin.iX = 0;
		OneU_InitWin.iY = 0;
		OneU_InitWin.iWidth = 800;
		OneU_InitWin.iHeight = 600;
		OneU_InitWin.bPopUp = false;
		OneU_InitWin.pWndName = L"Plane Game Sample";
	}
	void OneUInit()
	{
		//初始化图形系统
		Graphics.PreInit();

		DX::DisplayMode DM;

		DM.Format = DX::PXLFMT_X8R8G8B8;
		DM.Width = 800;
		DM.Height = 600;
		DM.RefreshRate = 60;

		Graphics.InitWindowed( 800, 600, GetWindow() );

		//初始化操作系统
		Input.Init();
		KB.Create();
		KB.Acquire();

		//随机种子
		srand(GetTickCount());
		//初始化游戏资源
		g_f.Create(20, 10, 1);
		plane.img = CreateImage( L"plane.png" );
		Texture_Lk tex;
		CreateTextureFromFile(L"plane.png", tex, 1, PXLFMT_A8R8G8B8);

		Feng::GetImageBound<Lib::PXLFMT_A8R8G8B8>(tex, 0, NULL, SpPol, 50);
		NewGame();
	}
	void NewGame()
	{
		hp = 100.0f;
		plane.x = 120.0f;
		plane.y = 300.0f;
		status = GAMING;
		ay = 0;
		score = 0;
		rlist.clear();
	}
	void OneUMain()
	{
		KB.Read(KB_State);
		switch(status)
		{
		case GAMING:
			{
				score += 10;
				ay += 0.2;
				if(KB_State.GetState(IK_UP)) ay -= 0.5;
				if(KB_State.GetState(IK_DOWN)) ay += 0.5;
				plane.y += ay;
				static int frame = 0;frame++;
				if(frame % 60 == 0)
				{
					float a = ((float)rand()) / RAND_MAX * 400, b = ((float)rand()) / RAND_MAX * ((600 - a) > 300 ? 100 :(400 - a)) + 200;
					rlist.push_back(CRect(800, a, 820, a + b));
				}
				static MPolygon tp;
				tp = SpPol;
				tp << Translate2(plane.x,plane.y);

				if(CLine2(0,0,800,0) ^ tp || CLine2(0, 600, 800, 600) ^ tp) hp = -1.0f;
				for(List<CRect>::iterator iter = rlist.begin(); iter != rlist.end();)
				{
					(*iter) << Translate2(-5, 0);
					if(Cross((Polygon(*iter)), tp))
						hp = -1;//die immediately when hit
						//hp -= 1;
					if(iter->right < 0)
						rlist.erase(iter++);
					else ++iter;
				}
				if(hp <= 0.0f) status = GAMEOVER;
			}
			break;
		case GAMEOVER:
			{
				if(KB_State.GetState(IK_R)) NewGame();
			}
			break;
		}
		try
		{
			Graphics.ClearTarget();
			{
				Lib::RenderManip rd;
				{
					Feng::ScreenPainter painter( rd );
					painter.Paint(plane.img.Get(), CVector2(plane.x, plane.y));
				}
				{
					Feng::ScreenDrawer drawer(rd);
					for(List<CRect>::iterator iter = rlist.begin(); iter != rlist.end(); ++iter)
						drawer.Fill(*iter, 0xffffffff);
				}
				wchar_t buf[100];
				//swprintf(buf, L"hp:%f", hp);//if you want to use hp
				//g_f.DrawText(buf, &CRect_n(0,20,200,100), 0xffffffff);
				swprintf(buf, L"score:%f", score);
				g_f.DrawText(buf, &CRect_n(0,0,200,100), 0xffffffff);
				if(status == GAMEOVER)
					g_f.DrawText(L"    Game Over\nPress R to restart", &CRect_n(300, 300, 500, 400), 0xffff0000);
			}
			Graphics.Present();
		}
		catch(DXLostDevice)
		{
			GraphicsResMgr.UnloadAll();
			Graphics.Reset();
			GraphicsResMgr.ReloadAll();
		}
	}
	void OneUEnd()
	{
	}
}