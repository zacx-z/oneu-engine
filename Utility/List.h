#pragma once
#include "../Base/Base.h"
#include <list>
#ifndef ONEU_NO_BOOST
#include <boost/pool/pool_alloc.hpp>
#endif

namespace OneU
{
	template<class T>
	class List
#ifndef ONEU_NO_BOOST
		: public std::list<T, boost::pool_allocator<T> >{};
#else
		: public std::list<T>{};
#endif
}