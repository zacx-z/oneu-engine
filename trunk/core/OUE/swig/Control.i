%module OUE
%include "types.i"

%{
#include "../Control.h"
%}

namespace OneU
{
	class IControl
	{
	public:
		virtual bool keyIsDown(OneU::uint32 scancode) = 0;
		virtual bool keyPress(OneU::uint32 scancode) = 0;
		virtual bool keyRelease(OneU::uint32 scancode) = 0;

		virtual bool buttonIsDown(OneU::uint32 ButtonID) = 0;
		virtual bool buttonRelease(OneU::uint32 ButtonID) = 0;
		
		virtual bool buttonPress(OneU::uint32 ButtonID) = 0;
		virtual OneU::vector2i_t mouseOffset() = 0;
		%extend{
		static const uint32 OIK_ESCAPE = 0x01;
		static const uint32 OIK_1 = 0x02;
		static const uint32 OIK_2 = 0x03;
		static const uint32 OIK_3 = 0x04;
		static const uint32 OIK_4 = 0x0;
		static const uint32 OIK_5 = 0x06;
		static const uint32 OIK_6 = 0x07;
		static const uint32 OIK_7 = 0x08;
		static const uint32 OIK_8 = 0x09;
		static const uint32 OIK_9 = 0x0A;
		static const uint32 OIK_0 = 0x0B;
		static const uint32 OIK_MINUS = 0x0C;
		static const uint32 OIK_EQUALS = 0x0D;
		static const uint32 OIK_BACK = 0x0E;
		static const uint32 OIK_TAB = 0x0F;
		static const uint32 OIK_Q = 0x10;
		static const uint32 OIK_W = 0x11;
		static const uint32 OIK_E = 0x12;
		static const uint32 OIK_R = 0x13;
		static const uint32 OIK_T = 0x14;
		static const uint32 OIK_Y = 0x15;
		static const uint32 OIK_U = 0x16;
		static const uint32 OIK_I = 0x17;
		static const uint32 OIK_O = 0x18;
		static const uint32 OIK_P = 0x19;
		static const uint32 OIK_LBRACKET = 0x1A;
		static const uint32 OIK_RBRACKET = 0x1B;
		static const uint32 OIK_RETURN = 0x1C;
		static const uint32 OIK_LCONTROL = 0x1D;
		static const uint32 OIK_A = 0x1E;
		static const uint32 OIK_S = 0x1F;
		static const uint32 OIK_D = 0x20;
		static const uint32 OIK_F = 0x21;
		static const uint32 OIK_G = 0x22;
		static const uint32 OIK_H = 0x23;
		static const uint32 OIK_J = 0x24;
		static const uint32 OIK_K = 0x25;
		static const uint32 OIK_L = 0x26;
		static const uint32 OIK_SEMICOLON = 0x27;
		static const uint32 OIK_APOSTROPHE = 0x28;
		static const uint32 OIK_GRAVE = 0x29;
		static const uint32 OIK_LSHIFT = 0x2A;
		static const uint32 OIK_BACKSLASH = 0x2B;
		static const uint32 OIK_Z = 0x2C;
		static const uint32 OIK_X = 0x2D;
		static const uint32 OIK_C = 0x2E;
		static const uint32 OIK_V = 0x2F;
		static const uint32 OIK_B = 0x30;
		static const uint32 OIK_N = 0x31;
		static const uint32 OIK_M = 0x32;
		static const uint32 OIK_COMMA = 0x33;
		static const uint32 OIK_PERIOD = 0x34;
		static const uint32 OIK_SLASH = 0x35;
		static const uint32 OIK_RSHIFT = 0x36;
		static const uint32 OIK_MULTIPLY = 0x37;
		static const uint32 OIK_LMENU = 0x38;
		static const uint32 OIK_SPACE = 0x39;
		static const uint32 OIK_CAPITAL = 0x3A;
		static const uint32 OIK_F1 = 0x3B;
		static const uint32 OIK_F2 = 0x3C;
		static const uint32 OIK_F3 = 0x3D;
		static const uint32 OIK_F4 = 0x3E;
		static const uint32 OIK_F5 = 0x3F;
		static const uint32 OIK_F6 = 0x40;
		static const uint32 OIK_F7 = 0x41;
		static const uint32 OIK_F8 = 0x42;
		static const uint32 OIK_F9 = 0x43;
		static const uint32 OIK_F10 = 0x44;
		static const uint32 OIK_NUMLOCK = 0x45;
		static const uint32 OIK_SCROLL = 0x46;
		static const uint32 OIK_NUMPAD7 = 0x47;
		static const uint32 OIK_NUMPAD8 = 0x48;
		static const uint32 OIK_NUMPAD9 = 0x49;
		static const uint32 OIK_SUBTRACT = 0x4A;
		static const uint32 OIK_NUMPAD4 = 0x4B;
		static const uint32 OIK_NUMPAD5 = 0x4C;
		static const uint32 OIK_NUMPAD6 = 0x4D;
		static const uint32 OIK_ADD = 0x4E;
		static const uint32 OIK_NUMPAD1 = 0x4F;
		static const uint32 OIK_NUMPAD2 = 0x50;
		static const uint32 OIK_NUMPAD3 = 0x51;
		static const uint32 OIK_NUMPAD0 = 0x52;
		static const uint32 OIK_DECIMAL = 0x53;
		static const uint32 OIK_OEM_102 = 0x56;
		static const uint32 OIK_F11 = 0x57;
		static const uint32 OIK_F12 = 0x58;
		static const uint32 OIK_F13 = 0x64;
		static const uint32 OIK_F14 = 0x65;
		static const uint32 OIK_F15 = 0x66;
		static const uint32 OIK_KANA = 0x70;
		static const uint32 OIK_ABNT_C1 = 0x73;
		static const uint32 OIK_CONVERT = 0x79;
		static const uint32 OIK_NOCONVERT = 0x7B;
		static const uint32 OIK_YEN = 0x7D;
		static const uint32 OIK_ABNT_C2 = 0x7E;
		static const uint32 OIK_NUMPADEQUALS = 0x8D;
		static const uint32 OIK_PREVTRACK = 0x90;
		static const uint32 OIK_AT = 0x91;
		static const uint32 OIK_COLON = 0x92;
		static const uint32 OIK_UNDERLINE = 0x93;
		static const uint32 OIK_KANJI = 0x94;
		static const uint32 OIK_STOP = 0x95;
		static const uint32 OIK_AX = 0x96;
		static const uint32 OIK_UNLABELED = 0x97;
		static const uint32 OIK_NEXTTRACK = 0x99;
		static const uint32 OIK_NUMPADENTER = 0x9C;
		static const uint32 OIK_RCONTROL = 0x9D;
		static const uint32 OIK_MUTE = 0xA0;
		static const uint32 OIK_CALCULATOR = 0xA1;
		static const uint32 OIK_PLAYPAUSE = 0xA2;
		static const uint32 OIK_MEDIASTOP = 0xA4;
		static const uint32 OIK_VOLUMEDOWN = 0xAE;
		static const uint32 OIK_VOLUMEUP = 0xB0;
		static const uint32 OIK_WEBHOME = 0xB2;
		static const uint32 OIK_NUMPADCOMMA = 0xB3;
		static const uint32 OIK_DIVIDE = 0xB5;
		static const uint32 OIK_SYSRQ = 0xB7;
		static const uint32 OIK_RMENU = 0xB8;
		static const uint32 OIK_PAUSE = 0xC5;
		static const uint32 OIK_HOME = 0xC7;
		static const uint32 OIK_UP = 0xC8;
		static const uint32 OIK_PRIOR = 0xC9;
		static const uint32 OIK_LEFT = 0xCB;
		static const uint32 OIK_RIGHT = 0xCD;
		static const uint32 OIK_END = 0xCF;
		static const uint32 OIK_DOWN = 0xD0;
		static const uint32 OIK_NEXT = 0xD1;
		static const uint32 OIK_INSERT = 0xD2;
		static const uint32 OIK_DELETE = 0xD3;
		static const uint32 OIK_LWIN = 0xDB;
		static const uint32 OIK_RWIN = 0xDC;
		static const uint32 OIK_APPS = 0xDD;
		static const uint32 OIK_POWER = 0xDE;
		static const uint32 OIK_SLEEP = 0xDF;
		static const uint32 OIK_WAKE = 0xE3;
		static const uint32 OIK_WEBSEARCH = 0xE5;
		static const uint32 OIK_WEBFAVORITES = 0xE6;
		static const uint32 OIK_WEBREFRESH = 0xE7;
		static const uint32 OIK_WEBSTOP = 0xE8;
		static const uint32 OIK_WEBFORWARD = 0xE9;
		static const uint32 OIK_WEBBACK = 0xEA;
		static const uint32 OIK_MYCOMPUTER = 0xEB;
		static const uint32 OIK_MAIL = 0xEC;
		static const uint32 OIK_MEDIASELECT = 0xED;

		/*
		*  Alternate names for keys, to facilitate transition from DOS.
		*/
		static const uint32 OIK_BACKSPACE = OIK_BACK;
		static const uint32 OIK_NUMPADSTAR = OIK_MULTIPLY;
		static const uint32 OIK_LALT = OIK_LMENU;
		static const uint32 OIK_CAPSLOCK = OIK_CAPITAL;
		static const uint32 OIK_NUMPADMINUS = OIK_SUBTRACT;
		static const uint32 OIK_NUMPADPLUS = OIK_ADD;
		static const uint32 OIK_NUMPADPERIOD = OIK_DECIMAL;
		static const uint32 OIK_NUMPADSLASH = OIK_DIVIDE;
		static const uint32 OIK_RALT = OIK_RMENU;
		static const uint32 OIK_UPARROW = OIK_UP;
		static const uint32 OIK_PGUP = OIK_PRIOR;
		static const uint32 OIK_LEFTARROW = OIK_LEFT;
		static const uint32 OIK_RIGHTARROW = OIK_RIGHT;
		static const uint32 OIK_DOWNARROW = OIK_DOWN;
		static const uint32 OIK_PGDN = OIK_NEXT;

		/*
		*  Alternate names for keys originally not used on US keyboards.
		*/
		static const uint32 OIK_CIRCUMFLEX = OIK_PREVTRACK;
		}
	};
}
