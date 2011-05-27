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
#include "DXVideo.h"
#include "Win32.h"
#include "../../DXGraphics/DXGraphics.h"
#include "../../DXGraphics/ImageE.h"
#include "../../DXGraphics/VertexUP.h"
#include "../../DXGraphics/TStage.h"
#include "../Atom.h"
#include "../Game.h"

#ifndef _WIN32
#pragma message("Error! Platform isn't win32.")
#endif
namespace OneU
{
	DX::RenderManip* g_pRD;

	class _Video_ClearNode
		: public video::INode
	{
		void paint(){ DX::Graphics.ClearTarget();}
		void describe(String& buffer, int depth){ buffer.append(L"<colorfill>\n");}
	};

	DXVideo::DXVideo()
		: m_ImageSource(0.0f, 0.0f, 1.0f, 1.0f), mix_color(0, 0, 0, 0), mix_mode(video::CBM_NONE)
	{
		g_pRD = NULL;
	}
	void DXVideo::init(uint width, uint height, bool bWindowed){
		ASSERT(g_hWnd != NULL);
		m_IsWindowed = bWindowed;
		try{
			DX::Graphics.PreInit();
			if(bWindowed)
				DX::Graphics.InitWindowed(width, height, g_hWnd);
			else{
				DX::DisplayMode DM;
				DM.Format = DX::PXLFMT_A8R8G8B8;
				DM.Width = width;
				DM.Height = height;
				DM.RefreshRate = 0;//use default
				DX::Graphics.InitFullScreen(&DM, g_hWnd);
			}
			DX::Graphics.SetCullMode(0);
		}
		catch(Exception& e){
			ONEU_RAISE(e);
		}
		catch(...){
			ONEU_RAISE(L"未知错误");
		}

		m_DeviceSize = vector2u_t(width, height);

		m_pRoot = ONEU_NEW INodeContainer;

		m_pRoot->addChild(ONEU_NEW _Video_ClearNode, -100);

		//Atom
		GetAtom().getSystemEnv()->createSymbol(L"Direct3D9_", atom::makeValue((void*)DX::_pD3D));
		GetAtom().getSystemEnv()->createSymbol(L"Direct3DDevice9_", atom::makeValue((void*)DX::_pD3DDevice));
	}

	void DXVideo::switchDevice(uint width, uint height, bool bWindowed){
		unloadD3DResource();
		m_IsWindowed =bWindowed;
		if(bWindowed){
			if(width == 0 || height == 0){
				width = m_DeviceSize.x;
				height = m_DeviceSize.y;
			}
			DX::Graphics.ResetWindowed(width, height, g_hWnd);
		}else{
			if(width == 0 || height == 0){
				width = m_DeviceSize.x;
				height = m_DeviceSize.y;
			}
			DX::DisplayMode DM;
			DM.Format = DX::PXLFMT_A8R8G8B8;
			DM.RefreshRate = 0;//use default
			DM.Width = width;
			DM.Height = height;
			DX::Graphics.ResetFullScreen(&DM, g_hWnd);
		}
		reloadD3DResource();
		m_DeviceSize = vector2u_t(width, height);
	}
	vector2u_t DXVideo::getDeviceSize(){
		return m_DeviceSize;
	}
	void DXVideo::render(){
		try{
			DX::RenderManip rd;
			g_pRD = &rd;
			m_pRoot->visit_paint();
			g_pRD = NULL;
		}
		catch(Exception& e){
			ONEU_RAISE(e);
		}
		catch(...){
			ONEU_RAISE(L"未知错误");
		}
	}
	void DXVideo::update(){
		m_pRoot->visit_update();
	}

	void DXVideo::flip(){
		try{
			DX::Graphics.Present();
		}
		catch(Exception& e){
			ONEU_RAISE(e);
		}
		catch(...){
			ONEU_RAISE(L"未知错误");
		}
	}

	_DXImageTag* DXVideo::_getDXImageTag(pcwstr filename){
		_DXImageTag** p = m_ImageTable.insert(filename);
		if(p != NULL){//didn't find
			*p = NULL;
			*p = new _DXImageTag;
			_DXImageTag* pTag = *p;
			pTag->ref = 1;
			pTag->video = this;
			pTag->it = m_ImageTable.getLastItem();
			pTag->fileName = filename;
			try{
				DX::CreateTextureFromFile(filename, pTag->texture, 1, DX::PXLFMT_A8R8G8B8);
				D3DXGetImageInfoFromFile(filename, &pTag->info);
			}
			catch(Exception& e){
				ONEU_RAISE(e);
			}

			return pTag;
		}
		else{
			_DXImageTag* pTag = m_ImageTable.getLastItem().getValue();
			++pTag->ref;
			return pTag;
		}
	}

	image_t DXVideo::loadImage(pcwstr filename){
		DXImage* p = ONEU_NEW DXImage(_getDXImageTag(filename));
		return p;
	}

