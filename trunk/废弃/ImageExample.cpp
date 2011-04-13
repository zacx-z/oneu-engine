#include "..\DXGraphics\Include.h"
#include "..\Utility\Feng\Painter.h"
#include "..\Utility\Resource\Graphics\Image.h"
#include "..\Utility\Resource\Graphics\GraphicsSaver.h"
#include "..\Utility\Resource\File\OPackFile.h"
#include "..\Utility\Resource\File\IFile.h"
#include <list>
#include "..\Utility\Matrix.h"
namespace OneU
{
	struct MySprite
	{
		SImage img;
		float x,y;
	};
	//该变量可能会后于GraphicsResMgr_cls析构
	//OneU里调用clear
	std::list< MySprite > sl;
	using DX::Graphics;
	void ActiveWindow( BOOL ){}
	void OneUPreInitWin()
	{
		OneU_InitWin.iX = 0;
		OneU_InitWin.iY = 0;
		OneU_InitWin.iWidth = 800;
		OneU_InitWin.iHeight = 600;
		OneU_InitWin.bPopUp = false;
		OneU_InitWin.pWndName = L"Using Image Source Sample";
	}
	void OneUInit()
	{
		Graphics.PreInit();

		DX::DisplayMode DM;

		DM.Format = DX::PXLFMT_X8R8G8B8;
		DM.Width = 800;
		DM.Height = 600;
		DM.RefreshRate = 60;

		Graphics.InitWindowed( 800, 600, GetWindow() );

		{
			IFile f1( L"SF Res\\Enemy\\Enemy1.png" ), f2( L"SF Res\\Enemy\\Enemy2.png" );
			OPackFile fout( L"SF Res\\t.odt" );
			fout.Put( f1.ReadToBuffer(), L"Enemy1.png" );
			fout.Put( f2.ReadToBuffer(), L"Enemy23.png" );	
		}

		PackFileSystem_Cls::Manip manip;

		SImage a1 = CreateImage( L"SF Res\\t.odt->Enemy1.png" ),
			a2 = CreateImage( L"SF Res\\t.odt->Enemy23.png" ),
			a3 = CreateImage( L"SF Res\\Enemy\\Enemy3.png"),
			a4 = CreateImage( L"SF Res\\Enemy\\Enemy4.png"),
			a5 = CreateImage( L"SF Res\\Enemy\\Enemy5.png" );
		MySprite s1 = { a1, 0, 0 },
			s2 = { a2, 100, 100 },
			s3 = { a3, 100, 300 },
			s4 = { a4, 200, 100 },
			s5 = { a5, 300, 0 };
		sl.push_back( s1 );
		sl.push_back( s2 );
		sl.push_back( s3 );
		sl.push_back( s4 );
		sl.push_back( s5 );
	}
	void OneUMain()
	{
		try
		{
			Graphics.ClearTarget();
			{
				Lib::RenderManip rd;
				Feng::ScreenPainter painter( rd );
				for( std::list< MySprite >::iterator iter = sl.begin(); iter != sl.end(); ++iter )
					painter.Paint( iter->img.Get(), CVector2( iter->x, iter->y ) );
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
		sl.clear();
	}
}