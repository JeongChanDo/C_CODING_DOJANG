# 비트맵 파일 포맷 알아보기

비트맵 파일은 바이너리 형식으로 텍스트 편집기로 열어도 볼 수 없음.

비트맵 파일의 픽셀을 읽으러면 비트맵 파일의 구조를 이해 해야함.

## 비트맵 파일 구조
픽셀당      : 1,2,4,8비트 | 16,24,32비트  
비트맵 파일 헤더 : 14byte  |  14byte  
비트맵 정보 헤더 : 40byte  |  40byte  
(DIB 헤더)  
색상 테이블 :     가변     |     x  
픽셀 데이터 :     가변     |    가변  

이 예제에서 가장 널리 사용하는 24비트맵을 사용하므로 색상 테이블 처리 필요 없음

## 비트맵 파일 
비트맵 파일 헤더 : 비트맵 파일 식별 정보, 파일 크기, 데이터 위치 등  
DIB(device independent Bitmap) 헤더 : 가로, 세로 크기 , 해상도, 픽셀 비트 수 등 그림 정보  
픽셀 데이터 : 그림 실제 색상 정보. 픽셀당 색상수가 16비트 미만일 시 색상 테이블에 색상 정보 사용하고, 픽셀에는 테이블 인덱스 저장.  

## 비트맵 파일 헤더 구조
멤버         크기(바이트)  설명  
bfType      - 2 - BMP 파일 매직 넘버. 비트맵 파일이 맞는지 확인. ASCII코드로 0x42(B), 0x4D(M)이 저장  
bfSize      - 4 - 파일 크기(바이트)  
bfReserved1 - 2 - 예약된 공간. 사용 x  
bfReserved2 - 2 - 예약된 공간. 사용 x  
bfOffBits   - 4 - 비트맵 데이터 시작 위치  

## 비트맵 정보 헤더 구조
|맴버|크기(바이트)|설명|
|--|----------|---|  
|biSize			|4|현재 비트맵 정보 헤더의 크기||  
|biWidth		|4|비트맵 가로 크기(픽셀)|
|biHeight		|4|비트맵 세로 크기(픽셀)|
|biPlanes		|2|사용하는 색상 판 수. 항상 1|
|bitBitCount	|2|픽셀 하나를 표현하는 비트 수 |
|biCompression	|4|압축 방식. 보통 비트맵은 압축 안하므로 0|
|biSizeImage	|4|비트맵 이미지의 픽셀 데이터 크기(압축안된크기)|
|biXPelsPerMeter|4|가로 해상도(미터당 픽셀)|  
|biYPelsPerMeter|4|세로 해상도(미터당 픽셀)|
|biClrUsed		|4|색상 테이블에서 실제 사용하는 색상수|
|biClrImportant	|4|	비트맵을 표현하기위해 필요한 색상 인덱스 수|  
* 24비트 피브맵은 픽셀을 BGR 순으로 저장하며 각 생상 크기는 1바이트. 즉, 픽셀당 3바이트 사용  
* 색상이 파, 초, 빨 3색이라 RGBTRIPLE 구조체의 이름에 triple이 들어갑니다.  
* 구조체 맴버에서 rgb 뒤에 붙는 t도 triple을 의미함.  


## 비트맵 파일을 ASCII 문자로 저장하는 과정
1. 비트맵 파일 열기
2. 비트맵 파일 헤더 읽기
3. 매직 넘버 검사
4. 비트맵 정보 헤더 구조체 일기
5. 24비트맵 여부 검사
6. 픽셀 데이터 남는 공간 구하기
7. 픽셀 데이터 일기
8. 비트맵 파일 닫기
9. 텍스트 파일 열기
----------------------------
10. 픽셀로 흑백값 구함
11. 흑백값으로 ASCII문자 인덱스 구함
12. ASCII문자를 텍스트 파일에 쓰기
----------------------------
13. 10-12 가로X세로 크기만큼 반복
14. 텍스트 파일 닫기




## task 설정
- task.json 에서 자주 사용하는 명령 등록
```{.json}
{
    "version": "2.0.0",
    "runner": "terminal",
    "type": "shell",
    "echoCommand": true,
    "presentation" : { "reveal": "always" },
    "tasks": [
          //C++ 컴파일
          {
            "label": "save and compile for C++",
            "command": "g++",
            "args": [
                "${file}",
                "-std=c++11",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}"
            ],
            "group": "build",

            //컴파일시 에러를 편집기에 반영
            //참고:   https://code.visualstudio.com/docs/editor/tasks#_defining-a-problem-matcher

            "problemMatcher": {
                "fileLocation": [
                    "relative",
                    "${workspaceRoot}"
                ],
                "pattern": {
                    // The regular expression. 
                   //Example to match: helloWorld.c:5:3: warning: implicit declaration of function 'prinft'
                    "regexp": "^(.*):(\\d+):(\\d+):\\s+(warning error):\\s+(.*)$",
                    "file": 1,
                    "line": 2,
                    "column": 3,
                    "severity": 4,
                    "message": 5
                }
            }
        },
        // 바이너리 실행(Ubuntu)

        {
            "label": "execute",
            "command": "cd ${fileDirname} && ./${fileBasenameNoExtension}",
            "group": "test"
        }
	]
}
```
https://ldgeao99.tistory.com/203

## 디버깅 설정
- launch.json
```{.json}
{
    // IntelliSense를 사용하여 가능한 특성에 대해 알아보세요.
    // 기존 특성에 대한 설명을 보려면 가리킵니다.
    // 자세한 내용을 보려면 https://go.microsoft.com/fwlink/?linkid=830387을(를) 방문하세요.
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(lldb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}/${fileBasenameNoExtension}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "lldb"
        }
    ]
}
```
https://ldgeao99.tistory.com/203


## vscode task.json 변수 치환

When authoring tasks configurations, it is often useful to have a set of predefined common variables. VS Code supports variable substitution inside strings in the tasks.json file and has the following predefined variables:  

- ${workspaceFolder} the path of the workspace folder that contains the tasks.json file  
- ${workspaceFolderBasename} the name of the workspace folder that contains the tasks.json file without any slashes (/)  
- ${file} the current opened file  
- ${relativeFile} the current opened file relative to the workspace * folder containing the file  
- ${fileBasename} the current opened file's basename  
- ${fileBasenameNoExtension} the current opened file's basename without the extension  
- ${fileDirname} the current opened file's dirname  
- ${fileExtname} the current opened file's extension  
- ${cwd} the task runner's current working directory on startup  
- ${lineNumber} the current selected line number in the active file  

https://cartiertk.tistory.com/52