	void DXVideo::_RenderImage_SetState(){
		switch(mix_mode & 0xf){
		case CBM_MODULATE:
			DX::TStage(0).SetState(DX::TStage::COLOR_OP, DX::TStage::OP_MODULATE);
			break;
		case CBM_ADD:
			DX::TStage(0).SetState(DX::TStage::COLOR_OP, DX::TStage::OP_ADD);
			break;
		}
	}

	template<class T>
	inline void DXVideo::_RenderImage_SetDiffuse(T v[]){
		switch(mix_mode & 0xf0)
		{
		case 0:
			v[0].Diffuse() = v[1].Diffuse() = v[2].Diffuse() = v[3].Diffuse() = mix_color;
			break;
		case CBM_RIGHT:
			v[0].Diffuse() = v[1].Diffuse() = color_t(0, 0, 0, 0);
			v[2].Diffuse() = v[3].Diffuse() = mix_color;
			break;
		case CBM_LEFT:
			v[0].Diffuse() = v[1].Diffuse() = mix_color;
			v[2].Diffuse() = v[3].Diffuse() = color_t(0, 0, 0, 0);
			break;
		case CBM_TOP:
			v[1].Diffuse() = v[2].Diffuse() = color_t(0, 0, 0, 0);
			v[0].Diffuse() = v[3].Diffuse() = mix_color;
			break;
		case CBM_DOWN:
			v[1].Diffuse() = v[2].Diffuse() = mix_color;
			v[0].Diffuse() = v[3].Diffuse() = color_t(0, 0, 0, 0);
		}
	}
	//Render functions
	void DXVideo::renderImage(video::IImage& image, const rect& dest){
		////因为是单Video，所以存在DXVideo必然Image是DXImage。
		DXImage& img = ((DXImage&)image);
		DX::TStage(0).SetTexture(img._getTag()->texture);
		if(mix_mode == CBM_NONE){
			DX::TStage(0).SetState(DX::TStage::ALPHA_OP, DX::TStage::OP_SELECTARG1);
			DX::TStage(0).SetState(DX::TStage::COLOR_OP, DX::TStage::OP_SELECTARG1);

			static DX::VertexUP< DX::FVF_XYZ | DX::FVF_TEX1 > v[4];

			v[0].SetPos(dest.left, dest.top);
			v[1].SetPos(dest.left, dest.bottom);
			v[2].SetPos(dest.right, dest.bottom);
			v[3].SetPos(dest.right, dest.top);

			v[0].SetUV<0>(m_ImageSource.left, m_ImageSource.top);
			v[1].SetUV<0>(m_ImageSource.left, m_ImageSource.bottom);
			v[2].SetUV<0>(m_ImageSource.right, m_ImageSource.bottom);
			v[3].SetUV<0>(m_ImageSource.right, m_ImageSource.top);

			DX::Graphics.SetFVF(v);
			g_pRD->RenderVertexUP(DX::RenderManip::PT_TRIANGLEFAN, v, 2);
		}
		else{
			DX::TStage(0).SetState(DX::TStage::ALPHA_OP, DX::TStage::OP_MODULATE);
			_RenderImage_SetState();

			static DX::VertexUP< DX::FVF_XYZ | DX::FVF_TEX1 | DX::FVF_DIFFUSE > v[4];

			v[0].SetPos(dest.left, dest.top);
			v[1].SetPos(dest.left, dest.bottom);
			v[2].SetPos(dest.right, dest.bottom);
			v[3].SetPos(dest.right, dest.top);

			v[0].SetUV<0>(m_ImageSource.left, m_ImageSource.top);
			v[1].SetUV<0>(m_ImageSource.left, m_ImageSource.bottom);
			v[2].SetUV<0>(m_ImageSource.right, m_ImageSource.bottom);
			v[3].SetUV<0>(m_ImageSource.right, m_ImageSource.top);

			_RenderImage_SetDiffuse(v);

			DX::Graphics.SetFVF(v);
			g_pRD->RenderVertexUP(DX::RenderManip::PT_TRIANGLEFAN, v, 2);
		}


	}
	void DXVideo::renderImage_d(video::IImage& image, const rect& dest){
		////因为是单Video，所以存在DXVideo必然Image是DXImage。
		DXImage& img = ((DXImage&)image);
		DX::TStage(0).SetTexture(img._getTag()->texture);
		if(mix_mode == CBM_NONE){
			DX::TStage(0).SetState(DX::TStage::ALPHA_OP, DX::TStage::OP_SELECTARG1);
			DX::TStage(0).SetState(DX::TStage::COLOR_OP, DX::TStage::OP_SELECTARG1);

			static DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_TEX1 > v[4];

			v[0].RhW() = v[1].RhW() = v[2].RhW() = v[3].RhW() = 1.0f;

			v[0].SetPos(dest.left, dest.top);
			v[1].SetPos(dest.left, dest.bottom);
			v[2].SetPos(dest.right, dest.bottom);
			v[3].SetPos(dest.right, dest.top);

			v[0].SetUV<0>(m_ImageSource.left, m_ImageSource.top);
			v[1].SetUV<0>(m_ImageSource.left, m_ImageSource.bottom);
			v[2].SetUV<0>(m_ImageSource.right, m_ImageSource.bottom);
			v[3].SetUV<0>(m_ImageSource.right, m_ImageSource.top);

			DX::Graphics.SetFVF( v );

			g_pRD->RenderVertexUP(DX::RenderManip::PT_TRIANGLEFAN, v, 2);
		}
		else{
			DX::TStage(0).SetState(DX::TStage::ALPHA_OP, DX::TStage::OP_MODULATE);
			_RenderImage_SetState();

			static DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_TEX1 | DX::FVF_DIFFUSE > v[4];

			v[0].RhW() = v[1].RhW() = v[2].RhW() = v[3].RhW() = 1.0f;

			v[0].SetPos(dest.left, dest.top);
			v[1].SetPos(dest.left, dest.bottom);
			v[2].SetPos(dest.right, dest.bottom);
			v[3].SetPos(dest.right, dest.top);

			v[0].SetUV<0>(m_ImageSource.left, m_ImageSource.top);
			v[1].SetUV<0>(m_ImageSource.left, m_ImageSource.bottom);
			v[2].SetUV<0>(m_ImageSource.right, m_ImageSource.bottom);
			v[3].SetUV<0>(m_ImageSource.right, m_ImageSource.top);

			_RenderImage_SetDiffuse(v);

			DX::Graphics.SetFVF(v);
			g_pRD->RenderVertexUP(DX::RenderManip::PT_TRIANGLEFAN, v, 2);
		}
	}

