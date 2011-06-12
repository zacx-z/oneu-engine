//单场景脚本驱动的游戏
#include "../OUE/OUE.h"

using namespace OneU;

static IInterpreter* s_I = NULL;

class SingleScene
	: public IScene
{
public:
	SingleScene(){
	}
	void begin(){
		s_I->execFile(L"script/main.lua");
	}
	void update(){
		s_I->execCode("mainloop()");
	}
};

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ::LPSTR lpCmdLine, int nShowCmd){
	Game_build(Game_create);

	s_I = Interpreter_Lua();
	s_I->execFile(L"script/startup.lua");

	IGame& game = GetGame();

	SingleScene* s = ONEU_NEW SingleScene;
	game.replaceScene(s);
	s->begin();
	return game.run();
}
