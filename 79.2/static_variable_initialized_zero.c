#include <stdio.h>

static int num1;

int main()
{
	static int num2;

	//정적 전역 변수는 초깃값을 지정하지 않으면 0이 들어간다.
	printf("%d\n", num1);
	printf("%d\n", num2);
}