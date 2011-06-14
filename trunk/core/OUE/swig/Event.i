%module(directors = "1") OUE
%{
#include "../Event.h"
%}

#ifdef SUPPORT_DIRECTORS

namespace OneU
{
	%nodefaultctor CharEvent;
	%nodefaultctor KeyEvent;
	%nodefaultctor MouseEvent;
	class CharEvent
	{
	public:
		wchar getCode() const;
	};
	class KeyEvent
	{
	public:
		uint32 getCode();
		bool isRelease();
		bool isPress();
	};

	class MouseEvent
	{
	public:
		uint32 getButtonID();
		bool isRelease();
		bool isPress();
	};

	%director IInputReceiver;
	%rename(InputReceiver) IInputReceiver;
	class IInputReceiver
	{
	public:
		virtual ~IInputReceiver();
		virtual bool onChar(const OneU::CharEvent& event){ return true; }
		virtual bool onKey(const OneU::KeyEvent& event){ return true; }
		virtual bool onMouse(const OneU::MouseEvent& event){ return true; }
	};
}

#endif