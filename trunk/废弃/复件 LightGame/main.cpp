#include "../FrameSystem/Game.h"
#include "ScriptScene.h"
//轻量级游戏自始至终只有一个Scene
namespace OneU
{
	class MyGame
		: public Game
	{
	public:
		void InitScene(Scene*& pScene);
	};
	void MyGame::InitScene(Scene*& pScene)
	{
		pScene = new ScriptScene;
	}
	//在里面创建整个游戏
	Game* GameFactroy()
	{
		return new MyGame();
	}
}