#include <iostream>
#include <raylib.h>
#include <raymath.h>

int main(int argc, char **argv) {
  int WIDTH = 1920;
  int HEIGHT = 1080;

  InitWindow(WIDTH, HEIGHT, "le gamey");

  // textures
  Texture2D player_tex = LoadTexture("assets/noobwarrior.png");
  float player_scale = 0.1f;
  float player_width = player_tex.width * player_scale;
  float player_height = player_tex.height * player_scale;

  Texture2D ground_tex = LoadTexture("assets/forestfloor.png");

  SetTextureFilter(ground_tex, TEXTURE_FILTER_BILINEAR);
  SetTextureWrap(ground_tex, TEXTURE_WRAP_REPEAT);

  Vector2 player_pos = {0, 0};
  float player_rotation = 0.0f;
  float camera_rotation = 0.0f;
  float speed = 300.0f;

  Camera2D camera = {0};
  camera.offset = {WIDTH / 2.0f, HEIGHT * (2.0f / 3)};
  camera.zoom = 1.0f;

  SetTargetFPS(120);

  Vector2 mouse_anchor = {0, 0};
  bool mouse_locked = false;

  // Game loop
  while (!WindowShouldClose()) {
    // 1. Event hadling
    float dt = GetFrameTime();

    Vector2 mouse_delta = GetMouseDelta();

    // 2. Update positions
    bool double_mouse = false;
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) &&
        IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      double_mouse = true;
    }
    if (double_mouse) {
      HideCursor();
      player_rotation = camera_rotation;
      float rotationchange = mouse_delta.x * 0.2f;
      player_rotation += rotationchange;
      camera_rotation += rotationchange;
      if (!IsKeyDown(KEY_S)) {
        Vector2 forward = {0, -1};
        forward = Vector2Rotate(forward, player_rotation * DEG2RAD);
        player_pos = Vector2Add(player_pos, Vector2Scale(forward, speed * dt));
      }
    } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      HideCursor();
      float rotationchange = mouse_delta.x * 0.2f;
      player_rotation += rotationchange;
      camera_rotation += rotationchange;
    } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      HideCursor();
      camera_rotation += mouse_delta.x * 0.2f;
    } else {
      ShowCursor();
    }

    Vector2 move_direction = {0, 0};
    if (double_mouse == false && IsKeyDown(KEY_W))
      move_direction.y -= 1;
    if (IsKeyDown(KEY_S))
      move_direction.y += 1;
    if (IsKeyDown(KEY_A))
      move_direction.x -= 1;
    if (IsKeyDown(KEY_D))
      move_direction.x += 1;

    if (Vector2Length(move_direction) > 0) {
      move_direction = Vector2Normalize(move_direction);

      move_direction = Vector2Rotate(move_direction, player_rotation * DEG2RAD);

      player_pos.x += move_direction.x * speed * dt;
      player_pos.y += move_direction.y * speed * dt;
    }

    camera.target = player_pos;
    camera.rotation = -camera_rotation; // Ska inte vara -player rotation?

    // 3. Drawing
    BeginDrawing();
    ClearBackground(DARKGRAY);
    BeginMode2D(camera);

    // Rita bakgrund
    float map_size = 10000.0f;
    Rectangle ground_rect = {-map_size / 2, -map_size / 2, map_size, map_size};
    Rectangle source = {0, 0, map_size, map_size};
    DrawTextureRec(ground_tex, source, {ground_rect.x, ground_rect.y}, WHITE);

    // rita player
    Rectangle source_rectangle = {0.0f, 0.0f, (float)player_tex.width,
                                  (float)player_tex.height};

    Rectangle dest_rectangle = {player_pos.x, player_pos.y, player_width,
                                player_height};

    Vector2 origin = {(float)player_width / 2, (float)player_height / 2};

    DrawTexturePro(player_tex, source_rectangle, dest_rectangle, origin,
                   player_rotation, WHITE);
    EndMode2D();

    DrawText("WASD: Move/Strafe | Mus-V: Titta | Mus-H: Sväng", 10, 10, 20,
             RAYWHITE);

    EndDrawing();
  }

  UnloadTexture(player_tex);
  UnloadTexture(ground_tex);
  CloseWindow();
  return 0;
}
