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
#include "DXLib/DXGraphics.h"
#include "DXLib/Texture.h"
#include "DXLib/DXAUX.h"
#include "../Table.h"
#include "../Stack.h"
#include <d3dx9.h>

namespace OneU
{
	struct _DXImageTag;

	extern DX::RenderManip* g_pRD;

	using namespace video;

	class DXVideo
		: public IVideo
	{
		IDirect3D9* m_pD3D;
		IDirect3DDevice9* m_pD3DDevice;

		typedef Table<_DXImageTag*> ImageTable_t;
		ImageTable_t m_ImageTable;
		friend class DXImage;

		vector2u_t m_DeviceSize;
		bool m_IsWindowed;

		//D3D
		D3DCAPS9 m_Caps;
		D3DDISPLAYMODE m_desktopMode;
	public:
		DXVideo();
		~DXVideo();

		//Standard Override
		LPCTSTR getName(){ return L"OneU D3D Video 1.0"; }

		void prepare();
		void init(uint32 width, uint32 height, bool bWindowed);
		void switchDevice(uint32 width, uint32 height, bool bWindowed);

		bool isWindowed(){ return m_IsWindowed;}
		vector2u_t getDeviceSize();

		void render();
		void update(float dt);
		void flip();

		video::IImage* _loadImage(pcwstr filename);

	private:
		//Renderer State
		rect m_ImageSource;
		color_t mix_color;
		uint32 mix_mode;

		//Implement
		void _RenderImage_SetState();
		template<class T>
		inline void _RenderImage_SetDiffuse(T vertex[]);
	public:
		//Rendering Functions
		void renderImage(video::IImage& image, const rect& dest);
		void renderImage_d(video::IImage& image, const rect& dest);
		void setTransform(const matrix& m);
		void selectImageSourceRect(const rect& src){ m_ImageSource = src; }
		void setBlendMode(video::BLENDMODE mode);
		void setBlendColor(uint32 mode, color_t color){ mix_mode = mode; mix_color = color; }

		//Renderable Node Functions
		video::IRenderScene* setRenderScene(video::IRenderScene* pRenderScene){
			if(m_pRenderScene)m_pRenderScene->detach();
			if(pRenderScene) m_pRoot->addChild(pRenderScene, 0);
			return IVideo::setRenderScene(pRenderScene);
		}
		video::IRenderScene* getRenderScene();

		_DXImageTag* _getDXImageTag(pcwstr filename);

	public:
		void getAvailableMode(List<video::Mode>& buf);
		//others
		//不太重要的
		void showInfo();

	private:
		//Resource Functions
		//called when device is lost
		void unloadD3DResource();
		void reloadD3DResource();
		//Auxiliary library
		//for d3d only
	private:
		List<ID3DXFont*> m_fontList;
		List<ID3DXSprite*> m_spriteList;
	public:
		//for resource manage
		List<ID3DXFont*>::iterator _registerD3DXFont(ID3DXFont* pFont);
		void _unregisterD3DXFont(List<ID3DXFont*>::iterator pFont);
		List<ID3DXSprite*>::iterator _registerD3DXSprite(ID3DXSprite* pSprite);
		void _unregisterD3DXSprite(List<ID3DXSprite*>::iterator pSprite);

	};

	struct _DXImageTag
	{
		DXVideo* video;
		Table<_DXImageTag*>::iterator it;
		uint32 ref;
		String fileName;
		DX::Texture texture;
		D3DXIMAGE_INFO info;
	};

	class DXImage
		: public video::IImage
	{
		_DXImageTag* m_pTag;
		uint32 m_ref;

		DXImage(_DXImageTag* pTag) : m_ref(0), m_pTag(pTag){}
		friend class DXVideo;
	public:
		~DXImage();

		uint32 getWidth();
		uint32 getHeight();

		_DXImageTag* _getTag(){ return m_pTag; }
	};
}