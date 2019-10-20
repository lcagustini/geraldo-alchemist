#define RAYMATH_IMPLEMENTATION
#include <raymath.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include <raylib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define CARD_SIZE 50
#define INGREDIENT_CARD_SIZE CARD_SIZE/2
#define CARD_PADDING 5

#define MIN_RANGE_TO_USE_DEVICE 0.75f
#define MIN_RANGE_TO_PICKUP_ITEM 0.75f

#define PLAYER_ITEM_PICKUP_COOLDOWN 0.2f
#define PLAYER_SPEED 5.0f

#define FLOOR_SIZE 2.0f

#define CAULDRON_SPEED 20.0f
#define CENTRIFUGE_SPEED 20.0f
#define SCALE_SPEED 5.0f

#define GET_COUNTER_BBOX(a) (BoundingBox){(Vector3){(a).pos.x - 0.5f, \
                                                    (a).pos.y - 0.5f, \
                                                    (a).pos.z - 0.5f},\
                                          (Vector3){(a).pos.x + 0.5f, \
                                                    (a).pos.y + 0.5f, \
                                                    (a).pos.z + 0.5f}}\

#define GLSL_VERSION 330

#include "data.h"

Model global_counter_model;
Model global_character_model;
Model global_scale_model;
Model global_cauldron_model;
Model global_centrifuge_open_model;
Model global_centrifuge_closed_model;

PotionProcess global_potion_process_list[] = {
  {
    .before = {IT_INGREDIENT1},
    .before_len = 1,
    .process = DT_MASHER,
    .after = IT_INGREDIENT1_POWDER
  },
  {
    .before = {IT_INGREDIENT2},
    .before_len = 1,
    .process = DT_MASHER,
    .after = IT_INGREDIENT2_POWDER
  },
  {
    .before = {IT_INGREDIENT3},
    .before_len = 1,
    .process = DT_SCALE,
    .after = IT_INGREDIENT3_SMALL
  },
  {
    .before = {IT_INGREDIENT4},
    .before_len = 1,
    .process = DT_SCALE,
    .after = IT_INGREDIENT4_SMALL,
  },
  {
    .before = {IT_INGREDIENT3_SMALL, IT_INGREDIENT4_SMALL},
    .before_len = 2,
    .process = DT_CAULDRON,
    .after = IT_INGREDIENT_COOKED_3_4,
  },
  {
    .before = {IT_INGREDIENT3_SMALL},
    .before_len = 1,
    .process = DT_CENTRIFUGE,
    .after = IT_INGREDIENT_SOLID_3,
  }
};
int global_potion_process_list_len;

Color global_item_colors[] = {
  MAGENTA,

  GREEN,
  YELLOW,
  PURPLE,
  RED,

  DARKGREEN,
  GOLD,
  DARKPURPLE,
  MAROON,

  PINK,
  VIOLET,

  BLACK
};

#include "util.c"
#include "cards.c"
#include "data.c"
#include "map.c"
#include "input.c"

