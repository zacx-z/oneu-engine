#pragma once


#include "BaseLayer.h"
#include <windows.h>
#include "WinException.h"
#include "File.h"

namespace OneU
{
	extern HINSTANCE g_hInstance;
	extern HWND g_hWnd;
	//定义初始化结构体
	/** 
	 * @name Windows封装库
	 * @{ */
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 窗口初始化结构体
	 */
	/* ----------------------------------------------------------------------------*/
	typedef struct tagONEU_INITWIN
	{
		int iX;
		int iY;
		int iWidth;
		int iHeight;
		bool bPopUp;
		WCHAR *pWndName;
	} ONEU_INITWIN;

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 全局窗口初始化结构体
	 *
	 * 应在OneUPreInitWin中填充该结构体
	 * @sa OneUPreInitWin
	 */
	/* ----------------------------------------------------------------------------*/
	extern ONEU_INITWIN OneU_InitWin;

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 系统时间结构体
	 *
	 * 传入GetSystemTime函数的参数类型
	 * @sa GetSystemTime
	 */
	/* ----------------------------------------------------------------------------*/
	typedef struct tagSystemTime
	{
		WORD wYear; 
		WORD wMonth; 
		WORD wDayOfWeek; 
		WORD wDay; 
		WORD wHour; 
		WORD wMinute; 
		WORD wSecond; 
		WORD wMilliseconds; 
	}SYSTEMTIME;

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 句柄
	 */
	/* ----------------------------------------------------------------------------*/
	class Handle
	{
	private:
		::HANDLE m_h;
	public:
		typedef ::HANDLE Type;
		Handle() : m_h( 0 ){}
		Handle( ::HANDLE h ) : m_h( h ){}
		HANDLE GetHandle(){ return m_h; }
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 窗口句柄
	 */
	/* ----------------------------------------------------------------------------*/
	class HWindow
		: public Handle
	{
	private:
		::HWND m_hWnd;
	public:
		typedef ::HWND Type;
		HWindow() : m_hWnd( 0 ){}
		HWindow( HWND hWnd ) : m_hWnd( hWnd ){}
		HWND GetHWnd(){ return m_hWnd; }
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 进程句柄
	 */
	/* ----------------------------------------------------------------------------*/
	class HInstance
		: public Handle
	{
	private:
		::HINSTANCE m_hInstance;
	public:
		typedef ::HINSTANCE Type;
		HInstance() : m_hInstance( 0 ){}
		HInstance( HINSTANCE hInst ) : m_hInstance( hInst ){}
		HINSTANCE GetHInstance(){ return m_hInstance; }
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 设备描述表句柄
	 */
	/* ----------------------------------------------------------------------------*/
	class HDeviceContext
		: public Handle
	{
	private:
		::HDC m_hDC;
	public:
		typedef ::HDC Type;
		HDeviceContext() : m_hDC( 0 ){}
		HDeviceContext( HDC hDC ) : m_hDC( hDC ){}
		HDC GetHDC(){ return m_hDC; }
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 窗口绘图类
	 */
	/* ----------------------------------------------------------------------------*/
	class PaintDC
	{
	private:
		HDeviceContext m_hDC;
		HWindow m_hWnd;
	public:
		PaintDC( HWindow hWnd = g_hWnd )
		{
			::GetDC( hWnd.GetHWnd() );
			m_hWnd = hWnd;
		}
		~PaintDC()
		{
			::ReleaseDC( m_hWnd.GetHWnd(), m_hDC.GetHDC() );
		}
		HDeviceContext GetDeviceContext(){ return m_hDC; }
	};

	
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 退出函数
	 */
	/* ----------------------------------------------------------------------------*/
	inline void Quit(){ PostQuitMessage( 0 ); }
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 睡眠
	 * 
	 * @param dwMilliseconds 睡眠时间，以毫秒计
	 */
	/* ----------------------------------------------------------------------------*/
	inline void Sleep( DWORD dwMilliseconds ){ ::Sleep( dwMilliseconds ); }
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取相对时间
	 *
	 * 一定程度上不准确
	 * 
	 * @return 相对时间的值
	 */
	/* ----------------------------------------------------------------------------*/
	inline DWORD GetTime(){ return ::timeGetTime(); } 
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取系统时间
	