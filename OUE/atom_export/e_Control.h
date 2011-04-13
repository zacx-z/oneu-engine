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
#pragma once
#include "../Atom.h"
#include "../Control.h"
namespace OneU
{
	namespace atom_export
	{
        using namespace atom;
        class e_Control
                : public exported
        {
            IControl* e_control;
        public:
            e_Control(value* v)
            {
                e_control = &OneU::GetControl();
            }    
            value* e_keyIsDown(value* v)
            {
                return makeValue(e_control->keyIsDown(dword(utoArg<int>(v, 0))));
            }    
            value* e_keyPress(value* v)
            {
                return makeValue(e_control->keyPress(dword(utoArg<int>(v, 0)))); 
            }    
            value* e_keyRelease(value* v)
            {
                return makeValue(e_control->keyRelease(dword(utoArg<int>(v, 0))));
            }    
            value* e_buttonIsDown(value* v)
            {
                return makeValue(e_control->buttonIsDown(uint(utoArg<int>(v, 0))));
            }    
            value* e_buttonRelease(value* v)
            {
                return makeValue(e_control->buttonRelease(uint(utoArg<int>(v, 0))));
            }    
            value* e_buttonPress(value* v)
            {
                return makeValue(e_control->buttonPress(uint(utoArg<int>(v, 0))));
            }    
            value* e_mouseOffset(value* v)
            {
                return makeTupleWithElem(2, makeValue(e_control->mouseOffset().x), makeValue(e_control->mouseOffset().y));
            }    
            ~e_Control()
            {}    
        };

