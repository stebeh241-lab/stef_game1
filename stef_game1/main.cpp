#include <iostream>
#include <raylib.h>

int main(int argc, char **argv)

{
  InitWindow(1600, 900, "le gamey");
  SetTargetFPS(60);

  // Game loop
  while (WindowShouldClose() == false) {
    // 1. Event hadling

    // 2. Update positions

    // 3. Drawing
    BeginDrawing();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
