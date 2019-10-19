#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_COUNTERS 50

#include "data.h"
#include "data.c"

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "geraldo alchemist");

  Map map = { 0 };
  init_data(&map);

  Camera3D camera = { 0 };
  camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
  camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
  camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;                                // Camera field-of-view Y
  camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

  Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

  while (!WindowShouldClose()) {
    if (IsGamepadAvailable(GAMEPAD_PLAYER1)) {
      if (IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_UP)) cubePosition.z -= 0.1f;
      if (IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) cubePosition.z += 0.1f;
      if (IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) cubePosition.x -= 0.1f;
      if (IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) cubePosition.x += 0.1f;
    }
    if (IsKeyDown(KEY_UP)) cubePosition.z -= 0.1f;
    if (IsKeyDown(KEY_DOWN)) cubePosition.z += 0.1f;
    if (IsKeyDown(KEY_LEFT)) cubePosition.x -= 0.1f;
    if (IsKeyDown(KEY_RIGHT)) cubePosition.x += 0.1f;

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    for (int i = 0; i < map.counter_list_size; i++) {
      DrawCube(map.counter_list[i].pos, 1.0f, 1.0f, 1.0f, MAROON);
      DrawCubeWires(map.counter_list[i].pos, 1.0f, 1.0f, 1.0f, YELLOW);
    }

    DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, MAROON);
    DrawCubeWires(cubePosition, 1.0f, 1.0f, 1.0f, YELLOW);

    DrawGrid(10, 1.0f);

    EndMode3D();

    DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

    DrawFPS(10, 10);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
