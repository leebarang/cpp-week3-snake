#include "console.h"
#include "string"

#define BOARD_SIZE 20
#define MOVE_DELAY 10 // 60FPS, 1/6 약 0.166초 마다 딜레이
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

int snakeLength; // 뱀의 길이
int snakeX[(BOARD_SIZE-2) * (BOARD_SIZE-2)]; // 뱀의 x 좌표 배열
int snakeY[(BOARD_SIZE-2) * (BOARD_SIZE-2)]; // 뱀의 y 좌표 배열
int appleX, appleY; // 사과의 좌표
int score; // 점수
bool fail; // 게임 실패 여부
std::string direction; // 방향

void game();

// 사과 생성
void makeApple() {

  // 사과의 위치 랜덤 설정
  int availablePositions[(BOARD_SIZE - 2) * (BOARD_SIZE - 2)];
    int k = 0;
    for (int x = 1; x < BOARD_SIZE - 1; ++x) {
        for (int y = 1; y < BOARD_SIZE - 1; ++y) {
            bool isSnakeBody = false;
            for (int i = 0; i < snakeLength; ++i) {
                if (snakeX[i] == x && snakeY[i] == y) {
                    isSnakeBody = true;
                    break;
                }
            }
            if (!isSnakeBody) {
                availablePositions[k++] = x + y * BOARD_SIZE;
            }
        }
    }
    int randomIndex = rand() % k;
    // 사과의 좌표 설정
    appleX = availablePositions[randomIndex] % BOARD_SIZE;
    appleY = availablePositions[randomIndex] / BOARD_SIZE;
}

// 게임 초기화
void resetGame() {
  snakeLength = 1;
  snakeX[0] = BOARD_SIZE / 2;
  snakeY[0] = BOARD_SIZE / 2;
  fail = false; // 게임 종료 여부
  direction = "right";
  score = 0;
  makeApple();
}

// 게임 승리
void endWin() {
  console::draw(BOARD_SIZE/2 - 4, BOARD_SIZE/2 - 1, "YOU WIN!");
  console::draw(BOARD_SIZE/2 - 9, BOARD_SIZE/2, "Try Again? (Enter)");
  while (true) {
    // 화면을 갱신하고 다음 프레임까지 대기한다.
    console::wait();
    if (console::key(console::K_ESC))
      exit(0);
    if (console::key(console::K_ENTER)) {
        resetGame();
        game();
    }
  }
}

// 게임 패배
void endLose() {
  console::draw(BOARD_SIZE/2 - 4, BOARD_SIZE/2 - 1, "YOU LOSE!");
  console::draw(BOARD_SIZE/2 - 9, BOARD_SIZE/2, "Try Again? (Enter)");
  while (true) {
    // 화면을 갱신하고 다음 프레임까지 대기한다.
    console::wait();
    if (console::key(console::K_ESC))
      exit(0);
    if (console::key(console::K_ENTER)) {
      resetGame();
      game();
    }
  }
}

// 키 입력 감지
void handleInput() {

  if (console::key(console::K_ESC)) {
    exit(0);
  }
  if (console::key(console::K_LEFT)) {
    if (direction != "right" || snakeLength == 1)
      direction = "left";
  }
  if (console::key(console::K_RIGHT)) {
    if (direction != "left" || snakeLength == 1)
      direction = "right";
  }
  if (console::key(console::K_UP)) {
    if (direction != "down" || snakeLength == 1)
      direction = "up";
  }
  if (console::key(console::K_DOWN)) {
    if (direction != "up" || snakeLength == 1)
      direction = "down";
  }
}

// 뱀 이동
void moveSnake() {

  // 뱀의 몸통 이동
  if (snakeLength > 1) {
    for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
    }
  }
  
  // 뱀의 머리 이동
  if (direction == "left")
    snakeX[0]--;
  else if (direction == "right")
    snakeX[0]++;
  else if (direction == "up")
    snakeY[0]--;
  else if (direction == "down")
    snakeY[0]++;

  // 벽과 충돌 확인
  if (snakeX[0] == 0 || snakeX[0] == BOARD_SIZE - 1 || snakeY[0] == 0 || snakeY[0] == BOARD_SIZE - 1) {
    fail = true;
  }

  // 뱀의 몸통과 머리의 충돌 확인
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      fail = true;
      break;
    }
  }

  // 사과 먹기 확인
  if (snakeX[0] == appleX && snakeY[0] == appleY) {
    snakeLength++; // 뱀의 길이 증가
    score += 10; // 점수 증가
    makeApple(); // 새로운 사과 생성
    // 게임 종료 확인
    if (snakeLength == (BOARD_SIZE - 2) * (BOARD_SIZE - 2)) {
      endWin();
    }
  }
}

void restrictInScreen() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (snakeX[0] < 0)
    snakeX[0] = 0;
  if (snakeX[0] >= console::SCREEN_WIDTH)
    snakeX[0] = console::SCREEN_WIDTH - 1;
  if (snakeY[0] < 0)
    snakeY[0] = 0;
  if (snakeY[0] >= console::SCREEN_HEIGHT)
    snakeY[0] = console::SCREEN_HEIGHT - 1;
}

void drawBackground() {
  // 배경을 그린다.
  console::draw(0,0,WALL_LEFT_TOP_STRING);
  console::draw(BOARD_SIZE-1,0,WALL_RIGHT_TOP_STRING);
  console::draw(0,BOARD_SIZE-1,WALL_LEFT_BOTTOM_STRING);
  console::draw(BOARD_SIZE-1,BOARD_SIZE-1,WALL_RIGHT_BOTTOM_STRING);
  for (int i=1; i<BOARD_SIZE-1; i++) {
    console::draw(i,0,WALL_HORIZONTAL_STRING);
    console::draw(i,BOARD_SIZE-1,WALL_HORIZONTAL_STRING);
    console::draw(0,i,WALL_VERTICAL_STRING);
    console::draw(BOARD_SIZE-1,i,WALL_VERTICAL_STRING);
  }
}

void drawSnake() {
  // 뱀을 그린다.
  for (int i=0; i<snakeLength; i++) {
    if (i==0)
      console::draw(snakeX[i],snakeY[i], SNAKE_STRING);
    else
      console::draw(snakeX[i],snakeY[i], SNAKE_BODY_STRING);
  }
}

void drawApple() {
  // 사과를 그린다.
  console::draw(appleX, appleY, APPLE_STRING);
}

void drawScore() {
  // 점수 출력
  std::string scoreString = "Score: " + std::to_string(score);
  console::draw(BOARD_SIZE/2 - scoreString.size()/2, BOARD_SIZE, scoreString);
}

void game() {
  // 콘솔 라이브러리를 초기화한다.
  console::init();

  int stack = 0;

  while (true) {
    // 화면을 지운다.
    console::clear();

    handleInput();
    if (stack >= MOVE_DELAY) {
      moveSnake();
      stack = 0;
    }
    restrictInScreen();
    drawBackground();
    drawApple();
    drawSnake();
    drawScore();

    // 게임 종료 조건 확인
    if (fail) {
      endLose();
    } else if (snakeLength == (BOARD_SIZE - 2) * (BOARD_SIZE - 2)) {
      endWin();
    }

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    console::wait();

    stack++;
  }
}

int main() {
  resetGame();
  game();
  return 0;
}