		inline void export_control_const(env* e_env)
		{
			e_env->createSymbol(L"OIK_ESCAPE",makeValue(OIK_ESCAPE));
			e_env->createSymbol(L"OIK_1",makeValue(OIK_1));
			e_env->createSymbol(L"OIK_2",makeValue(OIK_2));
			e_env->createSymbol(L"OIK_3",makeValue(OIK_3));
			e_env->createSymbol(L"OIK_4",makeValue(OIK_4));
			e_env->createSymbol(L"OIK_5",makeValue(OIK_5));
			e_env->createSymbol(L"OIK_6",makeValue(OIK_6));
			e_env->createSymbol(L"OIK_7",makeValue(OIK_7));
			e_env->createSymbol(L"OIK_8",makeValue(OIK_8));
			e_env->createSymbol(L"OIK_9",makeValue(OIK_9));
			e_env->createSymbol(L"OIK_0",makeValue(OIK_0));
			e_env->createSymbol(L"OIK_MINUS",makeValue(OIK_MINUS));
			e_env->createSymbol(L"OIK_EQUALS",makeValue(OIK_EQUALS));
			e_env->createSymbol(L"OIK_BACK",makeValue(OIK_BACK));
			e_env->createSymbol(L"OIK_TAB",makeValue(OIK_TAB));
			e_env->createSymbol(L"OIK_Q",makeValue(OIK_Q));                
			e_env->createSymbol(L"OIK_W",makeValue(OIK_W));                
			e_env->createSymbol(L"OIK_E",makeValue(OIK_E));                
			e_env->createSymbol(L"OIK_R",makeValue(OIK_R));                
			e_env->createSymbol(L"OIK_T",makeValue(OIK_T));                
			e_env->createSymbol(L"OIK_Y",makeValue(OIK_Y));                
			e_env->createSymbol(L"OIK_U",makeValue(OIK_U));                
			e_env->createSymbol(L"OIK_I",makeValue(OIK_I));                
			e_env->createSymbol(L"OIK_O",makeValue(OIK_O));                
			e_env->createSymbol(L"OIK_P",makeValue(OIK_P));                
			e_env->createSymbol(L"OIK_LBRACKET",makeValue(OIK_LBRACKET));         
			e_env->createSymbol(L"OIK_RBRACKET",makeValue(OIK_RBRACKET));         
			e_env->createSymbol(L"OIK_RETURN",makeValue(OIK_RETURN));                 
			e_env->createSymbol(L"OIK_LCONTROL",makeValue(OIK_LCONTROL));         
			e_env->createSymbol(L"OIK_A",makeValue(OIK_A));                
			e_env->createSymbol(L"OIK_S",makeValue(OIK_S));                
			e_env->createSymbol(L"OIK_D",makeValue(OIK_D));                
			e_env->createSymbol(L"OIK_F",makeValue(OIK_F));                
			e_env->createSymbol(L"OIK_G",makeValue(OIK_G));                
			e_env->createSymbol(L"OIK_H",makeValue(OIK_H));                
			e_env->createSymbol(L"OIK_J",makeValue(OIK_J));                
			e_env->createSymbol(L"OIK_K",makeValue(OIK_K));                
			e_env->createSymbol(L"OIK_L",makeValue(OIK_L));                
			e_env->createSymbol(L"OIK_SEMICOLON",makeValue(OIK_SEMICOLON));        
			e_env->createSymbol(L"OIK_APOSTROPHE",makeValue(OIK_APOSTROPHE));       
			e_env->createSymbol(L"OIK_GRAVE",makeValue(OIK_GRAVE));                  
			e_env->createSymbol(L"OIK_LSHIFT",makeValue(OIK_LSHIFT));           
			e_env->createSymbol(L"OIK_BACKSLASH",makeValue(OIK_BACKSLASH));        
			e_env->createSymbol(L"OIK_Z",makeValue(OIK_Z));                
			e_env->createSymbol(L"OIK_X",makeValue(OIK_X));                
			e_env->createSymbol(L"OIK_C",makeValue(OIK_C));                
			e_env->createSymbol(L"OIK_V",makeValue(OIK_V));                
			e_env->createSymbol(L"OIK_B",makeValue(OIK_B));                
			e_env->createSymbol(L"OIK_N",makeValue(OIK_N));                
			e_env->createSymbol(L"OIK_M",makeValue(OIK_M));                
			e_env->createSymbol(L"OIK_COMMA",makeValue(OIK_COMMA));            
			e_env->createSymbol(L"OIK_PERIOD",makeValue(OIK_PERIOD));                 
			e_env->createSymbol(L"OIK_SLASH",makeValue(OIK_SLASH));                   
			e_env->createSymbol(L"OIK_RSHIFT",makeValue(OIK_RSHIFT));           
			e_env->createSymbol(L"OIK_MULTIPLY",makeValue(OIK_MULTIPLY));               
			e_env->createSymbol(L"OIK_LMENU",makeValue(OIK_LMENU));                  
			e_env->createSymbol(L"OIK_SPACE",makeValue(OIK_SPACE));            
			e_env->createSymbol(L"OIK_CAPITAL",makeValue(OIK_CAPITAL));          
			e_env->createSymbol(L"OIK_F1",makeValue(OIK_F1));               
			e_env->createSymbol(L"OIK_F2",makeValue(OIK_F2));               
			e_env->createSymbol(L"OIK_F3",makeValue(OIK_F3));               
			e_env->createSymbol(L"OIK_F4",makeValue(OIK_F4));               
			e_env->createSymbol(L"OIK_F5",makeValue(OIK_F5));               
			e_env->createSymbol(L"OIK_F6",makeValue(OIK_F6));               
			e_env->createSymbol(L"OIK_F7",makeValue(OIK_F7));               
			e_env->createSymbol(L"OIK_F8",makeValue(OIK_F8));               
			e_env->createSymbol(L"OIK_F9",makeValue(OIK_F9));               
			e_env->createSymbol(L"OIK_F10",makeValue(OIK_F10));              
			e_env->createSymbol(L"OIK_NUMLOCK",makeValue(OIK_NUMLOCK));          
			e_env->createSymbol(L"OIK_SCROLL",makeValue(OIK_SCROLL));                 
			e_env->createSymbol(L"OIK_NUMPAD7",makeValue(OIK_NUMPAD7));          
			e_env->createSymbol(L"OIK_NUMPAD8",makeValue(OIK_NUMPAD8));          
			e_env->createSymbol(L"OIK_NUMPAD9",makeValue(OIK_NUMPAD9));          
			e_env->createSymbol(L"OIK_SUBTRACT",makeValue(OIK_SUBTRACT));               
			e_env->createSymbol(L"OIK_NUMPAD4",makeValue(OIK_NUMPAD4));          
			e_env->createSymbol(L"OIK_NUMPAD5",makeValue(OIK_NUMPAD5));          
			e_env->createSymbol(L"OIK_NUMPAD6",makeValue(OIK_NUMPAD6));          
			e_env->createSymbol(L"OIK_ADD",makeValue(OIK_ADD));                    
			e_env->createSymbol(L"OIK_NUMPAD1",makeValue(OIK_NUMPAD1));          
			e_env->createSymbol(L"OIK_NUMPAD2",makeValue(OIK_NUMPAD2));          
			e_env->createSymbol(L"OIK_NUMPAD3",makeValue(OIK_NUMPAD3));          
			e_env->createSymbol(L"OIK_NUMPAD0",makeValue(OIK_NUMPAD0));          
			e_env->createSymbol(L"OIK_DECIMAL",makeValue(OIK_DECIMAL));                
			e_env->createSymbol(L"OIK_OEM_102",makeValue(OIK_OEM_102));                
			e_env->createSymbol(L"OIK_F11",makeValue(OIK_F11));              
			e_env->createSymbol(L"OIK_F12",makeValue(OIK_F12));              
			e_env->createSymbol(L"OIK_F13",makeValue(OIK_F13));                    
			e_env->createSymbol(L"OIK_F14",makeValue(OIK_F14));                    
			e_env->createSymbol(L"OIK_F15",makeValue(OIK_F15));                    
			e_env->createSymbol(L"OIK_KANA",makeValue(OIK_KANA));                   
			e_env->createSymbol(L"OIK_ABNT_C1",makeValue(OIK_ABNT_C1));                
			e_env->createSymbol(L"OIK_CONVERT",makeValue(OIK_CONVERT));                
			e_env->createSymbol(L"OIK_NOCONVERT",makeValue(OIK_NOCONVERT));              
			e_env->createSymbol(L"OIK_YEN",makeValue(OIK_YEN));                    
			e_env->createSymbol(L"OIK_ABNT_C2",makeValue(OIK_ABNT_C2));                
			e_env->createSymbol(L"OIK_NUMPADEQUALS",makeValue(OIK_NUMPADEQUALS));           
			e_env->createSymbol(L"OIK_PREVTRACK",makeValue(OIK_PREVTRACK));              
			e_env->createSymbol(L"OIK_AT",makeValue(OIK_AT));                     
			e_env->createSymbol(L"OIK_COLON",makeValue(OIK_COLON));                  
			e_env->createSymbol(L"OIK_UNDERLINE",makeValue(OIK_UNDERLINE));              
			e_env->createSymbol(L"OIK_KANJI",makeValue(OIK_KANJI));                  
			e_env->createSymbol(L"OIK_STOP",makeValue(OIK_STOP));                   
			e_env->createSymbol(L"OIK_AX",makeValue(OIK_AX));                     
			e_env->createSymbol(L"OIK_UNLABELED",makeValue(OIK_UNLABELED));              
			e_env->createSymbol(L"OIK_NEXTTRACK",makeValue(OIK_NEXTTRACK));              
			e_env->createSymbol(L"OIK_NUMPADENTER",makeValue(OIK_NUMPADENTER));            
			e_env->createSymbol(L"OIK_RCONTROL",makeValue(OIK_RCONTROL));         
			e_env->createSymbol(L"OIK_MUTE",makeValue(OIK_MUTE));                   
			e_env->createSymbol(L"OIK_CALCULATOR",makeValue(OIK_CALCULATOR));             
			e_env->createSymbol(L"OIK_PLAYPAUSE",makeValue(OIK_PLAYPAUSE));            
			e_env->createSymbol(L"OIK_MEDIASTOP",makeValue(OIK_MEDIASTOP));              
			e_env->createSymbol(L"OIK_VOLUMEDOWN",makeValue(OIK_VOLUMEDOWN));             
			e_env->createSymbol(L"OIK_VOLUMEUP",makeValue(OIK_VOLUMEUP));               
			e_env->createSymbol(L"OIK_WEBHOME",makeValue(OIK_WEBHOME));                
			e_env->createSymbol(L"OIK_NUMPADCOMMA",makeValue(OIK_NUMPADCOMMA));            
			e_env->createSymbol(L"OIK_DIVIDE",makeValue(OIK_DIVIDE));                  
			e_env->createSymbol(L"OIK_SYSRQ",makeValue(OIK_SYSRQ));            
			e_env->createSymbol(L"OIK_RMENU",makeValue(OIK_RMENU));                  
			e_env->createSymbol(L"OIK_PAUSE",makeValue(OIK_PAUSE));                  
			e_env->createSymbol(L"OIK_HOME",makeValue(OIK_HOME));                   
			e_env->createSymbol(L"OIK_UP",makeValue(OIK_UP));                     
			e_env->createSymbol(L"OIK_PRIOR",makeValue(OIK_PRIOR));                  
			e_env->createSymbol(L"OIK_LEFT",makeValue(OIK_LEFT));                   
			e_env->createSymbol(L"OIK_RIGHT",makeValue(OIK_RIGHT));                  
			e_env->createSymbol(L"OIK_END",makeValue(OIK_END));                    
			e_env->createSymbol(L"OIK_DOWN",makeValue(OIK_DOWN));                   
			e_env->createSymbol(L"OIK_NEXT",makeValue(OIK_NEXT));                   
			e_env->createSymbol(L"OIK_INSERT",makeValue(OIK_INSERT));                 
			e_env->createSymbol(L"OIK_DELETE",makeValue(OIK_DELETE));                 
			e_env->createSymbol(L"OIK_LWIN",makeValue(OIK_LWIN));                   
			e_env->createSymbol(L"OIK_RWIN",makeValue(OIK_RWIN));                   
			e_env->createSymbol(L"OIK_APPS",makeValue(OIK_APPS));                   
			e_env->createSymbol(L"OIK_POWER",makeValue(OIK_POWER));                  
			e_env->createSymbol(L"OIK_SLEEP",makeValue(OIK_SLEEP));                  
			e_env->createSymbol(L"OIK_WAKE",makeValue(OIK_WAKE));                   
			e_env->createSymbol(L"OIK_WEBSEARCH",makeValue(OIK_WEBSEARCH));              
			e_env->createSymbol(L"OIK_WEBFAVORITES",makeValue(OIK_WEBFAVORITES));           
			e_env->createSymbol(L"OIK_WEBREFRESH",makeValue(OIK_WEBREFRESH));             
			e_env->createSymbol(L"OIK_WEBSTOP",makeValue(OIK_WEBSTOP));                
			e_env->createSymbol(L"OIK_WEBFORWARD",makeValue(OIK_WEBFORWARD));             
			e_env->createSymbol(L"OIK_WEBBACK",makeValue(OIK_WEBBACK));                
			e_env->createSymbol(L"OIK_MYCOMPUTER",makeValue(OIK_MYCOMPUTER));             
			e_env->createSymbol(L"OIK_MAIL",makeValue(OIK_MAIL));                   
			e_env->createSymbol(L"OIK_MEDIASELECT",makeValue(OIK_MEDIASELECT));            

			e_env->createSymbol(L"OIK_BACKSPACE",makeValue(OIK_BACKSPACE));            
			e_env->createSymbol(L"OIK_NUMPADSTAR",makeValue(OIK_NUMPADSTAR));        
			e_env->createSymbol(L"OIK_LALT",makeValue(OIK_LALT));                
			e_env->createSymbol(L"OIK_CAPSLOCK",makeValue(OIK_CAPSLOCK));            
			e_env->createSymbol(L"OIK_NUMPADMINUS",makeValue(OIK_NUMPADMINUS));            
			e_env->createSymbol(L"OIK_NUMPADPLUS",makeValue(OIK_NUMPADPLUS));                  
			e_env->createSymbol(L"OIK_NUMPADPERIOD",makeValue(OIK_NUMPADPERIOD));        
			e_env->createSymbol(L"OIK_NUMPADSLASH",makeValue(OIK_NUMPADSLASH));            
			e_env->createSymbol(L"OIK_RALT",makeValue(OIK_RALT));                   
			e_env->createSymbol(L"OIK_UPARROW",makeValue(OIK_UPARROW));                     
			e_env->createSymbol(L"OIK_PGUP",makeValue(OIK_PGUP));              
			e_env->createSymbol(L"OIK_LEFTARROW",makeValue(OIK_LEFTARROW));                   
			e_env->createSymbol(L"OIK_RIGHTARROW",makeValue(OIK_RIGHTARROW));              
			e_env->createSymbol(L"OIK_DOWNARROW",makeValue(OIK_DOWNARROW));                  
			e_env->createSymbol(L"OIK_PGDN",makeValue(OIK_PGDN));                        

			e_env->createSymbol(L"OIK_CIRCUMFLEX",makeValue(OIK_CIRCUMFLEX));    
		}   

        inline void export_Control()
        {
            value* v = makeKlass<e_Control>();
			klass* k = v->get<klass>();
			GetAtom().getEnv()->createSymbol(L"Control_k", v);
			uexportMethod(k, L"keyIsDown", &e_Control::e_keyIsDown);
			uexportMethod(k, L"keyPress", &e_Control::e_keyPress);
			uexportMethod(k, L"keyRelease", &e_Control::e_keyRelease);
			uexportMethod(k, L"buttonIsDown", &e_Control::e_buttonIsDown);
			uexportMethod(k, L"buttonPress", &e_Control::e_buttonPress);
			uexportMethod(k, L"buttonRelease", &e_Control::e_buttonRelease);
			uexportMethod(k, L"mouseOffset", &e_Control::e_mouseOffset);

			export_control_const(k);

			GetAtom().getEnv()->createSymbol(L"Control", makeObj(k, NULL));
		}    
    }
}        
