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
#include "../Video.h"
#include "../../DXGraphics/DXGraphics.h"
#include "../../DXGraphics/SurTex/SurTex.h"
#include "../Table.h"
#include <d3dx9.h>

namespace OneU
{
	struct _DXImageTag;

	extern DX::RenderManip* g_pRD;

	using namespace video;

	class DXVideo
		: public IVideo
	{

		typedef Table<_DXImageTag*> ImageTable_t;
		ImageTable_t m_ImageTable;
		friend class DXImage;

		vector2u_t m_DeviceSize;
		bool m_IsWindowed;
	public:
		DXVideo();
		~DXVideo();
		void destroy();

		LPCTSTR getName(){ return L"OneU D3D Video 1.0"; }

		void init(uint width, uint height, bool bWindowed);
		void switchDevice(uint width, uint height, bool bWindowed);

		bool isWindowed(){ return m_IsWindowed;}
		vector2u_t getDeviceSize();

		void render();
		void update();
		void flip();

		image_t loadImage(pcwstr filename);

		//Rendering functions
	private:
		rect m_ImageSource;
		color_t mix_color;
		dword mix_mode;

		//implement
		void _RenderImage_SetState();
		template<class T>
		inline void _RenderImage_SetDiffuse(T vertex[]);
	public:
		void renderImage(video::IImage& image, const rect& dest);
		void renderImage_d(video::IImage& image, const rect& dest);
		void setTransform(const matrix& m);
		void selectImageSourceRect(const rect& src){ m_ImageSource = src; }
		void setBlendMode(video::BLENDMODE mode);
		void setBlendColor(dword mode, color_t color){ mix_mode = mode; mix_color = color; }

		//渲染树函数
		video::IRenderScene* setRenderScene(video::IRenderScene* pRenderScene){
			if(m_pRenderScene != NULL)m_pRenderScene->detach();
			m_pRoot->addChild(pRenderScene, 0);
			return IVideo::setRenderScene(pRenderScene);
		}
		video::IRenderScene* getRenderScene();

		_DXImageTag* _getDXImageTag(pcwstr filename);
	private:
		void unloadD3DResource();
		void reloadD3DResource();

	public:
		//others
		//不太重要的
		void showInfo();
	};

	struct _DXImageTag
	{
		DXVideo* video;
		Table<_DXImageTag*>::iterator it;
		uint ref;
		String fileName;
		DX::Texture texture;
		D3DXIMAGE_INFO info;
	};

	class DXImage
		: public video::IImage
	{
		_DXImageTag* m_pTag;
		uint m_ref;

		DXImage(_DXImageTag* pTag) : m_ref(0), m_pTag(pTag){}
		friend class DXVideo;
	public:
		~DXImage();
		void destroy();

		uint addRef();
		uint release();

		uint getWidth();
		uint getHeight();

		_DXImageTag* _getTag(){ return m_pTag; }
	};
}