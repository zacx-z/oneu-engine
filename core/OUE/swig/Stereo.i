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
		%extend {
			float getVolume(){ return self->get() ? self->get()->getVolume() : 0.0f; }
			void setVolume(float volume){ if(self->get()) self->get()->setVolume(volume); }
			OneU::uint32 getFrequency(){ return self->get() ? self->get()->getFrequency() : 0.0f; }
			void setFrequency(uint32 freq){ if(self->get()) self->get()->setFrequency(freq); }
			float getPan(){ return self->get() ? self->get()->getPan() : 0.0f; }
			void setPan(float pan){ if(self->get()) self->get()->setPan(pan); }
		}
	};
	%rename(Stereo) IStereo;
	class IStereo
	{
	public:
		virtual void init() = 0;
		virtual sound_t loadSound(OneU::pcwstr filename, bool streamed) = 0;
		virtual void playMusic(OneU::sound_t sound, bool looped = true) = 0;
		virtual void stopMusic() = 0;
		sound_t getPlayingMusic() = 0;
		virtual void playFX(OneU::sound_t sound) = 0;
	};
}
