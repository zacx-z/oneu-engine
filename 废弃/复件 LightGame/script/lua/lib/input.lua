require "spext"

local old_env = splugin.bind_env(splugin.get("input"))

input = {}
spFuncP(input,
	{"key_release", "key_press", "key_isdown",
	 "get_mouse_lx", "get_mouse_ly",
	 "lbutton_isdown", "lbutton_release", "lbutton_press",
	 "rbutton_isdown", "rbutton_release", "rbutton_press"})

local fuck = function(v, name)
	input[name] = splugin.get(name)
end

	fuck(IK_0, "IK_0");
	fuck(IK_1, "IK_1");
	fuck(IK_2, "IK_2");
	fuck(IK_3, "IK_3");
	fuck(IK_4, "IK_4");
	fuck(IK_5, "IK_5");
	fuck(IK_6, "IK_6");
	fuck(IK_7, "IK_7");
	fuck(IK_8, "IK_8");
	fuck(IK_9, "IK_9");
	fuck(IK_A, "IK_A");
	fuck(IK_ABNT_C1, "IK_ABNT_C1");
	fuck(IK_ABNT_C2, "IK_ABNT_C2");
	fuck(IK_ADD, "IK_ADD");
	fuck(IK_APOSTROPHE, "IK_APOSTROPHE");
	fuck(IK_APPS, "IK_APPS");
	fuck(IK_AT, "IK_AT");
	fuck(IK_AX, "IK_AX");
	fuck(IK_B, "IK_B");
	fuck(IK_BACK, "IK_BACK");
	fuck(IK_BACKSLASH, "IK_BACKSLASH");
	fuck(IK_C, "IK_C");
	fuck(IK_CALCULATOR, "IK_CALCULATOR");
	fuck(IK_CAPITAL, "IK_CAPITAL");
	fuck(IK_COLON, "IK_COLON");
	fuck(IK_COMMA, "IK_COMMA");
	fuck(IK_CONVERT, "IK_CONVERT");
	fuck(IK_D, "IK_D");
	fuck(IK_DECIMAL, "IK_DECIMAL");
	fuck(IK_DELETE, "IK_DELETE");
	fuck(IK_DIVIDE, "IK_DIVIDE");
	fuck(IK_DOWN, "IK_DOWN");
	fuck(IK_E, "IK_E");
	fuck(IK_END, "IK_END");
	fuck(IK_EQUALS, "IK_EQUALS");
	fuck(IK_ESCAPE, "IK_ESCAPE");
	fuck(IK_F, "IK_F");
	fuck(IK_F1, "IK_F1");
	fuck(IK_F2, "IK_F2");
	fuck(IK_F3, "IK_F3");
	fuck(IK_F4, "IK_F4");
	fuck(IK_F5, "IK_F5");
	fuck(IK_F6, "IK_F6");
	fuck(IK_F7, "IK_F7");
	fuck(IK_F8, "IK_F8");
	fuck(IK_F9, "IK_F9");
	fuck(IK_F10, "IK_F10");
	fuck(IK_F11, "IK_F11");
	fuck(IK_F12, "IK_F12");
	fuck(IK_F13, "IK_F13");
	fuck(IK_F14, "IK_F14");
	fuck(IK_F15, "IK_F15");
	fuck(IK_G, "IK_G");
	fuck(IK_GRAVE, "IK_GRAVE");
	fuck(IK_H, "IK_H");
	fuck(IK_HOME, "IK_HOME");
	fuck(IK_I, "IK_I");
	fuck(IK_INSERT, "IK_INSERT");
	fuck(IK_J, "IK_J");
	fuck(IK_K, "IK_K");
	fuck(IK_KANA, "IK_KANA");
	fuck(IK_KANJI, "IK_KANJI");
	fuck(IK_L, "IK_L");
	fuck(IK_LBRACKET, "IK_LBRACKET");
	fuck(IK_LCONTROL, "IK_LCONTROL");
	fuck(IK_LEFT, "IK_LEFT");
	fuck(IK_LMENU, "IK_LMENU");
	fuck(IK_LSHIFT, "IK_LSHIFT");
	fuck(IK_LWIN, "IK_LWIN");
	fuck(IK_M, "IK_M");
	fuck(IK_MAIL, "IK_MAIL");
	fuck(IK_MEDIASELECT, "IK_MEDIASELECT");
	fuck(IK_MEDIASTOP, "IK_MEDIASTOP");
	fuck(IK_MINUS, "IK_MINUS");
	fuck(IK_MULTIPLY, "IK_MULTIPLY");
	fuck(IK_MUTE, "IK_MUTE");
	fuck(IK_MYCOMPUTER, "IK_MYCOMPUTER");
	fuck(IK_N, "IK_N");
	fuck(IK_NEXT, "IK_NEXT");
	fuck(IK_NEXTTRACK, "IK_NEXTTRACK");
	fuck(IK_NOCONVERT, "IK_NOCONVERT");
	fuck(IK_NUMLOCK, "IK_NUMLOCK");
	fuck(IK_NUMPAD0, "IK_NUMPAD0");
	fuck(IK_NUMPAD1, "IK_NUMPAD1");
	fuck(IK_NUMPAD2, "IK_NUMPAD2");
	fuck(IK_NUMPAD3, "IK_NUMPAD3");
	fuck(IK_NUMPAD4, "IK_NUMPAD4");
	fuck(IK_NUMPAD5, "IK_NUMPAD5");
	fuck(IK_NUMPAD6, "IK_NUMPAD6");
	fuck(IK_NUMPAD7, "IK_NUMPAD7");
	fuck(IK_NUMPAD8, "IK_NUMPAD8");
	fuck(IK_NUMPAD9, "IK_NUMPAD9");
	fuck(IK_NUMPADCOMMA, "IK_NUMPADCOMMA");
	fuck(IK_NUMPADENTER, "IK_NUMPADENTER");
	fuck(IK_NUMPADEQUALSDIK_NUMPADEQUALS, "IK_NUMPADEQUALSDIK_NUMPADEQUALS");
	fuck(IK_O, "IK_O");
	fuck(IK_OEM_102, "IK_OEM_102");
	fuck(IK_P, "IK_P");
	fuck(IK_PAUSE, "IK_PAUSE");
	fuck(IK_PERIOD, "IK_PERIOD");
	fuck(IK_PLAYPAUSE, "IK_PLAYPAUSE");
	fuck(IK_POWER, "IK_POWER");
	fuck(IK_PREVTRACK, "IK_PREVTRACK");
	fuck(IK_PRIOR, "IK_PRIOR");
	fuck(IK_Q, "IK_Q");
	fuck(IK_R, "IK_R");
	fuck(IK_RBRACKET, "IK_RBRACKET");
	fuck(IK_RCONTROL, "IK_RCONTROL");
	fuck(IK_RETURN, "IK_RETURN");
	fuck(IK_RIGHT, "IK_RIGHT");
	fuck(IK_RMENU, "IK_RMENU");
	fuck(IK_RSHIFT, "IK_RSHIFT");
	fuck(IK_RWIN, "IK_RWIN");
	fuck(IK_S, "IK_S");
	fuck(IK_SCROLL, "IK_SCROLL");
	fuck(IK_SEMICOLON, "IK_SEMICOLON");
	fuck(IK_SLASH, "IK_SLASH");
	fuck(IK_SLEEP, "IK_SLEEP");
	fuck(IK_SPACE, "IK_SPACE");
	fuck(IK_STOP, "IK_STOP");
	fuck(IK_SUBTRACT, "IK_SUBTRACT");
	fuck(IK_SYSRQ, "IK_SYSRQ");
	fuck(IK_T, "IK_T");
	fuck(IK_TAB, "IK_TAB");
	fuck(IK_U, "IK_U");
	fuck(IK_UNDERLINE, "IK_UNDERLINE");
	fuck(IK_UNLABELED, "IK_UNLABELED");
	fuck(IK_UP, "IK_UP");
	fuck(IK_V, "IK_V");
	fuck(IK_VOLUMEDOWN, "IK_VOLUMEDOWN");
	fuck(IK_VOLUMEUP, "IK_VOLUMEUP");
	fuck(IK_W, "IK_W");
	fuck(IK_WAKE, "IK_WAKE");
	fuck(IK_WEBBACK, "IK_WEBBACK");
	fuck(IK_WEBFAVORITESDIK_WEBFAVORITES, "IK_WEBFAVORITESDIK_WEBFAVORITES");
	fuck(IK_WEBFORWARD, "IK_WEBFORWARD");
	fuck(IK_WEBHOME, "IK_WEBHOME");
	fuck(IK_WEBREFRESH, "IK_WEBREFRESH");
	fuck(IK_WEBSEARCH, "IK_WEBSEARCH");
	fuck(IK_WEBSTOP, "IK_WEBSTOP");
	fuck(IK_X, "IK_X");
	fuck(IK_Y, "IK_Y");
	fuck(IK_YEN, "IK_YEN");
	fuck(IK_Z, "IK_Z");


splugin.bind_env(old_env)
