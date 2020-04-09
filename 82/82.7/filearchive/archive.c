#define _CRT_SECURE_NO_WARNINGS //fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "archive.h"
#include "file.h"

#define ARCHIVE_NAME "archive.bin"	//아카이브 파일 이름

PARCHIVE initialize()
{
	PARCHIVE archive = malloc(sizeof(ARCHIVE));
	memset(archive, 0, sizeof(ARCHIVE));

	FILE *fp = fopen(ARCHIVE_NAME, "r+b");
	if (fp == NULL)
	{
		fp = fopen(ARCHIVE_NAME, "w+b");
		if (fp == NULL)
			return NULL;
		archive->header.magic = 'AF'; //매직넘버 AF저장(리틀 엔디언에서는 FA로 저장됨)
		archive->header.version = 1;

		//아카이브 파일에 아카이브 헤더 저장
		if (fwrite(&archive->header, sizeof(ARCHIVE_HEADER), 1, fp) < 1)
		{
			printf("아카이브 헤더 쓰기 실패");
			fclose(fp);
			return (NULL);
		}
	}
	else
	{
		if (feard(&archive->header, sizeof(ARCHIVE_HEADER),1 , fp) < 1)
		{
			printf("아카이브 헤더 읽기 실패 \n");
			fclose(fp);
			return NULL;
		}
	}
	
	if (archive->header.magic != 'AF')
	{
		printf("아카이브 파일이 아닙니다.\n");
		fclose(fp);
		return (NULL);
	}

	// check version of archive file.
	if (archive->header.version != 1)
	{
		printf("version of archive file does not match\n");
		fclose(fp);
		return NULL;
	}
	//assign pointer of archive file
	archive->fp = fp;

	//get file size from file descripter pointer
	uint32_t size = getFileSize(fp);
	//get current position of file pointer
	uint32_t currPos = ftell(fp);

	while (size > currPos)
	{
		PFILE_NODE node = malloc(sizeof(FILE_NODE));
		memset(node, 0, sizeof(FILE_NODE));
		
		//read file info
		if (fread(&node->desc, sizeof(FILE_DESC), 1, fp) < 1)
		{
			printf("reading archive file failed\n");
			free(node);
			finalize(archive);
			return NULL;
		}
		// add FILE INFO NODE to linked list
		node->next = archive->fileList.next;
		archive->fileList.next = node;
		//move to next file info position by adding current pointer and file size
		currPos = ftell(fp) + node->desc.size;
		fseek(fp, currPos, SEEK_SET);
	}
	return archive;
}

void finalize(PARCHIVE archive)
{
	PFILE_NODE curr = archive->fileList.next;
	while (curr != NULL)
	{
		PFILE_NODE next = curr->next;
		free(curr);
		curr = next;
	}
	//close pointer of archive file
	fclose(archive->fp);
	// free archive main struct
	free(archive);
}

bool isExist(PARCHIVE archive, char *filename)
{
	PFILE_NODE curr = archive->fileList.next;
	while (curr != NULL)
	{
		if (strcmp(curr->desc.name, filename) == 0)
			return true;
		curr = curr->next;
	}
	return false;
}