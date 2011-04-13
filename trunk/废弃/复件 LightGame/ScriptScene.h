#pragma once
#include "../FrameSystem/Scene.h"
#include "../LuaInterpreter/Interpreter.h"

namespace OneU
{
	class ScriptScene
		: public Scene
	{
		Lua::Interpreter* m_Ip;
		bool m_bValid;
	public:
		ScriptScene();
		~ScriptScene();
		void Update();
	};
}