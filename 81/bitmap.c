#include "bitmap.h"
#include <stdio.h>
#define PIXEL_SIZE 3 //픽셀 한개 당 3바이트
#define PIXEL_ALIGN 4 // 픽셀 가로 한 줄은 4의 배수 크기로 저장

int main()
{
	FILE *fpBmp;					//비트맵 파일 포인터
	FILE *fpTxt;					//텍스트 파일 포인터
	BITMAPFILEHEADER fileHeader;	//비트맵 파일 헤더 구조체 변수
	BITMAPINFOHEADER infoHeader;	//비트맵 정보 헤더 구조체 변수

	unsigned char *image;	//픽셀 데이터 포인터
	int size;
	int width, height;
	int padding;			//픽셀 데이터 가로 크기가 4의배수가 아닌 경우 남은 공간 크기


	
}