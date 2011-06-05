//单场景脚本驱动的游戏
#include "../OUE/OUE.h"

using namespace OneU;

class SingleScene
	: public IScene
{
	atom::IInterpreter* m_Ip;
	IShape* shape;
public:
	SingleScene(){
		shape = Shape_rect(rect(-50, -50, 100, 100));
		shape->setX(100);
		getRenderScene()->addChild(shape);
		m_Ip = GetAtom().createInterpreter(IAtom::L_LUA);
		m_Ip->execFile(L"./script/lua/main.lua");

		m_Ip->execWithScene(L"Scene:init()", this);

		GetStereo().playMusic(L"06-my_chemical_romance-the_only_hope_for_me_is_you.mp3");
		GetStereo().playSound(L"04-my_chemical_romance-sing.mp3");
	}
	void update(){
		static int frame = 0;
		++frame;
		if(frame == 100)
			GetStereo().playMusic(L"04-my_chemical_romance-sing.mp3");
		m_Ip->execWithScene(L"Scene:main()", this);
	}
	~SingleScene(){
		m_Ip->execWithScene(L"Scene:destroy()", this);
		ONEU_DELETE m_Ip;
	}
};

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ::LPSTR lpCmdLine, int nShowCmd){
	Game_build(Game_create);
	IGame& game = GetGame();
	game.init(L"OneU Engine", 800, 600, true);

	game.replaceScene(ONEU_NEW SingleScene);
	return game.run();
}
