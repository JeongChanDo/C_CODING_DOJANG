#include <stdio.h>

//다른 소스 파일(외부)에 있는 정적 전역 변수는 extern으로 사용 불가
//전역 변수에 static을 붙이면 변수의 범위가 파일 범위로 제한되기 때문
extern int num1;

int main()
{
	printf("%d\n", num1);
}