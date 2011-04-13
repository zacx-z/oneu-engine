//字符串转换函数 保留 目前低效
#include <cstring>
#include <string>
#include <sstream>
inline ::std::string& inttostr( const int value, ::std::string& str )
{
	_itoa( value, const_cast<char*> (str.c_str()), 10 );//暴力转换 因为string缓冲最小15字符 满足需要
	return str;
}
inline ::std::string& dwordtostr( const DWORD value, ::std::string& str )
{
	::std::ostringstream oss;
	oss << value;
	oss.str( str );
	return str;
}
