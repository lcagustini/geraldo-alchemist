void add_counter(Map *map, float x, float z, Vector3 dir) {
  map->counter_list[map->counter_list_size].item = IT_UNINITIALIZED;
  map->counter_list[map->counter_list_size].pos = (Vector3){ x, 0.0f, z };
  map->counter_list[map->counter_list_size].model = global_counter_model;
  float angle = atan2f(-dir.z, dir.x);
  Matrix rot = MatrixRotate((Vector3){0,1,0}, angle);
  Vector3 pos = map->counter_list[map->counter_list_size].pos;
  Matrix tran = MatrixTranslate(pos.x, pos.y, pos.z);
  Matrix scale = MatrixScale(1.1, 1.0, 1.1);
  map->counter_list[map->counter_list_size].model.transform = MatrixMultiply(scale, MatrixMultiply(rot, tran));
  map->counter_list_size++;
}

void add_scale(Map *map, float x, float z, Vector3 dir) {
  map->scale_list[map->scale_list_size].item = IT_UNINITIALIZED;
  map->scale_list[map->scale_list_size].progress = 5;
  map->scale_list[map->scale_list_size].pos = (Vector3){ x, 0.0f, z };
  map->scale_list[map->scale_list_size].dir = dir;
  map->scale_list[map->scale_list_size].model = global_scale_model;
  float angle = atan2f(-dir.z, dir.x);
  Matrix rot = MatrixRotate((Vector3){0,1,0}, angle);
  Vector3 pos = map->scale_list[map->scale_list_size].pos;
  Matrix tran = MatrixTranslate(pos.x, pos.y, pos.z);
  map->scale_list[map->scale_list_size].model.transform = MatrixMultiply(rot, tran);
  map->scale_list_size++;
}

void add_cauldron(Map *map, float x, float z, Vector3 dir) {
  memset(map->cauldron_list[map->cauldron_list_size].items, 0, map->cauldron_list[map->cauldron_list_size].items_size);
  map->cauldron_list[map->cauldron_list_size].progress = 5;
  map->cauldron_list[map->cauldron_list_size].pos = (Vector3){ x, 0.0f, z };
  map->cauldron_list[map->cauldron_list_size].dir = dir;
  map->cauldron_list[map->cauldron_list_size].model = global_cauldron_model;
  float angle = atan2f(-dir.z, dir.x);
  Matrix rot = MatrixRotate((Vector3){0,1,0}, angle);
  Vector3 pos = map->cauldron_list[map->cauldron_list_size].pos;
  Matrix tran = MatrixTranslate(pos.x, pos.y, pos.z);
  Matrix scale = MatrixScale(1.2, 1.2, 1.2);
  map->cauldron_list[map->cauldron_list_size].model.transform = MatrixMultiply(scale, MatrixMultiply(rot, tran));
  map->cauldron_list_size++;
}

void add_item_to_counter(Counter *c, ItemType it) {
  c->item = it;
}

void init_data(Map *map, GUI *gui) {
  Vector3 dir;

  dir = (Vector3){ 1.0f, 0.0f, 0.0f };
  add_counter(map, -5.028, -2.0, dir);
  add_counter(map, -5.028, -1.0, dir);
  add_counter(map, -5.028, 0.0, dir);
  add_counter(map, -5.028, 1.0, dir);
  add_counter(map, -5.028, 2.0, dir);

  dir = (Vector3){ 0.0f, 0.0f, 1.0f };
  add_counter(map, -5.0, -3.0, dir);
  add_counter(map, -4.0, -3.0, dir);
  add_counter(map, -3.0, -3.0, dir);
  add_counter(map, -2.0, -3.0, dir);
  add_counter(map, -1.0, -3.0, dir);
  add_counter(map, 0.0, -3.0, dir);
  add_counter(map, 1.0, -3.0, dir);
  add_counter(map, 2.0, -3.0, dir);
  add_counter(map, 3.0, -3.0, dir);
  add_counter(map, 4.0, -3.0, dir);
  add_counter(map, 5.0, -3.0, dir);

  dir = (Vector3){ -1.0f, 0.0f, 0.0f };
  add_counter(map, 5.028, -2.0, dir);
  add_counter(map, 5.028, -1.0, dir);
  add_counter(map, 5.028, 0.0, dir);
  add_counter(map, 5.028, 1.0, dir);
  add_counter(map, 5.028, 2.0, dir);

  dir = (Vector3){ 0.0f, 0.0f, -1.0f };
  add_counter(map, -5.0, 3.0, dir);
  add_counter(map, -4.0, 3.0, dir);
  add_counter(map, -3.0, 3.0, dir);
  add_counter(map, -2.0, 3.0, dir);
  add_counter(map, -1.0, 3.0, dir);
  add_scale(map, 0.0, 3.0, dir);
  add_cauldron(map, 1.0, 3.0, dir);
  add_cauldron(map, 2.0, 3.0, dir);
  add_cauldron(map, 3.0, 3.0, dir);
  add_counter(map, 4.0, 3.0, dir);
  add_counter(map, 5.0, 3.0, dir);

  add_item_to_counter(&map->counter_list[4], IT_INGREDIENT1);
  add_item_to_counter(&map->counter_list[7], IT_INGREDIENT4);
  add_item_to_counter(&map->counter_list[8], IT_INGREDIENT3);

  ItemType list[5] = {1, 1, 1, 1, 1};
  add_card(gui, list, 2);
  add_card(gui, list, 3);
  add_card(gui, list, 5);
}
