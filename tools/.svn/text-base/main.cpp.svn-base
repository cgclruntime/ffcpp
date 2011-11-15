#include <stdio.h>
#include "parallel.h"


void func1(int a)
{
	a++;
	printf("%d ", a);
	if (a % 10 == 0)
		printf("\n");
}

void func2(int b)
{
	b++;
	printf("%d ", b);
	if (b % 10 == 0)
		printf("\n");
}

void func3(int c)
{
	printf("%d \n", c);
}

void func4(int d)
{
	printf("%d \n", d);
}

void func5(int e)
{
	printf("%d \n", e);
}

int main(int argc, char* argv[])
{
	FF_INITIALIZE(5);
	int i, j;
	/*ff::Parallel<void(int), func1> f1[128];
	for (i = 0; i < 128; i++)
	{
		f1[i](i);
	}
	ff::Parallel<void(int), func2> f2[128];
	ff::Parallel<void(int), func3> f3;
	ff::Parallel<void(int), func4> f4;
	ff::Parallel<void(int), func5> f5;
	for (j = 200; j < 328; j++)
	{
		f2[j - 200](j);
	}
	f3.wait(any(f1))(500);
	f4.wait(all(f2))(1000);
	f5.wait(f3 && f4)(2000);*/
	ff::Parallel<void(int), func5> f1, f3;
	f1(1);
	f3.wait(f1)(3);

	return 0;
}
