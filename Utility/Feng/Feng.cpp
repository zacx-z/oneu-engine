#include "FengAlgo.h"
namespace OneU
{
	dword Cross( const CLine2 &l1, const CLine2 &l2 )
	{
		return ( ( l1.p1 >> l1.p2 ) % ( l1.p1 >> l2.p1 ) ) * ( ( l1.p1 >> l1.p2 ) % ( l1.p1 >> l2.p2 ) ) < 0
			&& ( ( l2.p1 >> l2.p2 ) % ( l2.p1 >> l1.p1 ) ) * ( ( l2.p1 >> l2.p2 ) % ( l2.p1 >> l1.p2 ) ) < 0;
	}
}