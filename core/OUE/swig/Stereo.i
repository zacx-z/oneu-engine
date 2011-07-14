%module(directors = "1") OUE

%{
#include "../Stereo.h"
%}

namespace OneU
{
	%rename(Sound_t) sound_t;
	class sound_t
	{
	public:
		sound_t();
	};
	%rename(Stereo) IStereo;
	class IStereo
	{
	public:
		virtual void init() = 0;
		virtual sound_t loadSound(OneU::pcwstr filename, bool streamed) = 0;
		virtual void playMusic(OneU::sound_t sound, bool looped = true) = 0;
		virtual void playFX(OneU::sound_t sound) = 0;
	};
}
