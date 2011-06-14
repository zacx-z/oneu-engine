#include "../OUE/OUE.h"

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ::LPSTR lpCmdLine, int nShowCmd){
	OneU::RubyRun();
	return 0;
}