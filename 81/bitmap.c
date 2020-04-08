#include <stdlib.h>
#include <stdio.h>
#pragma pack(push, 1)		//구조체를 1바이트 크기로 정렬
#define PIXEL_SIZE 3 //픽셀 한개 당 3바이트
#define PIXEL_ALIGN 4 // 픽셀 가로 한 줄은 4의 배수 크기로 저장
typedef struct _BITMAPFILEHEADER
{
	unsigned short	bfType;
	unsigned int	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned int	bfOffBits;
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER
{
	unsigned int	biSize;
	int				biWidth;
	int				biHeight;
	unsigned short	biPlanes;
	unsigned short	biBitCount;
	unsigned int	biCompression;
	unsigned int	biSizeImage;
	int				biXPelsPerMeter;
	int				biYPelsPerMeter;
	unsigned int	biClrUsed;
	unsigned int	biClrImportant;
} BITMAPINFOHEADER;

typedef struct _RGBTRIPLE
{
	unsigned char	rgbtBlue;
	unsigned char	rgbtGreen;
	unsigned char	rgbtRed;
} RGBTRIPLE;

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

	//RGB 모두 0이면 검정이고, 모두 255면 흰색. 값이 작을수록 어두워지고 클수록 밝아짐
	// 배열의 낮은 인덱스에 획이 많은 문자 배치. 높은 인덱스에 획이 적은 문자 배치. 가장 큰 인덱스에는 공백
	char ascii[] = {'#', '#', '@', '%', '=', '+', '*', ':', '-', '.', ' '};

	fpBmp = fopen("Peppers80x80.bmp", "rb");
	if (fpBmp == NULL)
		return 0;
	if (fread(&fileHeader, sizeof(BITMAPINFOHEADER), 1, fpBmp) < 1)
	{
		fclose(fpBmp);
		return 0;
	}

	/*
	매직 넘버가 MB가 맞는지 확인 (2바이트 크기의 MB를 리틀 엔디언으로 읽었으므로 MB가 된다.)
	*/
	if (fileHeader.bfType != 'MB')
	{
		fclose(fpBmp);
		return 0;
	}

	size = infoHeader.biSizeImage;
	width = infoHeader.biWidth;
	height = infoHeader.biHeight;
	padding = (PIXEL_ALIGN - ((width * PIXEL_SIZE) % PIXEL_ALIGN)) % PIXEL_ALIGN;

	if (size == 0)
	{
		size = (width * PIXEL_SIZE + padding) * height;
	}
	image = malloc(size);

	// 파일 포인터를 픽셀 데이터 시작 위치로 이동
	fseek(fpBmp, fileHeader.bfOffBits, SEEK_SET);
	if (fread(image, size, 1, fpBmp) <1)
	{
		fclose(fpBmp);
		return 0;
	}
	fclose(fpBmp);

	fpTxt = fopen("ascii.txt", "w");
	if (fpTxt == NULL)
	{
		free(image);
		return 0;
	}

	for (int y = height - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			int index = (x * PIXEL_SIZE) + (y * (width * PIXEL_SIZE)) + (padding * y);

			RGBTRIPLE *pixel = (RGBTRIPLE*)&image[index];
			unsigned char blue = pixel->rgbtBlue;
			unsigned char green = pixel->rgbtGreen;
			unsigned char red = pixel->rgbtRed;

			unsigned char gray = (red + green + blue) / PIXEL_SIZE;
			char c = ascii[gray * sizeof(ascii) / 256];
			//보통 아스키문자는 세로로 길쭉하기 때문에 가로 세로 같은 길이로 출력하면 세로로 길쭉하게됨
			//정사각형과 비슷하게 출력하기 위해 같은 문자 두번 저장
			fprintf(fpTxt, "%c%c", c, c);
		}
		fprintf(fpTxt, "\n");
	}

}


#pragma pack(pop);