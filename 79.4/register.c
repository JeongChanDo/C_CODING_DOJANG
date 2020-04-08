#include <stdio.h>
#include <stdlib.h>

int main()
{
	register int num1 = 10;

	printf("%d\n", num1);
	//printf("%p\n", &num1); //num1은 메모리에 없으므로 메모리 주소를 구할 수 없음.

	register int *numPtr = malloc(sizeof(int));

	//레지스터 변수에 메모리 주소는 저장할 수 있으므로 역참조 연산자를 사용할 수 있음
	*numPtr = 20;
	printf("%d\n", *numPtr);
	free(numPtr);
}