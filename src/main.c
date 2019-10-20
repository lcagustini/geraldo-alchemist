#define RAYMATH_IMPLEMENTATION
#include <raymath.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include <raylib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define CARD_SIZE 50
#define INGREDIENT_CARD_SIZE CARD_SIZE/2
#define CARD_PADDING 5

#define MIN_RANGE_TO_USE_DEVICE 0.75f
#define MIN_RANGE_TO_PICKUP_ITEM 0.75f

#define PLAYER_ITEM_PICKUP_COOLDOWN 0.2f
#define PLAYER_SPEED 5.0f

#define FLOOR_SIZE 2.6f

#define MAX_WANTED_ITEMS 8

#define RECIPE_GENERATION_TIMER 20.0f;

#define CAULDRON_SPEED 15.0f
#define CENTRIFUGE_SPEED 10.0f
#define SCALE_SPEED 3.0f
#define MASHER_SPEED 3.0f

#define GET_COUNTER_BBOX(a) (BoundingBox){(Vector3){(a).pos.x - 0.5f, \
                                                    (a).pos.y - 0.5f, \
                                                    (a).pos.z - 0.5f},\
                                          (Vector3){(a).pos.x + 0.5f, \
                                                    (a).pos.y + 0.5f, \
                                                    (a).pos.z + 0.5f}}\

#define GLSL_VERSION 330

#include "data.h"

Model global_character_model;

Model global_counter_model;
Model global_scale_empty_model;
Model global_scale_full_model;
Model global_cauldron_model;
Model global_centrifuge_open_model;
Model global_centrifuge_closed_model;
Model global_masher_model;

Model global_chest_model;
Model global_delivery_model;
Model global_trashcan_model;

Model global_red_potion_model;
Model global_blue_potion_model;
Model global_yellow_potion_model;
Model global_garbage_bottle_model;
Model global_empty_bottle_model;

Model global_red_stone_model;
Model global_small_red_stone_model;
Model global_yellow_stone_model;
Model global_green_stone_model;
Model global_purple_stone_model;

Model global_crystal_model;
Model global_red_crystal_model;
Model global_blue_crystal_model;
Model global_bone_model;
Model global_small_bone_model;
Model global_white_powder_model;
Model global_red_powder_model;
Model global_green_powder_model;
Model global_blue_powder_model;
Model global_chest_model;
Model global_delivery_model;
Model global_trashcan_model;
Model global_garbage_model;

Model global_flower_model;
Model global_small_flower_model;
Model global_mushroom_model;
Model global_small_mushroom_model;

