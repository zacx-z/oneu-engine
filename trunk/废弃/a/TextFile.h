#pragma once
#include "File.h"

namespace OneU
{
	class CTextFile : public RawFile
	{
	public:
		CTextFile( LPCSTR lpstrFileName, bool m_bRead=TRUE ):RawFile( lpstrFileName, m_bRead ){}
		void WriteText(const string& Str){
			Write( Str.c_str(), Str.length() );
		}
	};
}