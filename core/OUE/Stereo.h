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
 * @file Stereo.h
 * @brief 音响系统
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
#include "TplTypes.h"

namespace OneU
{
	namespace stereo
	{
		class ISound
			: public InterfaceRef
		{
		public:
			virtual float getVolume() = 0;
			virtual void setVolume(float volume) = 0;
			virtual uint32 getFrequency() = 0;
			virtual void setFrequency(uint32 freq) = 0;
			virtual float getPan() = 0;
			virtual void setPan(float pan) = 0;
		};
	}
	typedef RefWrapper<stereo::ISound> sound_t;
	class IStereo
		: public Interface
	{
	public:
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 初始化
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void init() = 0;
		virtual sound_t loadSound(pcwstr filename, bool streamed) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 播放音乐流
		 *
		 * @param filename 音乐的流文件路径
		 * @remarks 会停止当前播放的音乐
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void playMusic(sound_t sound, bool looped = true) = 0;
		virtual void stopMusic() = 0;
		virtual sound_t getPlayingMusic() = 0;

		virtual void playFX(sound_t sound) = 0;
		virtual void update() = 0;
	};

	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 音响工厂
	 *
	 * @return 音响接口
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API IStereo* Stereo_create();
}
