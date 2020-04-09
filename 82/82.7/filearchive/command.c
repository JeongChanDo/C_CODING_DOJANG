#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "archive.h"
#include "file.h"


int	extract(PARCHIVE archive, char *filename)
{
	PFILE_NODE curr = archive->fileList.next;
	while (curr != NULL)
	{
		//추출할 파일이 있는지 검사
		if (strcmp(curr->desc.name, filename) == 0)
		{
			int ret = 0;
			uint32_t size = curr->desc.size;
			uint8_t *buffer = malloc(size);

			//파일 데이터가 있는 곳으로 파일 포인터를 이동 시킴
			fseek(archive->fp, curr->desc.dataOffset, SEEK_SET);

			// 파일 데이터 읽기
			if (fread(buffer, size, 1, archive->fp) < 1)
			{
				printf("아카이브 파일 읽기 실패\n");
				ret = -1;
				goto Error1;
			}

			//추출한 파일을 저장할 새 파일 생성
			FILE *fp = fopen(filename, "wb");
			if (fp == NULL)
			{
				printf("%s 파일 열기 실패 \n", filename);
				ret = -1;
				goto Error1;
			}
			if (fwrite(buffer, size, 1, fp) < 1)
			{
				printf("%s 파일 쓰기 실패 \n", filename);
				ret = -1;
				goto Error2;
			}
			printf("%s 파일 추출 성공 \n 크기 : %d\n", filename, size);

		Error2:
			fclose(fp);
		Error1:
			free(buffer);
			return ret;
		}
		curr = curr->next;
	}
	return -1;
}

void list(PARCHIVE archive)
{
	printf("파일 목록 : \n");

	PFILE_NODE curr = archive->fileList.next;
	while (curr != NULL)
	{
		printf("    %s\n", curr->desc.name);
		curr = curr->next;
	}
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

	PFILE_NODE node = archive->fileList.next;
	if (node == NULL)
	{			
		//아카이브 헤더 바로 다음으로 파일 포인터 이동 시킴
		fseek(archive->fp,sizeof(ARCHIVE_HEADER), SEEK_SET);
		// 파일 데이터의 시작 위치는 현재 파일 포인터의 위치에
		// 파일 정보 크기만큼 순방향으로 이동 시킨 값
		desc->dataOffset = ftell(archive->fp) + sizeof(FILE_DESC);
	}
	else
	{
		//마지막에 추가된 파일 데이터 위치에서 파일 크기만큼 순방향 이동
		fseek(archive->fp, node->desc.dataOffset + node->desc.size, SEEK_SET);
		//새로 추가될 데이터 시작 위치는
		// 마지막에 추가된 파일 데이터 위치에서
		// 파일 크기, 파일 정보 크기 만큼 순방향으로 이동시킨 값
		desc->dataOffset = node->desc.dataOffset + node->desc.size + sizeof(FILE_DESC);
	}
	


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