%module OUE

%{
#include "../Stereo.h"
%}

namespace OneU
{
	class IStereo
	{
	public:
		virtual void init() = 0;
		virtual void playMusic(OneU::pcwstr filename) = 0;
		virtual void playSound(OneU::pcwstr filename) = 0;
	};
}