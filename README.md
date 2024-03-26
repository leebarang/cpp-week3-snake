# Snake Game
Snake Game은 C++로 작성된 게임입니다. Snake 게임은 전통적인 아케이드 게임으로, 플레이어가 뱀을 조종하여 사과를 먹고 뱀의 길이를 늘려가는 게임입니다.
이 게임 프로젝트에는 main.cpp, console.cpp, console.h 세 개의 파일이 포함되어 있습니다.

## 파일 구성
### main.cpp
* main(): 게임 초기화 후, 게임을 실행합니다.
* resetGame(): 게임을 초기화합니다.
* game(): 게임을 실행합니다.
* makeApple(): 무작위 위치 (뱀이 아닌 곳)에 사과 좌표를 생성합니다.
* endWin(), endLose(): 게임 종료 여부 판별
* handleInput(): 키 입력 감지
* moveSnake(): 뱀 이동
* restrictInScreen(): 화면 이동 방지
* drawBackground(), drawSnake(), drawApple(), drawScore(): 화면에 표시
### console.cpp
* console.h 헤더 파일과 함께 게임의 전반적인 시스템을 구축합니다.
### console.h
* 콘솔 헤더파일

## 실행 방법
프로젝트를 다운로드하고 소스 코드를 가져옵니다.
컴파일러가 C++11 이상을 지원하는지 확인합니다.

```
git clone https://github.com/leebarang/cpp-week3-snake.git snake
cd snake

g++ -Werror -std=c++11 -c main.cpp console.cpp
g++ -o main.exe main.o console.o
./main.exe
```

## 게임 규칙
* 플레이어는 화살표 키를 사용하여 뱀을 위, 아래, 왼쪽, 오른쪽으로 이동시킬 수 있습니다.
* 뱀은 화면을 가득 채울 때까지 사과를 먹어야 합니다.
* 뱀의 머리가 벽에 부딪히거나 자신의 몸에 닿으면 게임이 종료됩니다.
* 사과를 먹을 때마다 점수가 증가하며, 모든 사과를 먹으면 플레이어가 이깁니다.