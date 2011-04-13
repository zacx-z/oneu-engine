#include "ScriptScene.h"
#include "../WindowsEnc/WindowsEnc.h"
#include "../FrameSystem/Sprite.h"
#include "../LuaInterpreter/IprtException.h"

#include "../SPlugin/SPlugin.h"
#include "../FrameSystem/stdsp.h"
#include "../Utility/Utility.h"
namespace OneU
{
	ScriptScene::ScriptScene()
		: m_bValid(true)
	{
		spInit();
		spImportSTD();
		ShowCursor(FALSE);
		SetCurPaintGroup(GetRenderer()->GetLayer(0));

		m_Ip = new Lua::Interpreter;
		try
		{
			m_Ip->DoFile(".\\script\\lua\\main.lua");
		}
		catch(IptrException& e)
		{
			OneU::ErrorBox(e.GetString());
			m_bValid = false;
			delete m_Ip;
			spUninit();
			throw e;
		}
		catch(...)
		{
			OneU::ErrorBox(L"½Å±¾´íÎó");
			m_bValid = false;
			delete m_Ip;
			spUninit();
			throw;
		}
	}

	void ScriptScene::Update()
	{
		if(!m_bValid) return;
		try
		{
			m_Ip->Run("main()");
			spGC();
		}
		catch(IptrException& e)
		{
			OneU::ErrorBox(e.GetString());
			m_bValid = false;
			throw e;
		}
		catch(...)
		{
			OneU::ErrorBox(L"½Å±¾´íÎó");
			m_bValid = false;
			throw;
		}
	}

	ScriptScene::~ScriptScene()
	{
		try
		{
			m_Ip->Run("destroy()");
			delete m_Ip;
			spUninit();
		}
		catch(IptrException& e)
		{
			OneU::ErrorBox(e.GetString());
			m_bValid = false;
		}
		catch(...)
		{
			OneU::ErrorBox(L"½Å±¾´íÎó");
			m_bValid = false;
		}
	}

}