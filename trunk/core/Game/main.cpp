//单场景脚本驱动的游戏
#include "../OUE/OUE.h"

using namespace OneU;

class SingleScene
	: public IScene
{
	IInterpreter* m_I;
public:
	SingleScene(){
		m_I = Interpreter_Lua();
	}
	void begin(){
		m_I->execFile(L"script/main.lua");
	}
	void update(){
		m_I->execCode("mainloop()");
	}
};

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ::LPSTR lpCmdLine, int nShowCmd){
	Game_build(Game_create);
	IGame& game = GetGame();
	game.init(L"OneU Engine", 800, 600, true);

	SingleScene* s = ONEU_NEW SingleScene;
	game.replaceScene(s);
	s->begin();
	return game.run();
}
