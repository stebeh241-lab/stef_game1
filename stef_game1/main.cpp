#include <iostream>
#include <raylib.h>
#include <raymath.h>

struct Tree {
  Vector2 pos;
  float scale;
};

Tree big_trees[5] = {{{500, 500}, 0.8f},
                     {{-300, 800}, 0.9f},
                     {{1200, -200}, 0.85f},
                     {{100, -600}, 0.8f},
                     {{-900, -300}, 0.9f}};
Tree bush[5] = {{{200, 200}, 0.4f},
                {{600, -400}, 0.5f},
                {{-400, -100}, 0.45f},
                {{800, 900}, 0.5f},
                {{-100, 300}, 0.4f}};

const int MAX_TREES = 60;
Tree forest[MAX_TREES];

Vector2 ResolveCollision(Vector2 pPos, float pRad, Vector2 tPos, float tRad) {
  float distance = Vector2Distance(pPos, tPos);
  float minDistance = pRad + tRad;

  if (distance < minDistance) {
    if (distance == 0)
      return {pPos.x + 0.1f, pPos.y}; // Förhindra divison med noll
    // Räkna ut hur mycket de överlappar
    float overlap = minDistance - distance;
    // Skapa en riktningsvektor från trädet till spelaren
    Vector2 normal = Vector2Normalize(Vector2Subtract(pPos, tPos));
    // Knuffa ut spelaren med överlappet
    return Vector2Add(pPos, Vector2Scale(normal, overlap));
  }
  return pPos;
}

int main(int argc, char **argv) {
  int WIDTH = 2560;
  int HEIGHT = 1440;

  InitWindow(WIDTH, HEIGHT, "le gamey");

  // textures
  Texture2D player_tex = LoadTexture("assets/noobwarrior.png");
  float player_scale = 0.1f;
  float player_width = player_tex.width * player_scale;
  float player_height = player_tex.height * player_scale;

  float radius_player = 15.0f;

  Texture2D ground_tex = LoadTexture("assets/forestfloor.png");

  Texture2D tree_tex = LoadTexture("assets/tree_big.png");

  float radius_tree_trunk = 80.0f;

  Texture2D bush_tex = LoadTexture("assets/bush.png");

  float radius_bush_trunk = 120.0f;

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
  ToggleFullscreen();

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

    // --- Kollisionshantering ---

    // Kolla mot alla små träd
    for (int i = 0; i < 5; i++) {
      player_pos = ResolveCollision(player_pos, radius_player, bush[i].pos,
                                    radius_bush_trunk);
    }

    // Kolla mot alla stora träd
    for (int i = 0; i < 5; i++) {
      player_pos = ResolveCollision(player_pos, radius_player, big_trees[i].pos,
                                    radius_tree_trunk);
    }

    float leftWall = -WIDTH / 4.0f;
    float rightWall = WIDTH / 4.0f;

    for (int i = 0; i < MAX_TREES; i++) {
      // Sprid ut dem vertikalt över ett stort område (t.ex. 4000px)
      forest[i].pos.y = GetRandomValue(-2000, 2000);

      // Placera hälften till vänster och hälften till höger
      if (i % 2 == 0)
        forest[i].pos.x = GetRandomValue(-WIDTH / 2, leftWall);
      else
        forest[i].pos.x = GetRandomValue(rightWall, WIDTH / 2);

      forest[i].scale = GetRandomValue(7, 12) / 10.0f;
    }

    if (player_pos.x - radius_player < leftWall)
      player_pos.x = leftWall + radius_player;
    if (player_pos.y + radius_player > rightWall)
      player_pos.x = rightWall - radius_player;

    // Uppdatera kameran först efter att kollisionen är löst
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

    // rita träden i mellanlagret

    for (int i = 0; i < 5; i++) {
      float parallaxFactor = 0.10f; // Hur mycket "högre" de är
      Vector2 displayPos = {
          bush[i].pos.x + (bush[i].pos.x - player_pos.x) * parallaxFactor,
          bush[i].pos.y + (bush[i].pos.y - player_pos.y) * parallaxFactor};
      // VIKTIGT: Räkna ut offset för att centrera bilden
      Vector2 offset = {(bush_tex.width * bush[i].scale) / 2.0f,
                        (bush_tex.height * bush[i].scale) / 2.0f};

      // Rita själva busken (centrerad)
      DrawTextureEx(bush_tex, Vector2Subtract(displayPos, offset), 0,
                    bush[i].scale, WHITE);

      // DEBUG: Rita hitboxen på marken (utan parallax!)
      DrawCircleLines(bush[i].pos.x, bush[i].pos.y, radius_bush_trunk, RED);
    }

    // rita träden i topplager
    for (int i = 0; i < 5; i++) {
      float parallaxFactor = 0.35f; // Högre värde = närmare kameran
      Vector2 displayPos = {
          big_trees[i].pos.x +
              (big_trees[i].pos.x - player_pos.x) * parallaxFactor,
          big_trees[i].pos.y +
              (big_trees[i].pos.y - player_pos.y) * parallaxFactor};
      Vector2 offset = {(tree_tex.width * big_trees[i].scale) / 2.0f,
                        (tree_tex.height * big_trees[i].scale) / 2.0f};

      // Skugga (också centrerad)
      DrawTextureEx(tree_tex,
                    Vector2Subtract(Vector2Add(displayPos, {20, 20}), offset),
                    0, big_trees[i].scale, {0, 0, 0, 100});
      // Träd (centrerat)
      DrawTextureEx(tree_tex, Vector2Subtract(displayPos, offset), 0,
                    big_trees[i].scale, WHITE);

      // DEBUG: Rita hitboxen på marken
      DrawCircleLines(big_trees[i].pos.x, big_trees[i].pos.y, radius_tree_trunk,
                      RED);
    }

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
