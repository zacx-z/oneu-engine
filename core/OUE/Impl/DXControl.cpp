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
#include "DXControl.h"
#include "DXLib/DXInput.h"
#include "Win32.h"
#include "../Event.h"
#include "../Game.h"

namespace OneU
{
#pragma warning(push)
#pragma warning(disable :4800)
	void __KeyDataHandler(uint32 scancode, uint32 status, uint32, uint32){
		((DXControl&)GetControl())._notifyKey(scancode, bool(status & 0x80));
		GetGame().onKey(KeyEvent(scancode, (bool)(status & 0x80)));
	}
	void __MouseDataHandler(uint32 off, uint32 status, uint32, uint32){
		if(off != DIMOFS_X && off != DIMOFS_Y && off != DIMOFS_Z){
			uint32 buttoncode = off - DIMOFS_BUTTON0;
			((DXControl&)GetControl())._notifyButton(buttoncode, bool(status & 0x80));
			GetGame().onMouse(MouseEvent(buttoncode, (bool)(status & 0x80)));
		}
	}
#pragma warning(pop)
	DXControl::DXControl()
	{
		memset(&m_KB_State, 0, sizeof(m_KB_State));
		DX::Input.Init(g_hInstance, g_hWnd);
		m_KB.Create(10);
		m_KB.Acquire();
		m_Mouse.Create(10);
		m_Mouse.Acquire();
	}
	DXControl::~DXControl(){
		m_KB.Destroy();
		m_Mouse.Destroy();
		DX::Input.Destroy();
	}
	void DXControl::update(){
		m_KB.Read(m_KB_State);
		m_Mouse.Read(m_Mouse_State);

		memset(&m_KB_Release, 0, sizeof(m_KB_Release));
		memset(&m_KB_Press, 0, sizeof(m_KB_Press));
		memset(&m_Button_Release, 0, sizeof(m_Button_Release));
		memset(&m_Button_Press, 0, sizeof(m_Button_Press));

		m_KB.HandleData(__KeyDataHandler);
		m_Mouse.HandleData(__MouseDataHandler);
	}
	bool DXControl::keyIsDown(uint32 scancode){
		return m_KB_State.GetState(scancode); 
	}

	bool DXControl::keyPress(uint32 scancode){
		return m_KB_Press[scancode];
	}

	bool DXControl::keyRelease(uint32 scancode){
		return m_KB_Release[scancode];
	}

	bool DXControl::buttonIsDown(uint32 ButtonID){
		return m_Mouse_State.GetState(ButtonID);
	}

	bool DXControl::buttonRelease(uint32 ButtonID){
		return m_Button_Release[ButtonID];
	}

	bool DXControl::buttonPress(uint32 ButtonID){
		return m_Button_Press[ButtonID];
	}

	vector2i_t DXControl::mouseOffset(){
		return vector2i_t(m_Mouse_State.GetlX(), m_Mouse_State.GetlY());
	}
}