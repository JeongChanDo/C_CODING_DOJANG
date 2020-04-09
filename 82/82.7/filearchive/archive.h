#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

typedef struct _FILE_NODE {
	struct _FILE_NODE *next;
	FILE_DESC desc;
} FILE_NODE, *PFILE_NODE;

//프로그램에서만 사용하는 구조체
typedef struct _ARCHIVE {
	ARCHIVE_HEADER header;
	FILE *fp;
	FILE_NODE fileList;
} ARCHIVE, *PARCHIVE;

PARCHIVE initilize();
void finalize(PARCHIVE archive);
bool isExist(PARCHIVE archive, char *filename);

#endif;