ItemType get_recipe_result(ItemType input[], int input_len, DeviceType process) {
  for (int j = 0; j < global_potion_process_list_len; j++) {
    if (global_potion_process_list[j].process == process &&
        global_potion_process_list[j].before_len == input_len) {
      sort(global_potion_process_list[j].before, input_len);
      sort(input, input_len);
      for (int i = 0; i < input_len; i++) {
        if (global_potion_process_list[j].before[i] != input[i]) goto for_continue;
      }
      return global_potion_process_list[j].after;
    }
for_continue: ;
  }
  return IT_GARBAGE;
}

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "geraldo alchemist");

  Mesh plane_mesh = GenMeshPlane(FLOOR_SIZE, FLOOR_SIZE, 1, 1);
  Model floor_model = LoadModelFromMesh(plane_mesh);
  floor_model.materials[0].maps[MAP_DIFFUSE].texture = LoadTexture("assets/wood.png");
  GenTextureMipmaps(&floor_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_scale_model = LoadModel("assets/scale.obj");
  SetMaterialTexture(&global_scale_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/scale_text.png"));
  GenTextureMipmaps(&global_scale_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_counter_model = LoadModel("assets/balcao.obj");
  SetMaterialTexture(&global_counter_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/balcao_text.png"));
  GenTextureMipmaps(&global_counter_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_cauldron_model = LoadModel("assets/cauldron.obj");
  SetMaterialTexture(&global_cauldron_model.materials[0], MAP_DIFFUSE,
    LoadTexture("assets/cauldron_text.png"));
  GenTextureMipmaps(&global_cauldron_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_centrifuge_open_model = LoadModel("assets/centrifuge_open.obj");
  SetMaterialTexture(&global_centrifuge_open_model.materials[0], MAP_DIFFUSE,
    LoadTexture("assets/centrifuge_text.png"));
  GenTextureMipmaps(&global_centrifuge_open_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_centrifuge_closed_model = LoadModel("assets/centrifuge_closed.obj");
  SetMaterialTexture(&global_centrifuge_closed_model.materials[0], MAP_DIFFUSE,
    LoadTexture("assets/centrifuge_text.png"));
  GenTextureMipmaps(&global_centrifuge_closed_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_character_model = LoadModel("assets/personagem.obj");
  //Texture2D texture = LoadTexture("assets/balcao_text.png");
  //SetMaterialTexture(&global_character_model.materials[0], MAP_DIFFUSE, texture);
  //GenTextureMipmaps(&global_character_model.materials[0].maps[MAP_DIFFUSE].texture);

  Shader shader = LoadShader("src/lighting.vs", "src/lighting.fs");
  shader.locs[LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
  shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){ 0.3f, 0.3f, 0.3f, 1.0f }, UNIFORM_VEC4);

  global_counter_model.materials[0].shader = shader;
  global_scale_model.materials[0].shader = shader;
  global_cauldron_model.materials[0].shader = shader;
  global_character_model.materials[0].shader = shader;
  global_centrifuge_open_model.materials[0].shader = shader;
  global_centrifuge_closed_model.materials[0].shader = shader;

  CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 3.0f, 20.0f, 7 }, (Vector3){ 0.0f, 0.0f, 0.0f }, (Color){ 255, 255, 255, 255 }, shader);

  Map map = { 0 };

  GUI gui = {0};

  init_data(&map, &gui);
  map.players[0].pos.x = -1;
  map.players[0].model = global_character_model;
  map.players[1].pos.x = 1;
  map.players[1].model = global_character_model;
  map.player_count = 2;

  global_potion_process_list_len = sizeof(global_potion_process_list)/sizeof(PotionProcess);

  Camera3D camera = { 0 };
  camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
  camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
  camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;                                // Camera field-of-view Y
  camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(GRAY);

    BeginMode3D(camera);

    for (int i = 0; i < map.player_count; i++) {
      Player *p = &map.players[i];

      keyboard_input(&map, i);

      if (p->item_pickup_cooldown >= 0) {
        p->item_pickup_cooldown -= GetFrameTime();
      }

      switch (p->current_action) {
        case DT_SCALE:
          map.scale_list[p->current_action_id].progress -= GetFrameTime();
          if (map.scale_list[p->current_action_id].progress <= 0) {
            printf("%d\n", map.scale_list[p->current_action_id].item);
            ItemType item_input[] = { map.scale_list[p->current_action_id].item };
            map.scale_list[p->current_action_id].item = get_recipe_result(item_input, 1, DT_SCALE);
            p->current_action = DT_NONE;
          }
          break;
        default:
          break;
      }

      for (int i = 0; i < map.centrifuge_list_size; i++) {
        if (map.centrifuge_list[i].progress > 0) {
          map.centrifuge_list[i].progress -= GetFrameTime();
        }
        else if (map.centrifuge_list[i].item && map.centrifuge_list[i].progress != -1.0f) {
          ItemType input[] = { map.centrifuge_list[i].item };
          map.centrifuge_list[i].item =
            get_recipe_result(input, 1, DT_CENTRIFUGE);
          map.centrifuge_list[i].progress = -1.0f;
        }
      }

      for (int i = 0; i < map.cauldron_list_size; i++) {
        if (map.cauldron_list[i].progress > 0) {
          map.cauldron_list[i].progress -= GetFrameTime();
        }
        else if (map.cauldron_list[i].items_size) {
          map.cauldron_list[i].items[0] =
            get_recipe_result(map.cauldron_list[i].items,
                map.cauldron_list[i].items_size,
                DT_CAULDRON);
          map.cauldron_list[i].items_size = 1;
          map.cauldron_list[i].progress = CAULDRON_SPEED;
        }
      }
    }

    // draw floor
    for (int i = -10; i < 10; i++) {
      for (int j = -10; j < 10; j++) {
        DrawModel(floor_model, (Vector3) { i * FLOOR_SIZE, 0, j * FLOOR_SIZE }, 1.0f, WHITE);
      }
    }

    // draw counters
    for (int i = 0; i < map.counter_list_size; i++) {
      Counter c = map.counter_list[i];

      DrawModel(c.model, Vector3Zero(), 1.0f, WHITE);

      if (c.item) {
        Vector3 item_pos = {c.pos.x, c.pos.y+1.1f, c.pos.z};
        DrawCube(item_pos, 0.2f, 0.2f, 0.2f, global_item_colors[c.item]);
      }
    }

    // draw scales
    for (int i = 0; i < map.scale_list_size; i++) {
      Scale s = map.scale_list[i];

      DrawModel(s.model, Vector3Zero(), 1.0f, WHITE);
      if (s.item) {
        Vector3 item_pos = {s.pos.x, s.pos.y+1.3f, s.pos.z};
        DrawCube(item_pos, 0.2f, 0.2f, 0.2f, global_item_colors[s.item]);
      }
    }

    // draw centrifuges
    for (int i = 0; i < map.centrifuge_list_size; i++) {
      Centrifuge s = map.centrifuge_list[i];

      DrawModel(s.item ? s.model_closed : s.model_open, Vector3Zero(), 1.0f, WHITE);
      if (s.item) {
        Vector3 item_pos = {s.pos.x, s.pos.y+1.3f, s.pos.z};
        DrawCube(item_pos, 0.2f, 0.2f, 0.2f, global_item_colors[s.item]);
      }
    }

    // draw cauldrons
    for (int i = 0; i < map.cauldron_list_size; i++) {
      Cauldron c = map.cauldron_list[i];

      DrawModel(c.model, Vector3Zero(), 1.0f, WHITE);

      for (int j = 0; j < c.items_size; j++) {
        Vector3 item_pos = {c.pos.x, c.pos.y+1.3f+(j*0.3), c.pos.z};
        DrawCube(item_pos, 0.2f, 0.2f, 0.2f, global_item_colors[c.items[j]]);
      }
    }

    // draw dropped items
    for (int i = 0; i < map.dropped_item_list_size; i++) {
      DroppedItem item = map.dropped_item_list[i];
      DrawCube(item.pos, 0.2f, 0.2f, 0.2f, global_item_colors[item.item]);
    }

    // draw players
    for (int i = 0; i < map.player_count; i++) {
      Player p = map.players[i];

      //DrawCube(p.pos, 1.0f, 1.0f, 1.0f, MAROON);
      //DrawCubeWires(p.pos, 1.0f, 1.0f, 1.0f, YELLOW);
      DrawModel(p.model, p.pos, 0.5f, WHITE);
      if (p.item) {
        Vector3 item_pos = {p.pos.x, p.pos.y+0.6f, p.pos.z};
        item_pos = Vector3Add(item_pos, Vector3Scale(p.dir, 0.6f));
        DrawCube(item_pos, 0.2f, 0.2f, 0.2f, global_item_colors[p.item]);
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
