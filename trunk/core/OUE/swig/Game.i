%module OUE

%include "types.i"
%include "Video.i"

%{
#include "../Game.h"
%}

namespace OneU
{
	class IGame
	{
	public:
		//virtual IBroadcast& getBroadcast(){ return *m_pBroadcast; }//reserved
		virtual OneU::IVideo& getVideo();
		virtual OneU::IStereo& getStereo();
		virtual OneU::IControl& getControl();
		virtual OneU::IScene& getScene();
		
		virtual OneU::IScene* replaceScene(OneU::IScene* newscene) = 0;

		virtual void init(OneU::pcwstr WindowName, OneU::uint32 width, OneU::uint32 height, bool bWindowed) = 0;
		virtual int run() = 0;
		virtual void quit() = 0;

		virtual void onFrame() = 0;
		virtual void onActiveWindow(bool bActive) = 0;

		virtual void setWindowTitle(OneU::pcwstr title) = 0;
		virtual OneU::vector2i_t getWindowPos() = 0;
		virtual void setWindowPos(const OneU::vector2i_t& newPos) = 0;
		virtual float getFPS() = 0;
		virtual float getTimeInterval() = 0;

		
		//virtual OneU::IInputReceiver* replaceInputFocus(OneU::IInputReceiver* pIR);
		//virtual void pushInputFocus(OneU::IInputReceiver* pIR);
		//virtual OneU::IInputReceiver* popInputFocus();
		//virtual void onChar(const CharEvent& event);
		//virtual void onKey(const KeyEvent& event);
		//virtual void onMouse(const MouseEvent& event);//reserved
		
		virtual void runShell(OneU::pcwstr command) = 0;
		virtual void output(OneU::pcwstr data) = 0;

		virtual void showInfo() = 0;
	};
	
	OneU::IGame& GetGame();
	
	//OneU::IBroadcast& GetBroadcast();//reserved
	OneU::IVideo& GetVideo();
	OneU::IStereo& GetStereo();
	OneU::IControl& GetControl();
	OneU::IScene& GetScene();
}


%inline %{
namespace OneU{
	void Game_build(){
		OneU::Game_build(Game_create);
	}
}
%}