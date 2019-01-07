#include "threemath.h"


template<typename T>
T SafeAcos(T x)
{
	if(x <= -1.0f)
	{
		return kPi;
	}

	if(x >=1.0f)
	{
		return 0.0f;
	}

	return acos(x);
}
