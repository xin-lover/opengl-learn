#include <stdio.h>
#include "../threemath.h"

int main()
{
	Vector3 a;
	a.Set(1,2,3);

	printf("x:%f\n",a.x());

	Vector3 b = a * 10;

	printf("x:%f\n",b.x());

	Vector3 c(10,10,10);
	Vector3 d = a + c;
	printf("y:%f\n",d.y());
	return 0;
}
