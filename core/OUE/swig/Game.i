%module(directors = "1") OUE

%include "types.i"
%include "Video.i"
%include "Scene.i"

%{
#include "../Game.h"
%}

namespace OneU
{
	%rename(Game) IGame;
	class IGame
	{
	public:
		//virtual IBroadcast& getBroadcast(){ return *m_pBroadcast; }//reserved
		virtual OneU::IVideo& getVideo();
		virtual OneU::IStereo& getStereo();
		virtual OneU::IControl& getControl();
		virtual OneU::IScene& getScene();
	
		virtual void init(OneU::pcwstr WindowName, uint32 width, uint32 height, bool bWindowed) = 0;
		virtual int run() = 0;
		virtual void quit() = 0;

		virtual void onFrame() = 0;
		virtual void onActiveWindow(bool bActive) = 0;

		virtual void setWindowTitle(OneU::pcwstr title) = 0;
		virtual OneU::vector2i_t getWindowPos() = 0;
		virtual void setWindowPos(const OneU::vector2i_t& newPos) = 0;
		virtual void showCursor(bool bShow) = 0;
		virtual float getFPS() = 0;
		virtual float getTimeInterval() = 0;
			
#ifdef SUPPORT_DIRECTORS
		%extend {
		virtual void replaceScene(Scene* scene){
			if(scene){ scene->__isActive = true; scene->__game = self; }
			OneU::IScene* ret = self->replaceScene(scene);
			if(!ret) return;
			Scene* s = dynamic_cast<Scene*>(ret);
			if(!s) rb_raise(rb_eRuntimeError, "Doesn't allow scene not created by script.");
			s->__isActive = false;
		}
		}
		virtual void onChar(const OneU::CharEvent& event);
		virtual void onKey(const OneU::KeyEvent& event);
		virtual void onMouse(const OneU::MouseEvent& event);	
#endif
		
		virtual void runShell(OneU::pcwstr command) = 0;
		virtual void output(OneU::pcwstr data) = 0;

		virtual void showInfo() = 0;
	};
	
	void Aux_GameInit(OneU::pcwstr WindowName, OneU::uint32 width, OneU::uint32 height, bool bWindowed);
	
	OneU::IGame& GetGame();
	
	//OneU::IBroadcast& GetBroadcast();//reserved
	OneU::IVideo& GetVideo();
	OneU::IStereo& GetStereo();
	OneU::IControl& GetControl();
	OneU::IScene& GetScene();
}