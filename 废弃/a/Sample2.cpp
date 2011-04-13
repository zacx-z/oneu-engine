#include "WindowsEnc.h"
#include "GameLib.h"
// π”√CVector3
#include "Feng\FengDef.h"

namespace OneU
{
	using DX::Graphics;
	using DX::Input;
	DX::Texture tex;
	DX::Texture_RT RT;
	void OneUPreInitWin()
	{
		Onew_InitWin.iX = 0;
		Onew_InitWin.iY = 0;
		Onew_InitWin.iWidth = 800;
		Onew_InitWin.iHeight = 600;
		Onew_InitWin.bPopUp = false;
		Onew_InitWin.pWndName = L"Window Sample - Textures And Matrix";
	}
	void OneUInit()
	{
		Graphics.PreInit();

		Graphics.InitWindowed( 800, 600, GetWindow() );

		CreateTextureFromFile( L"A.jpg", tex, 1, DX::PXLFMT_X8R8G8B8 );
		RT.Create( 800, 600, DX::PXLFMT_X8R8G8B8 );

	}
	void OneUMain()
	{
		RT.GetSurfaceLevel( 0 ).StretchRect( NULL, &Graphics.GetRenderTarget(), NULL );
		Graphics.ClearTarget();
		{
			DX::Render rd;
			DX::SpriteE::I.Begin();
			DX::SpriteE::I.SetTransform( &matIMatrix );
			DX::SpriteE::I.Draw( &tex, NULL, NULL, NULL );
			MATRIX Matrix;

			MatrixTransformation2D( Matrix, &CVector2( 0, 0 ), 0, NULL, &CVector2( 100.0f, 100.0f ), 50, &CVector2( 0, 0 ) );

			DX::SpriteE::I.SetTransform( &Matrix );

			DX::SpriteE::I.Draw( &RT, NULL, &CVector3( 10, 10, 0 ), NULL, 0x9cffffff );
			DX::SpriteE::I.End();
		}
		Graphics.Present();
	}
	void OneUEnd()
	{

	}
}