#define RAYMATH_IMPLEMENTATION
#include <raymath.h>
#include <raylib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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

bool collides_with_counters(Player p, Map map) {
  for (int i = 0; i < map.counter_list_size; i++) {
    if (CheckCollisionBoxes(GET_COUNTER_BBOX(p), GET_COUNTER_BBOX(map.counter_list[i]))) {
      return true;
    }
  }
  return false;
}

void get_item(Player *p, Map *map) {
  // try to get it from the counter
  {
    Ray ray = { p->pos, p->dir };
    Vector3 collision_point = { 0 };
    int nearest = -1;
    float nearest_dist = 999999.0f;
    for (int i = 0; i < map->counter_list_size; i++) {
      if (CheckCollisionRaySphereEx(ray, map->counter_list[i].pos, 1.0f, &collision_point)) { // migue pq n tem isso pra BBOX
        float distance = Vector3Distance(collision_point, p->pos);
        if (distance < nearest_dist) {
          nearest = i;
          nearest_dist = distance;
        }
      }
    }
    //printf("nearest_dist %f\n", nearest_dist);
    assert(nearest != -1);

    // TODO: tune this tolerance better?
    if (nearest_dist < 0.6f && map->counter_list[nearest].item.type) {
      // drop item if holding any
      if (p->item.type) {
        map->dropped_item_list[map->dropped_item_list_size].item = p->item;
        map->dropped_item_list[map->dropped_item_list_size].pos = p->pos;
        map->dropped_item_list_size++;
      }

      // get new item
      p->item = map->counter_list[nearest].item;
      map->counter_list[nearest].item.type = IT_UNINITIALIZED;
      return;
    }
  }

  // try to get it from the ground
  {
    int nearest = -1;
    float nearest_dist = 999999.0f;
    for (int i = 0; i < map->dropped_item_list_size; i++) {
      float distance = Vector3Distance(p->pos, map->dropped_item_list[i].pos);
      if (distance < nearest_dist) {
        nearest = i;
        nearest_dist = distance;
      }
    }

    // TODO: tune this tolerance better?
    if (nearest_dist < 0.6f) {
      // drop item if holding any
      if (p->item.type) {
        map->dropped_item_list[map->dropped_item_list_size].item = p->item;
        map->dropped_item_list[map->dropped_item_list_size].pos = p->pos;
        map->dropped_item_list_size++;
      }

      // get new item
      p->item = map->dropped_item_list[nearest].item;
      map->dropped_item_list_size--;
      for (int i = nearest; i < map->dropped_item_list_size; i++) {
        map->dropped_item_list[i] = map->dropped_item_list[i+1];
      }
    }
  }
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "geraldo alchemist");

  Map map = { 0 };

  GUI gui = {0};

  init_data(&map, &gui);

  Camera3D camera = { 0 };
  camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
  camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
  camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;                                // Camera field-of-view Y
  camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

  Player player = { 0 };

  while (!WindowShouldClose()) {
    Vector3 new_dir = { 0.0f, 0.0f, 0.0f };
    if (IsGamepadAvailable(GAMEPAD_PLAYER1)) {
      if (IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
        new_dir.z -= 1.0f;
        player.pos.z -= 0.1f;
        if (collides_with_counters(player, map)) player.pos.z += 0.1f;
      }
      if (IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
        new_dir.z += 1.0f;
        player.pos.z += 0.1f;
        if (collides_with_counters(player, map)) player.pos.z -= 0.1f;
      }
      if (IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
        new_dir.x -= 1.0f;
        player.pos.x -= 0.1f;
        if (collides_with_counters(player, map)) player.pos.x += 0.1f;
      }
      if (IsGamepadButtonDown(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
        new_dir.x += 1.0f;
        player.pos.x += 0.1f;
        if (collides_with_counters(player, map)) player.pos.x -= 0.1f;
      }
    }
    if (IsKeyDown(KEY_UP)) {
      new_dir.z -= 1.0f;
      player.pos.z -= 0.1f;
      if (collides_with_counters(player, map)) player.pos.z += 0.1f;
    }
    if (IsKeyDown(KEY_DOWN)) {
      new_dir.z += 1.0f;
      player.pos.z += 0.1f;
      if (collides_with_counters(player, map)) player.pos.z -= 0.1f;
    }
    if (IsKeyDown(KEY_LEFT)) {
      new_dir.x -= 1.0f;
      player.pos.x -= 0.1f;
      if (collides_with_counters(player, map)) player.pos.x += 0.1f;
    }
    if (IsKeyDown(KEY_RIGHT)) {
      new_dir.x += 1.0f;
      player.pos.x += 0.1f;
      if (collides_with_counters(player, map)) player.pos.x -= 0.1f;
    }
    if (Vector3Length(new_dir)) {
      player.dir = Vector3Normalize(new_dir);
    }
    if (IsKeyDown(KEY_Z) && player.item_pickup_cooldown < 0) {
      get_item(&player, &map);
      player.item_pickup_cooldown = PLAYER_ITEM_PICKUP_COOLDOWN;
    }
    if (player.item_pickup_cooldown >= 0) {
      player.item_pickup_cooldown -= GetFrameTime();
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    for (int i = 0; i < map.counter_list_size; i++) {
      Counter c = map.counter_list[i];

      DrawCube(c.pos, 1.0f, 1.0f, 1.0f, MAROON);
      DrawCubeWires(c.pos, 1.0f, 1.0f, 1.0f, YELLOW);

      if (c.item.type) {
        Vector3 item_pos = {c.pos.x, c.pos.y+0.6f, c.pos.z};
        DrawCube(item_pos, 0.2f, 0.2f, 0.2f, c.item.color);
      }
    }

    for (int i = 0; i < map.dropped_item_list_size; i++) {
      DroppedItem item = map.dropped_item_list[i];

      DrawCube(item.pos, 0.2f, 0.2f, 0.2f, item.item.color);
    }


    DrawCube(player.pos, 1.0f, 1.0f, 1.0f, MAROON);
    DrawCubeWires(player.pos, 1.0f, 1.0f, 1.0f, YELLOW);
    if (player.item.type) {
      Vector3 item_pos = {player.pos.x, player.pos.y+0.6f, player.pos.z};
      DrawCube(item_pos, 0.2f, 0.2f, 0.2f, player.item.color);
    }

    DrawGrid(10, 1.0f);

    EndMode3D();

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
