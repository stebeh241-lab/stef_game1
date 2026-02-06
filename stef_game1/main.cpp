#include <iostream>
#include <raylib.h>

int main(int argc, char **argv) {
  int WIDTH = 1600;
  int HEIGHT = 900;
  int ballx = WIDTH / 2;
  int bally = HEIGHT / 2;

  InitWindow(WIDTH, HEIGHT, "le gamey");
  SetTargetFPS(60);

  // Game loop
  while (WindowShouldClose() == false) {
    // 1. Event hadling
    if (IsKeyDown(KEY_W)) {
      bally -= 2;
    } else if (IsKeyDown(KEY_S)) {
      bally += 2;
    } else if (IsKeyDown(KEY_A)) {
      ballx -= 2;
    } else if (IsKeyDown(KEY_D)) {
      ballx += 2;
    }
    // 2. Update positions

    // 3. Drawing
    BeginDrawing();
    ClearBackground(BLACK);
    DrawCircle(ballx, bally, 50, RED);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
