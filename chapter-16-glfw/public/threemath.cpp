#include "threemath.h"


Float SafeAcos(Float x)
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