	void DXVideo::setTransform(const matrix& m){
		DX::GetGraphics()->SetWorldTransform((MATRIX*)&m);
	}

	void DXVideo::setBlendMode(video::BLENDMODE mode)
	{
		using DX::_pD3DDevice;
		switch(mode)
		{
		case BM_NORMAL:
			_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		case BM_ADD:
			_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;
		}
	}

	video::IRenderScene* DXVideo::getRenderScene(){
		return m_pRenderScene;
	}

 	void DXVideo::unloadD3DResource(){
		for(ImageTable_t::iterator it = m_ImageTable.begin(); it != m_ImageTable.end(); ++it)
			it.getValue()->texture.Destroy();
	}
	void DXVideo::reloadD3DResource(){
		for(ImageTable_t::iterator it = m_ImageTable.begin(); it != m_ImageTable.end(); ++it){
			_DXImageTag* p = it.getValue();
			DX::CreateTextureFromFile(p->fileName.c_str(), p->texture, 1, DX::PXLFMT_A8R8G8B8);
		}
	}

	void DXVideo::showInfo(){
		//-------------------------------------------------------------
		//Shell输出信息
		GetGame().stream() << L"Direct3D9 Video\n";

		// Adapter details
		D3DADAPTER_IDENTIFIER9 adapterID;
		memset(&adapterID, 0, sizeof(adapterID));
		DX::_pD3D->GetAdapterIdentifier(0, 0, &adapterID);

		GetGame().stream() << "Driver:" << adapterID.Description << endl;
		//GetGame().stream() << "Vendor:";
		//从OgreD3D9RenderSystem.cpp拷贝出来的
		// determine vendor
		// Full list of vendors here: http://www.pcidatabase.com/vendors.php?sort=id
		//switch(adapterID.VendorId)
		//{
		//case 0x10DE:
		//	GetGame().stream() << L"Nvidia" << endl;
		//	break;
		//case 0x1002:
		//	GetGame().stream() << L"ATI" << endl;
		//	break;
		//case 0x163C:
		//case 0x8086:
		//	GetGame().stream() << L"Intel" << endl;
		//	break;
		//case 0x5333:
		//	GetGame().stream() << L"S3" << endl;
		//	break;
		//case 0x3D3D:
		//	GetGame().stream() << L"3DLabs" << endl;
		//	break;
		//case 0x102B:
		//	GetGame().stream() << L"Matrox" << endl;
		//	break;
		//case 0x1039:
		//	GetGame().stream() << L"SIS" << endl;
		//	break;
		//default:
		//	GetGame().stream() << L"<Unknown>" << endl;
		//	break;
		//};
		GetGame().stream() << endl;
	}

	DXVideo::~DXVideo(){
		try{
			if(m_pRoot != NULL){ ONEU_DELETE m_pRoot; m_pRoot = NULL; }
			DX::Graphics.Destroy();
		}
		catch(Exception& e){
			ONEU_RAISE(e);
		}
		catch(...){
			ONEU_RAISE(L"未知错误");
		}
	}



	//DXImage
	//必须在Video销毁前析构
	DXImage::~DXImage(){
		if(--m_pTag->ref == 0){
			m_pTag->video->m_ImageTable.erase(m_pTag->it);
			delete m_pTag;
		}
	}
	
	uint DXImage::addRef(){
		return ++m_ref;
	}
	uint DXImage::release(){
		--m_ref;
		if(m_ref == 0){
			ONEU_DELETE this;
			return 0;
		}
		return m_ref;
	}

	uint DXImage::getWidth(){
		return m_pTag->info.Width;
	}
	uint DXImage::getHeight(){
		return m_pTag->info.Height;
	}
}