PotionProcess global_potion_process_list[] = {
  {
    .before = {IT_FLOWER},
    .before_len = 1,
    .process = DT_SCALE,
    .after = IT_SMALL_FLOWER
  },
  {
    .before = {IT_RED_ROCK},
    .before_len = 1,
    .process = DT_MASHER,
    .after = IT_RED_POWDER
  },
  {
    .before = {IT_MUSHROOM},
    .before_len = 1,
    .process = DT_SCALE,
    .after = IT_SMALL_MUSHROOM
  },
  {
    .before = {IT_BLUE_CRYSTAL},
    .before_len = 1,
    .process = DT_MASHER,
    .after = IT_BLUE_POWDER,
  },
  {
    .before = {IT_BONE},
    .before_len = 1,
    .process = DT_MASHER,
    .after = IT_WHITE_POWDER,
  },
  {
    .before = {IT_MUSHROOM},
    .before_len = 1,
    .process = DT_MASHER,
    .after = IT_GREEN_POWDER,
  },
  {
    .before = {IT_RED_ROCK},
    .before_len = 1,
    .process = DT_SCALE,
    .after = IT_SMALL_RED_ROCK,
  },
  {
    .before = {IT_BONE},
    .before_len = 1,
    .process = DT_SCALE,
    .after = IT_SMALL_BONE,
  },

  {
    .before = {IT_SMALL_FLOWER, IT_RED_POWDER, IT_SMALL_MUSHROOM},
    .before_len = 3,
    .process = DT_CAULDRON,
    .after = IT_RED_POTION,
  },
  {
    .before = {IT_BLUE_POWDER, IT_SMALL_FLOWER, IT_RED_POWDER},
    .before_len = 3,
    .process = DT_CAULDRON,
    .after = IT_YELLOW_POTION,
  },
  {
    .before = {IT_WHITE_POWDER, IT_SMALL_FLOWER, IT_GREEN_POWDER, IT_SMALL_RED_ROCK},
    .before_len = 4,
    .process = DT_CAULDRON,
    .after = IT_BLACK_POTION,
  },
  {
    .before = {IT_GREEN_POWDER, IT_SMALL_FLOWER, IT_BLUE_POWDER},
    .before_len = 3,
    .process = DT_CAULDRON,
    .after = IT_PINK_POTION,
  },
  {
    .before = {IT_SMALL_BONE, IT_SMALL_FLOWER, IT_BLUE_POWDER},
    .before_len = 3,
    .process = DT_CAULDRON,
    .after = IT_BLUE_POTION,
  },

  {
    .before = {IT_RED_POTION},
    .before_len = 1,
    .process = DT_CENTRIFUGE,
    .after = IT_RED_CRYSTAL,
  },
  {
    .before = {IT_YELLOW_POTION},
    .before_len = 1,
    .process = DT_CENTRIFUGE,
    .after = IT_YELLOW_CRYSTAL,
  },
  {
    .before = {IT_BLACK_POTION},
    .before_len = 1,
    .process = DT_CENTRIFUGE,
    .after = IT_LITTLE_MAN,
  }
};
int global_potion_process_list_len;

ItemType global_available_recipes[MAX_WANTED_ITEMS];
int global_available_recipes_count;

ItemType global_wanted_items[MAX_WANTED_ITEMS];
int global_wanted_items_start;
int global_wanted_items_end;

Model global_item_models[IT_MAX];

int global_delivered_items = 0;

Texture2D global_item_icons[IT_MAX];
float global_item_icons_scale[IT_MAX];

GUI gui = {0};

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
  return process == DT_CAULDRON ? IT_GARBAGE_BOTTLE : IT_GARBAGE;
}

