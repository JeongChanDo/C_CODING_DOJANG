
#pragma pack(push, 1)		//구조체를 1바이트 크기로 정렬

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

#pragma pack(pop);