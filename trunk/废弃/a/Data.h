// Data.h: 管理数据的类
//
//////////////////////////////////////////////////////////////////////

#pragma once

namespace OneU
{
	class RawFile;

	template<class T>
	class CData  
	{
	public:
		typedef T DataType;

		CData(){};
		CData(T& Ref){ memcpy( &_Contents, &Ref, sizeof(Ref) );}
		~CData(){};

		void Get(RawFile* pFile)//从文件获取数据
		{
			pFile->Read( (char*)&_Contents, sizeof( _Contents ) );
		}
		void Put(RawFile* pFile) const//将数据放入文件
		{
			pFile->Write( (char*)&_Contents , sizeof( _Contents ) );
		}

		T& Data(){ return _Contents;}
		T* operator->(){ return &_Contents;}

	private:
		T _Contents;
	};




	//Data类的第2实现
	template<int size>
	class CData2
	{
	public:
		CData2(){};
		CData2(void *px){ memcpy( _Contents, px, size );}
		~CData2(){};

		void Get(RawFile* pFile)//从文件获取数据
		{
			pFile->Read( (char*)_Contents, sizeof( _Contents ) );
		}
		void Put(RawFile* pFile) const//将数据放入文件
		{
			pFile->Write( (char*)_Contents , sizeof( _Contents ) );
		}

		void* Data(){ return _Contents;}

	private:
		char _Contents[ size ];
	};
}