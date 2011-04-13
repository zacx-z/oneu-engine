#pragma once
#include "..\DXGraphics\DXGraphics.h"
#include "..\DXInput\DXInput.h"
//for GraphicsResMgr_cls
#ifdef ONEU_EXPORTS
#define ONEU_UTIL_API __declspec(dllexport)
#else
#define ONEU_UTIL_API __declspec(dllimport)
#endif
