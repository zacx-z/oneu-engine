/*
This source file is part of OneU Engine.
Copyright (c) 2011 Ladace

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/**
 * @file Control.h
 * @brief 控制系统
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
#include "Vector.h"
/****************************************************************************
*
*      键盘扫描码（拷贝自dinput.h）
*
****************************************************************************/
#define OIK_ESCAPE          0x01
#define OIK_1               0x02
#define OIK_2               0x03
#define OIK_3               0x04
#define OIK_4               0x05
#define OIK_5               0x06
#define OIK_6               0x07
#define OIK_7               0x08
#define OIK_8               0x09
#define OIK_9               0x0A
#define OIK_0               0x0B
#define OIK_MINUS           0x0C    /* - on main keyboard */
#define OIK_EQUALS          0x0D
#define OIK_BACK            0x0E    /* backspace */
#define OIK_TAB             0x0F
#define OIK_Q               0x10
#define OIK_W               0x11
#define OIK_E               0x12
#define OIK_R               0x13
#define OIK_T               0x14
#define OIK_Y               0x15
#define OIK_U               0x16
#define OIK_I               0x17
#define OIK_O               0x18
#define OIK_P               0x19
#define OIK_LBRACKET        0x1A
#define OIK_RBRACKET        0x1B
#define OIK_RETURN          0x1C    /* Enter on main keyboard */
#define OIK_LCONTROL        0x1D
#define OIK_A               0x1E
#define OIK_S               0x1F
#define OIK_D               0x20
#define OIK_F               0x21
#define OIK_G               0x22
#define OIK_H               0x23
#define OIK_J               0x24
#define OIK_K               0x25
#define OIK_L               0x26
#define OIK_SEMICOLON       0x27
#define OIK_APOSTROPHE      0x28
#define OIK_GRAVE           0x29    /* accent grave */
#define OIK_LSHIFT          0x2A
#define OIK_BACKSLASH       0x2B
#define OIK_Z               0x2C
#define OIK_X               0x2D
#define OIK_C               0x2E
#define OIK_V               0x2F
#define OIK_B               0x30
#define OIK_N               0x31
#define OIK_M               0x32
#define OIK_COMMA           0x33
#define OIK_PERIOD          0x34    /* . on main keyboard */
#define OIK_SLASH           0x35    /* / on main keyboard */
#define OIK_RSHIFT          0x36
#define OIK_MULTIPLY        0x37    /* * on numeric keypad */
#define OIK_LMENU           0x38    /* left Alt */
#define OIK_SPACE           0x39
#define OIK_CAPITAL         0x3A
#define OIK_F1              0x3B
#define OIK_F2              0x3C
#define OIK_F3              0x3D
#define OIK_F4              0x3E
#define OIK_F5              0x3F
#define OIK_F6              0x40
#define OIK_F7              0x41
#define OIK_F8              0x42
#define OIK_F9              0x43
#define OIK_F10             0x44
#define OIK_NUMLOCK         0x45
#define OIK_SCROLL          0x46    /* Scroll Lock */
#define OIK_NUMPAD7         0x47
#define OIK_NUMPAD8         0x48
#define OIK_NUMPAD9         0x49
#define OIK_SUBTRACT        0x4A    /* - on numeric keypad */
#define OIK_NUMPAD4         0x4B
#define OIK_NUMPAD5         0x4C
#define OIK_NUMPAD6         0x4D
#define OIK_ADD             0x4E    /* + on numeric keypad */
#define OIK_NUMPAD1         0x4F
#define OIK_NUMPAD2         0x50
#define OIK_NUMPAD3         0x51
#define OIK_NUMPAD0         0x52
#define OIK_DECIMAL         0x53    /* . on numeric keypad */
#define OIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define OIK_F11             0x57
#define OIK_F12             0x58
#define OIK_F13             0x64    /*                     (NEC PC98) */
#define OIK_F14             0x65    /*                     (NEC PC98) */
#define OIK_F15             0x66    /*                     (NEC PC98) */
#define OIK_KANA            0x70    /* (Japanese keyboard)            */
#define OIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define OIK_CONVERT         0x79    /* (Japanese keyboard)            */
#define OIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define OIK_YEN             0x7D    /* (Japanese keyboard)            */
#define OIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define OIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define OIK_PREVTRACK       0x90    /* Previous Track (DOIK_CIRCUMFLEX on Japanese keyboard) */
#define OIK_AT              0x91    /*                     (NEC PC98) */
#define OIK_COLON           0x92    /*                     (NEC PC98) */
#define OIK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define OIK_KANJI           0x94    /* (Japanese keyboard)            */
#define OIK_STOP            0x95    /*                     (NEC PC98) */
#define OIK_AX              0x96    /*                     (Japan AX) */
#define OIK_UNLABELED       0x97    /*                        (J3100) */
#define OIK_NEXTTRACK       0x99    /* Next Track */
#define OIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define OIK_RCONTROL        0x9D
#define OIK_MUTE            0xA0    /* Mute */
#define OIK_CALCULATOR      0xA1    /* Calculator */
#define OIK_PLAYPAUSE       0xA2    /* Play / Pause */
#define OIK_MEDIASTOP       0xA4    /* Media Stop */
#define OIK_VOLUMEDOWN      0xAE    /* Volume - */
#define OIK_VOLUMEUP        0xB0    /* Volume + */
#define OIK_WEBHOME         0xB2    /* Web home */
#define OIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define OIK_DIVIDE          0xB5    /* / on numeric keypad */
#define OIK_SYSRQ           0xB7
#define OIK_RMENU           0xB8    /* right Alt */
#define OIK_PAUSE           0xC5    /* Pause */
#define OIK_HOME            0xC7    /* Home on arrow keypad */
#define OIK_UP              0xC8    /* UpArrow on arrow keypad */
#define OIK_PRIOR           0xC9    /* PgUp on arrow keypad */
#define OIK_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define OIK_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define OIK_END             0xCF    /* End on arrow keypad */
#define OIK_DOWN            0xD0    /* DownArrow on arrow keypad */
#define OIK_NEXT            0xD1    /* PgDn on arrow keypad */
#define OIK_INSERT          0xD2    /* Insert on arrow keypad */
#define OIK_DELETE          0xD3    /* Delete on arrow keypad */
#define OIK_LWIN            0xDB    /* Left Windows key */
#define OIK_RWIN            0xDC    /* Right Windows key */
#define OIK_APPS            0xDD    /* AppMenu key */
#define OIK_POWER           0xDE    /* System Power */
#define OIK_SLEEP           0xDF    /* System Sleep */
#define OIK_WAKE            0xE3    /* System Wake */
#define OIK_WEBSEARCH       0xE5    /* Web Search */
#define OIK_WEBFAVORITES    0xE6    /* Web Favorites */
#define OIK_WEBREFRESH      0xE7    /* Web Refresh */
#define OIK_WEBSTOP         0xE8    /* Web Stop */
#define OIK_WEBFORWARD      0xE9    /* Web Forward */
#define OIK_WEBBACK         0xEA    /* Web Back */
#define OIK_MYCOMPUTER      0xEB    /* My Computer */
#define OIK_MAIL            0xEC    /* Mail */
#define OIK_MEDIASELECT     0xED    /* Media Select */

