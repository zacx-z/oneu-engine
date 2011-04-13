#include "Utility.h"
#include "Matrix.h"
#include <vector>
//тсоН
namespace OneU
{
	const Matrix Matrix::E = matIMatrix;
	namespace internal
	{
		ONEU_UTIL_API Matrix matT1;
		ONEU_UTIL_API Matrix matT2;
	}

}