#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
//구조체를 1바이트 크기로 정렬

typedef struct _ARCHIVE_HEADER {
   uint16_t magic;		//아카이브 파일 매직 넘버
   uint16_t version;	//아카이브 파일 버전
} ARCHIVE_HEADER, *PARCHIVE_HEADER;

typedef struct _FILE_DESC {
   char     name[256];		//파일명
   uint32_t size;			//파일크기
   uint32_t dataOffset;		//파일 데이터 위치
} FILE_DESC, *PFILE_DESC;

#pragma pack(pop)

//프로그램에서만 사용하는 구조체
typedef struct _ARCHIVE {
	ARCHIVE_HEADER header;
	FILE *fp;
} ARCHIVE, *PARCHIVE;

#define ARCHIVE_NAME "archive.bin"

//파일 크기 구하는 함수 정의
uint32_t getFileSize(FILE *fp)
{
	uint32_t size;
	//ftell : 파일 스트림의 offset을 가져오는 하수
	uint32_t currPos = ftell(fp);

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, currPos, SEEK_SET);

	return size;
}

//파일 추가 함수 정의
int append(PARCHIVE archive, char *filename)
{
	//함수가 성공했는지 실패했는지 반환값으로 표현
	int ret = 0;

	//추가할 파일 열기
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("%s file not found\n", filename);
		return -1;
	}

	uint8_t *buffer;
	uint32_t size;
	size = getFileSize(fp);
	buffer = malloc(size);

	//추가할 파일 내용 읽기
	if (fread(buffer, size, 1, fp) < 1)
	{
		printf("%s file read failed\n", filename);
		ret = -1;
		goto Error1;
	}
	//새 파일 정보 생성
	PFILE_DESC desc = malloc(sizeof(FILE_DESC));
	memset(desc, 0, sizeof(FILE_DESC));
	strcpy(desc->name, filename);
	desc->size = size;

	//아카이브 헤더 바로 다음으로 파일 포인터 이동 시킴
	fseek(archive->fp,sizeof(ARCHIVE_HEADER), SEEK_SET);
	// 파일 데이터의 시작 위치는 현재 파일 포인터의 위치에
	// 파일 정보 크기만큼 순방향으로 이동 시킨 값
	desc->dataOffset = ftell(archive->fp) + sizeof(FILE_DESC);

	// 아카이브 파일에 새 파일 정보 쓰기
	if (fwrite(desc, sizeof(FILE_DESC), 1, archive->fp) < 1)
	{
		printf("파일 정보 쓰기 실패\n");
		ret = -1;
		goto Error2;
	}

	//아카이브 파일에 새 파일 데이터 쓰기
	if (fwrite(buffer, size, 1, archive->fp) < 1)
	{
		printf("파일 데이터 쓰기 실패\n");
		ret = -1;
		goto Error2;
	}
	printf("%s 파일 추가 성공 \n크기 : %d\n", filename, size);

Error2:
	free(desc);
Error1:
	free(buffer);
	fclose(fp);
	return ret;
}

int main()
{
	PARCHIVE archive = malloc(sizeof(ARCHIVE));
	memset(archive, 0, sizeof(ARCHIVE));

	FILE *fp = fopen(ARCHIVE_NAME, "r+b");
	if (fp == NULL)
	{
		fp = fopen(ARCHIVE_NAME, "w+b");
		if (fp == NULL)
			return -1;
		//새 아카이브 헤더 생성
		archive->header.magic = 'AF'; //매직 넘버 AF저장
		archive->header.version = 1;
		//아카이브 파일에 아카이브 헤더 저장
		if (fwrite(&archive->header, sizeof(ARCHIVE_HEADER), 1, fp) < 1)
		{
			printf("아카이브 헤더 쓰기 실패 \n");
			fclose(fp);
			return -1;
		}
		//아카이브 파일 포인터 저장
		archive->fp = fp;
		append(archive, "hello.txt");
	}
	fclose(fp);
	free(archive);
}