#include <OUE/Game.h>
#include <OUE/Video.h>
#include <OUE/Sprite.h>
#include <OUE/Scene.h>
#include <OUE/Control.h>

#pragma comment(lib, "OUE.lib")
#pragma comment(lib, "Base.lib")

using namespace OneU;

class BeginScene
	: public IScene
{
	ISprite* s;
public:
	BeginScene(){
		image_t a = GetVideo().loadImage(L"picture.png");

		s = Sprite_create(a);
		getRenderScene()->addChild(s);
		s->setCenterX(0.5f);
		s->setCenterY(0.5f);
	}
	void update(){
		if(s){
			s->setRotation(s->getRotation() + 1.0f);
			s->setScaleX(s->getScaleX() + 0.01f);

			if(GetControl().keyIsDown(OIK_RIGHT))
				s->setX(s->getX() + 10.0f);
			if(GetControl().keyIsDown(OIK_LEFT))
				s->setX(s->getX() - 10.0f);
			if(GetControl().keyIsDown(OIK_UP))
				s->setY(s->getY() - 10.0f);
			if(GetControl().keyIsDown(OIK_DOWN))
				s->setY(s->getY() + 10.0f);
		}
	}
};

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ::LPSTR lpCmdLine, int nShowCmd){
	Game_build(Game_create);
	IGame* game = GetGame();
	game->init(L"example", 800, 600, true);

	game->replaceScene(ONEU_NEW(BeginScene));
	return game->run();
}
