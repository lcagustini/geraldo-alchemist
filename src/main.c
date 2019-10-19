#define RAYMATH_IMPLEMENTATION
#include <raymath.h>
#include <raylib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define CARD_SIZE 50
#define INGREDIENT_CARD_SIZE CARD_SIZE/2
#define CARD_PADDING 5

#define PLAYER_ITEM_PICKUP_COOLDOWN 0.2f

#define GET_COUNTER_BBOX(a) (BoundingBox){(Vector3){(a).pos.x - 0.5f, \
                                                    (a).pos.y - 0.5f, \
                                                    (a).pos.z - 0.5f},\
                                          (Vector3){(a).pos.x + 0.5f, \
                                                    (a).pos.y + 0.5f, \
                                                    (a).pos.z + 0.5f}}\

#include "data.h"
#include "cards.c"
#include "data.c"
#include "map.c"
#include "input.c"

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "geraldo alchemist");

  Map map = { 0 };

  GUI gui = {0};

  init_data(&map, &gui);

  map.players[0].pos.x = -1;
  map.players[1].pos.x = 1;
  map.player_count = 2;

  Camera3D camera = { 0 };
  camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
  camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
  camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;                                // Camera field-of-view Y
  camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

  while (!WindowShouldClose()) {
    for (int i = 0; i < map.player_count; i++) {
      keyboard_input(&map, i);

      if (map.players[i].item_pickup_cooldown >= 0) {
        map.players[i].item_pickup_cooldown -= GetFrameTime();
      }
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // draw counters
    for (int i = 0; i < map.counter_list_size; i++) {
      Counter c = map.counter_list[i];

      DrawCube(c.pos, 1.0f, 1.0f, 1.0f, MAROON);
      DrawCubeWires(c.pos, 1.0f, 1.0f, 1.0f, YELLOW);

      if (c.item.type) {
        Vector3 item_pos = {c.pos.x, c.pos.y+0.6f, c.pos.z};
        DrawCube(item_pos, 0.2f, 0.2f, 0.2f, c.item.color);
      }
    }

    // draw dropped items
    for (int i = 0; i < map.dropped_item_list_size; i++) {
      DroppedItem item = map.dropped_item_list[i];

      DrawCube(item.pos, 0.2f, 0.2f, 0.2f, item.item.color);
    }

    // draw players
    for (int i = 0; i < map.player_count; i++) {
      Player p = map.players[i];

      DrawCube(p.pos, 1.0f, 1.0f, 1.0f, MAROON);
      DrawCubeWires(p.pos, 1.0f, 1.0f, 1.0f, YELLOW);
      if (p.item.type) {
        Vector3 item_pos = {p.pos.x, p.pos.y+0.6f, p.pos.z};
        DrawCube(item_pos, 0.2f, 0.2f, 0.2f, p.item.color);
      }
    }

    EndMode3D();

    // draw GUI
    for (int i = 0; i < gui.cards_len; i++) {
      Rectangle card = {
        gui.cards[i].pos.x,
        gui.cards[i].pos.y,
        get_card_width(gui.cards[i]), CARD_SIZE
      };
      DrawRectangleRounded(card, 0.2f, 0, WHITE);
      DrawRectangleRoundedLines(card, 0.2f, 0, 1, BLACK);

      for (int j = 0; j < gui.cards[i].ingredient_list_len; j++) {
        Rectangle ingredient = {
          gui.cards[i].pos.x + (j * INGREDIENT_CARD_SIZE),
          gui.cards[i].pos.y + (3 * CARD_SIZE / 4),
          INGREDIENT_CARD_SIZE, INGREDIENT_CARD_SIZE
        };
        DrawRectangleRec(ingredient, WHITE);
        DrawRectangleLines(ingredient.x, ingredient.y, ingredient.width, ingredient.height, BLACK);
      }
    }

    DrawFPS(10, 10);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