/*
*  Alternate names for keys, to facilitate transition from DOS.
*/
#define OIK_BACKSPACE       OIK_BACK            /* backspace */
#define OIK_NUMPADSTAR      OIK_MULTIPLY        /* * on numeric keypad */
#define OIK_LALT            OIK_LMENU           /* left Alt */
#define OIK_CAPSLOCK        OIK_CAPITAL         /* CapsLock */
#define OIK_NUMPADMINUS     OIK_SUBTRACT        /* - on numeric keypad */
#define OIK_NUMPADPLUS      OIK_ADD             /* + on numeric keypad */
#define OIK_NUMPADPERIOD    OIK_DECIMAL         /* . on numeric keypad */
#define OIK_NUMPADSLASH     OIK_DIVIDE          /* / on numeric keypad */
#define OIK_RALT            OIK_RMENU           /* right Alt */
#define OIK_UPARROW         OIK_UP              /* UpArrow on arrow keypad */
#define OIK_PGUP            OIK_PRIOR           /* PgUp on arrow keypad */
#define OIK_LEFTARROW       OIK_LEFT            /* LeftArrow on arrow keypad */
#define OIK_RIGHTARROW      OIK_RIGHT           /* RightArrow on arrow keypad */
#define OIK_DOWNARROW       OIK_DOWN            /* DownArrow on arrow keypad */
#define OIK_PGDN            OIK_NEXT            /* PgDn on arrow keypad */

/*
*  Alternate names for keys originally not used on US keyboards.
*/
#define OIK_CIRCUMFLEX      OIK_PREVTRACK       /* Japanese keyboard */

namespace OneU
{
	/**
	 * @addtogroup group_game
	 */
	//@{
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 控制系统接口
	 *
	 * 负责键盘和鼠标的输入控制。
	 */
	/* ----------------------------------------------------------------------------*/
	class IControl
		: public Interface
	{
	public:
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 更新
		 *
		 * 重新读取鼠标和键盘信息。每一帧需更新。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void update() = 0;
		//keyboard functions
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 按键按下状态
		 * 
		 * @param scancode 键盘扫描码
		 * 
		 * @returns 该键是否在按下状态
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool keyIsDown(uint32 scancode) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 键盘按下
		 *
		 * @param scancode 键盘扫描码
		 * 
		 * @returns 该键是否在这一帧中被按下
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool keyPress(uint32 scancode) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 键盘释放
		 * 
		 * @param scancode 键盘扫描码
		 * 
		 * @returns 该键是否在这一帧中被释放
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool keyRelease(uint32 scancode) = 0;

		//mouse functions
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 鼠标按钮按下状态
		 * 
		 * @param ButtonID 鼠标按钮ID
		 * 
		 * @returns 按钮是否在按下状态
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool buttonIsDown(uint32 ButtonID) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 鼠标按钮按下
		 * 
		 * @param ButtonID 鼠标按钮ID
		 * 
		 * @returns 按钮是否在这一帧中被按下
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool buttonRelease(uint32 ButtonID) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 鼠标按钮释放
		 * 
		 * @param ButtonID 鼠标按钮ID
		 * 
		 * @returns 按钮是否在这一帧中被释放
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool buttonPress(uint32 ButtonID) = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 鼠标偏移
		 * 
		 * @returns 在上一次更新之间的鼠标移动向量
		 */
		/* ----------------------------------------------------------------------------*/
		virtual vector2i_t mouseOffset() = 0;
	};

	//implementation in game.cpp
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 控制系统工厂
	 * 
	 * @returns 控制系统接口
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API IControl* Control_create();
	//@}
}