int main(void) {
  float recipe_generation_timer = RECIPE_GENERATION_TIMER;

  SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);

  InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "geraldo alchemist");

  global_small_mushroom_model = LoadModel("assets/mushroom.obj");
  SetMaterialTexture(&global_small_mushroom_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/mushroom_text.png"));
  GenTextureMipmaps(&global_small_mushroom_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_small_mushroom_model.transform = MatrixScale(0.3f, 0.3f, 0.3f);

  global_mushroom_model = LoadModel("assets/mushroom.obj");
  SetMaterialTexture(&global_mushroom_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/mushroom_text.png"));
  GenTextureMipmaps(&global_mushroom_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_mushroom_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_small_flower_model = LoadModel("assets/flor.obj");
  SetMaterialTexture(&global_small_flower_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/flor_text.png"));
  GenTextureMipmaps(&global_small_flower_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_small_flower_model.transform = MatrixScale(0.1f, 0.1f, 0.1f);

  global_flower_model = LoadModel("assets/flor.obj");
  SetMaterialTexture(&global_flower_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/flor_text.png"));
  GenTextureMipmaps(&global_flower_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_flower_model.transform = MatrixScale(0.15f, 0.15f, 0.15f);

  global_green_powder_model = LoadModel("assets/pozinho.obj");
  SetMaterialTexture(&global_green_powder_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/pozinho_verde_text.png"));
  GenTextureMipmaps(&global_green_powder_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_green_powder_model.transform = MatrixScale(0.2f, 0.2f, 0.2f);

  global_red_powder_model = LoadModel("assets/pozinho.obj");
  SetMaterialTexture(&global_red_powder_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/pozinho_vermelho_text.png"));
  GenTextureMipmaps(&global_red_powder_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_red_powder_model.transform = MatrixScale(0.2f, 0.2f, 0.2f);

  global_blue_powder_model = LoadModel("assets/pozinho.obj");
  SetMaterialTexture(&global_blue_powder_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/pozinho_azul_text.png"));
  GenTextureMipmaps(&global_blue_powder_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_blue_powder_model.transform = MatrixScale(0.2f, 0.2f, 0.2f);

  global_white_powder_model = LoadModel("assets/pozinho.obj");
  //SetMaterialTexture(&global_white_powder_model.materials[0], MAP_DIFFUSE,
      //LoadTexture("assets/po_branco_text.png"));
  //GenTextureMipmaps(&global_white_powder_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_white_powder_model.transform = MatrixScale(0.2f, 0.2f, 0.2f);

  global_small_bone_model = LoadModel("assets/osso.obj");
  //SetMaterialTexture(&global_bone_model.materials[0], MAP_DIFFUSE,
      //LoadTexture("assets/bone_text.png"));
  //GenTextureMipmaps(&global_bone_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_small_bone_model.transform = MatrixScale(0.13f, 0.13f, 0.13f);

  global_bone_model = LoadModel("assets/osso.obj");
  //SetMaterialTexture(&global_bone_model.materials[0], MAP_DIFFUSE,
      //LoadTexture("assets/bone_text.png"));
  //GenTextureMipmaps(&global_bone_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_bone_model.transform = MatrixScale(0.2f, 0.2f, 0.2f);

  global_garbage_model = LoadModel("assets/stone.obj");
  SetMaterialTexture(&global_garbage_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/garbage_text.png"));
  GenTextureMipmaps(&global_garbage_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_garbage_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_crystal_model = LoadModel("assets/crystal.obj");
  SetMaterialTexture(&global_crystal_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/crystal_text.png"));
  GenTextureMipmaps(&global_crystal_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_crystal_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_red_crystal_model = LoadModel("assets/crystal.obj");
  SetMaterialTexture(&global_red_crystal_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/crystal_red_text.png"));
  GenTextureMipmaps(&global_red_crystal_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_red_crystal_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_blue_crystal_model = LoadModel("assets/crystal.obj");
  SetMaterialTexture(&global_blue_crystal_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/crystal_blue_text.png"));
  GenTextureMipmaps(&global_blue_crystal_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_blue_crystal_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_purple_stone_model = LoadModel("assets/stone.obj");
  SetMaterialTexture(&global_purple_stone_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/stone_purple_text.png"));
  GenTextureMipmaps(&global_purple_stone_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_purple_stone_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_green_stone_model = LoadModel("assets/stone.obj");
  SetMaterialTexture(&global_green_stone_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/stone_green_text.png"));
  GenTextureMipmaps(&global_green_stone_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_green_stone_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_yellow_stone_model = LoadModel("assets/stone.obj");
  SetMaterialTexture(&global_yellow_stone_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/stone_yellow_text.png"));
  GenTextureMipmaps(&global_yellow_stone_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_yellow_stone_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_small_red_stone_model = LoadModel("assets/stone.obj");
  SetMaterialTexture(&global_small_red_stone_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/stone_red_text.png"));
  GenTextureMipmaps(&global_small_red_stone_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_small_red_stone_model.transform = MatrixScale(0.3f, 0.3f, 0.3f);

  global_red_stone_model = LoadModel("assets/stone.obj");
  SetMaterialTexture(&global_red_stone_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/stone_red_text.png"));
  GenTextureMipmaps(&global_red_stone_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_red_stone_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_red_potion_model = LoadModel("assets/pocao_vermelha.obj");
  SetMaterialTexture(&global_red_potion_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/pocao_vermelha_text.png"));
  GenTextureMipmaps(&global_red_potion_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_red_potion_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_blue_potion_model = LoadModel("assets/blue_potion.obj");
  SetMaterialTexture(&global_blue_potion_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/blue_potion_text.png"));
  GenTextureMipmaps(&global_blue_potion_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_blue_potion_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_garbage_bottle_model = LoadModel("assets/yellow_potion.obj");
  SetMaterialTexture(&global_garbage_bottle_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/garbage_bottle_text.png"));
  GenTextureMipmaps(&global_garbage_bottle_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_garbage_bottle_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_empty_bottle_model = LoadModel("assets/yellow_potion.obj");
  SetMaterialTexture(&global_empty_bottle_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/empty_bottle_text.png"));
  GenTextureMipmaps(&global_empty_bottle_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_empty_bottle_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_yellow_potion_model = LoadModel("assets/yellow_potion.obj");
  SetMaterialTexture(&global_yellow_potion_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/yellow_potion_text.png"));
  GenTextureMipmaps(&global_yellow_potion_model.materials[0].maps[MAP_DIFFUSE].texture);
  global_yellow_potion_model.transform = MatrixScale(0.4f, 0.4f, 0.4f);

  global_scale_empty_model = LoadModel("assets/scale_empty.obj");
  SetMaterialTexture(&global_scale_empty_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/scale_empty_text.png"));
  GenTextureMipmaps(&global_scale_empty_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_scale_full_model = LoadModel("assets/scale_full.obj");
  SetMaterialTexture(&global_scale_full_model.materials[0], MAP_DIFFUSE,
      LoadTexture("assets/scale_full_text.png"));
  GenTextureMipmaps(&global_scale_full_model.materials[0].maps[MAP_DIFFUSE].texture);

  Mesh plane_mesh = GenMeshPlane(FLOOR_SIZE, FLOOR_SIZE, 1, 1);
  Model floor_model = LoadModelFromMesh(plane_mesh);
  floor_model.materials[0].maps[MAP_DIFFUSE].texture = LoadTexture("assets/wood.png");
  GenTextureMipmaps(&floor_model.materials[0].maps[MAP_DIFFUSE].texture);

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

  global_masher_model = LoadModel("assets/masher_empty.obj");
  SetMaterialTexture(&global_masher_model.materials[0], MAP_DIFFUSE,
    LoadTexture("assets/masher_empty_text.png"));
  GenTextureMipmaps(&global_masher_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_chest_model = LoadModel("assets/chest.obj");
  SetMaterialTexture(&global_chest_model.materials[0], MAP_DIFFUSE,
    LoadTexture("assets/chest_text.png"));
  GenTextureMipmaps(&global_chest_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_delivery_model = LoadModel("assets/delivery.obj");
  SetMaterialTexture(&global_delivery_model.materials[0], MAP_DIFFUSE,
    LoadTexture("assets/delivery_text.png"));
  GenTextureMipmaps(&global_delivery_model.materials[0].maps[MAP_DIFFUSE].texture);

  global_trashcan_model = LoadModel("assets/trashcan.obj");
  SetMaterialTexture(&global_trashcan_model.materials[0], MAP_DIFFUSE,
    LoadTexture("assets/trashcan_text.png"));
  GenTextureMipmaps(&global_trashcan_model.materials[0].maps[MAP_DIFFUSE].texture);

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
  global_scale_full_model.materials[0].shader = shader;
  global_scale_empty_model.materials[0].shader = shader;
  global_cauldron_model.materials[0].shader = shader;
  global_character_model.materials[0].shader = shader;
  global_centrifuge_open_model.materials[0].shader = shader;
  global_centrifuge_closed_model.materials[0].shader = shader;
  global_masher_model.materials[0].shader = shader;
  global_chest_model.materials[0].shader = shader;
  global_delivery_model.materials[0].shader = shader;
  global_trashcan_model.materials[0].shader = shader;
  floor_model.materials[0].shader = shader;
  global_red_potion_model.materials[0].shader = shader;
  global_blue_potion_model.materials[0].shader = shader;
  global_yellow_potion_model.materials[0].shader = shader;
  global_empty_bottle_model.materials[0].shader = shader;
  global_garbage_bottle_model.materials[0].shader = shader;
  global_small_red_stone_model.materials[0].shader = shader;
  global_red_stone_model.materials[0].shader = shader;
  global_yellow_stone_model.materials[0].shader = shader;
  global_green_stone_model.materials[0].shader = shader;
  global_purple_stone_model.materials[0].shader = shader;
  global_garbage_model.materials[0].shader = shader;
  global_crystal_model.materials[0].shader = shader;
  global_red_crystal_model.materials[0].shader = shader;
  global_blue_crystal_model.materials[0].shader = shader;
  global_bone_model.materials[0].shader = shader;
  global_small_bone_model.materials[0].shader = shader;
  global_white_powder_model.materials[0].shader = shader;
  global_red_powder_model.materials[0].shader = shader;
  global_blue_powder_model.materials[0].shader = shader;
  global_green_powder_model.materials[0].shader = shader;
  global_flower_model.materials[0].shader = shader;
  global_small_flower_model.materials[0].shader = shader;
  global_mushroom_model.materials[0].shader = shader;
  global_small_mushroom_model.materials[0].shader = shader;

  {
    global_item_models[IT_EMPTY_BOTTLE] = global_empty_bottle_model;
    global_item_models[IT_GARBAGE_BOTTLE] = global_garbage_bottle_model;

    global_item_models[IT_RED_POTION] = global_red_potion_model;
    global_item_models[IT_YELLOW_POTION] = global_yellow_potion_model;
    global_item_models[IT_BLACK_POTION] = global_crystal_model;

    global_item_models[IT_PINK_POTION] = global_crystal_model;
    global_item_models[IT_BLUE_POTION] = global_blue_potion_model;

    global_item_models[IT_FLOWER] = global_flower_model;
    global_item_models[IT_RED_ROCK] = global_red_stone_model;
    global_item_models[IT_MUSHROOM] = global_mushroom_model;
    global_item_models[IT_BLUE_CRYSTAL] = global_blue_crystal_model;
    global_item_models[IT_BONE] = global_bone_model;

    global_item_models[IT_SMALL_FLOWER] = global_small_flower_model;
    global_item_models[IT_RED_POWDER] = global_red_powder_model;
    global_item_models[IT_SMALL_MUSHROOM] = global_small_mushroom_model;
    global_item_models[IT_BLUE_POWDER] = global_blue_powder_model;
    global_item_models[IT_WHITE_POWDER] = global_white_powder_model;
    global_item_models[IT_GREEN_POWDER] = global_green_powder_model;
    global_item_models[IT_SMALL_RED_ROCK] = global_small_red_stone_model;
    global_item_models[IT_SMALL_BONE] = global_small_bone_model;

    global_item_models[IT_RED_CRYSTAL] = global_red_crystal_model;
    global_item_models[IT_YELLOW_CRYSTAL] = global_crystal_model;
    global_item_models[IT_LITTLE_MAN] = global_crystal_model;

    global_item_models[IT_GARBAGE] = global_garbage_model;
  }

  {
    global_item_icons[IT_FLOWER] = LoadTexture("assets/flor.png");
    global_item_icons_scale[IT_FLOWER] = 0.05f;
    global_item_icons[IT_BONE] = LoadTexture("assets/osso.png");
    global_item_icons_scale[IT_BONE] = 0.05f;
    global_item_icons[IT_SMALL_BONE] = LoadTexture("assets/osso.png");
    global_item_icons_scale[IT_SMALL_BONE] = 0.05f;
    global_item_icons[IT_WHITE_POWDER] = LoadTexture("assets/pozinho_branco.png");
    global_item_icons_scale[IT_WHITE_POWDER] = 0.05f;
    global_item_icons[IT_RED_ROCK] = LoadTexture("assets/red_rock.png");
    global_item_icons_scale[IT_RED_ROCK] = 0.05f;
    global_item_icons[IT_RED_POWDER] = LoadTexture("assets/pozinho_vermelho.png");
    global_item_icons_scale[IT_RED_POWDER] = 0.05f;
    global_item_icons[IT_MUSHROOM] = LoadTexture("assets/mushroom.png");
    global_item_icons_scale[IT_MUSHROOM] = 0.05f;
    global_item_icons[IT_GREEN_POWDER] = LoadTexture("assets/pozinho_verde.png");
    global_item_icons_scale[IT_GREEN_POWDER] = 0.05f;
    global_item_icons[IT_BLUE_POWDER] = LoadTexture("assets/pozinho_azul.png");
    global_item_icons_scale[IT_BLUE_POWDER] = 0.05f;
    global_item_icons[IT_SMALL_FLOWER] = LoadTexture("assets/flor.png");
    global_item_icons_scale[IT_SMALL_FLOWER] = 0.05f;
    global_item_icons[IT_BLUE_CRYSTAL] = LoadTexture("assets/crystal_azul.png");
    global_item_icons_scale[IT_BLUE_CRYSTAL] = 0.05f;
    global_item_icons[IT_RED_CRYSTAL] = LoadTexture("assets/crystal_vermelho.png");
    global_item_icons_scale[IT_RED_CRYSTAL] = 0.05f;
    global_item_icons[IT_BLUE_POTION] = LoadTexture("assets/blue_potion.png");
    global_item_icons_scale[IT_BLUE_POTION] = 0.05f;
  }


  CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 3.0f, 20.0f, 7 }, (Vector3){ 0.0f, 0.0f, 0.0f }, (Color){ 255, 255, 255, 255 }, shader);

  Map map = { 0 };

  init_data(&map, &gui);
  map.players[0].pos.x = -1;
  map.players[0].model = global_character_model;
  map.players[1].pos.x = 1;
  map.players[1].model = global_character_model;
  map.player_count = 2;

  global_potion_process_list_len = sizeof(global_potion_process_list)/sizeof(PotionProcess);

  global_available_recipes_count = 0;

  add_available_recipes(IT_WHITE_POWDER);
  add_available_recipes(IT_RED_POWDER);
  add_available_recipes(IT_GREEN_POWDER);
  add_available_recipes(IT_BLUE_POWDER);
  //add_available_recipes(IT_SMALL_FLOWER);
  add_available_recipes(IT_BLUE_POTION);

  Camera3D camera = { 0 };
  camera.position = (Vector3){ 0.0f, 13.0f, 13.0f };  // Camera position
  camera.target = (Vector3){ 0.0f, 0.0f, 2.0f };      // Camera looking at point
  camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;                                // Camera field-of-view Y
  camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
  while (!WindowShouldClose()) {
    if (recipe_generation_timer < 0) {
      recipe_generation_timer = RECIPE_GENERATION_TIMER;
      generate_recipe(&gui);
    } else {
      recipe_generation_timer -= GetFrameTime();
    }

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
            ItemType item_input[] = { map.scale_list[p->current_action_id].item };
            map.scale_list[p->current_action_id].item = get_recipe_result(item_input, 1, DT_SCALE);
            p->current_action = DT_NONE;
          }
          break;
        case DT_MASHER:
          map.masher_list[p->current_action_id].progress -= GetFrameTime();
          if (map.masher_list[p->current_action_id].progress <= 0) {
            ItemType item_input[] = { map.masher_list[p->current_action_id].item };
            map.masher_list[p->current_action_id].item = get_recipe_result(item_input, 1, DT_MASHER);
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
        DrawModel(global_item_models[c.item], item_pos, 1.0f, WHITE);
      }
    }

    // draw scales
    for (int i = 0; i < map.scale_list_size; i++) {
      Scale s = map.scale_list[i];

      DrawModel(s.item ? s.model_full : s.model_empty, Vector3Zero(), 1.0f, WHITE);
      if (s.item) {
        Vector3 item_pos = {s.pos.x, s.pos.y+1.3f, s.pos.z};
        DrawModel(global_item_models[s.item], item_pos, 1.0f, WHITE);
      }
    }

    // draw mashers
    for (int i = 0; i < map.masher_list_size; i++) {
      Masher s = map.masher_list[i];

      DrawModel(s.model, Vector3Zero(), 1.0f, WHITE);
      if (s.item) {
        Vector3 item_pos = {s.pos.x, s.pos.y+1.3f, s.pos.z};
        DrawModel(global_item_models[s.item], item_pos, 1.0f, WHITE);
      }
    }

    // draw centrifuges
    for (int i = 0; i < map.centrifuge_list_size; i++) {
      Centrifuge s = map.centrifuge_list[i];

      DrawModel(s.item ? s.model_closed : s.model_open, Vector3Zero(), 1.0f, WHITE);
      if (s.item) {
        Vector3 item_pos = {s.pos.x, s.pos.y+1.3f, s.pos.z};
        DrawModel(global_item_models[s.item], item_pos, 1.0f, WHITE);
      }
    }

    // draw chests
    for (int i = 0; i < map.chest_list_size; i++) {
      Chest s = map.chest_list[i];

      DrawModel(s.model, Vector3Zero(), 1.0f, WHITE);
      Vector3 item_pos = {s.pos.x, s.pos.y+1.3f, s.pos.z};
      DrawModel(global_item_models[s.item], item_pos, 1.0f, WHITE);
    }

    // draw deliveries
    for (int i = 0; i < map.delivery_list_size; i++) {
      Delivery s = map.delivery_list[i];

      DrawModel(s.model, Vector3Zero(), 1.0f, WHITE);
    }

    // draw trashcans
    for (int i = 0; i < map.trashcan_list_size; i++) {
      Trashcan s = map.trashcan_list[i];

      DrawModel(s.model, Vector3Zero(), 1.0f, WHITE);
    }

    // draw cauldrons
    for (int i = 0; i < map.cauldron_list_size; i++) {
      Cauldron c = map.cauldron_list[i];

      DrawModel(c.model, Vector3Zero(), 1.0f, WHITE);

      for (int j = 0; j < c.items_size; j++) {
        Vector3 item_pos = {c.pos.x, c.pos.y+1.3f+(j*0.3), c.pos.z};
        DrawModel(global_item_models[c.items[j]], item_pos, 1.0f, WHITE);
      }
    }

    // draw dropped items
    for (int i = 0; i < map.dropped_item_list_size; i++) {
      DroppedItem item = map.dropped_item_list[i];
      DrawModel(global_item_models[item.item], item.pos, 1.0f, WHITE);
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
        DrawModel(global_item_models[p.item], item_pos, 1.0f, WHITE);
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
      float testf = (float) ((float)get_card_width(gui.cards[i]) - (global_item_icons[gui.cards[i].product].width * global_item_icons_scale[gui.cards[i].product])) / 2.0f;
      Vector2 item_sloc = {gui.cards[i].pos.x + testf, gui.cards[i].pos.y};
      DrawTextureEx(global_item_icons[gui.cards[i].product], item_sloc, 0.0f, global_item_icons_scale[gui.cards[i].product], WHITE);
      DrawRectangleRoundedLines(card, 0.2f, 0, 1, BLACK);

      for (int j = 0; j < gui.cards[i].ingredient_list_len; j++) {
        Rectangle ingredient = {
          gui.cards[i].pos.x + (j * INGREDIENT_CARD_SIZE),
          gui.cards[i].pos.y + (3 * CARD_SIZE / 4),
          INGREDIENT_CARD_SIZE, INGREDIENT_CARD_SIZE
        };
        DrawRectangleRec(ingredient, WHITE);
        ItemType ingredient_type = gui.cards[i].ingredient_list[j];
        //printf("ingredient_Type %d\n", ingredient_type);
        float testf = (float) ((float)INGREDIENT_CARD_SIZE - (global_item_icons[ingredient_type].width * global_item_icons_scale[ingredient_type])) / 2.0f;
        Vector2 item_sloc = {ingredient.x + testf, ingredient.y};
        DrawTextureEx(global_item_icons[ingredient_type], item_sloc, 0.0f, global_item_icons_scale[ingredient_type], WHITE);
        //DrawRectangleLines(ingredient.x, ingredient.y, ingredient.width, ingredient.height, BLACK);
      }
    }

    DrawFPS(GetScreenWidth()-80, 10);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
