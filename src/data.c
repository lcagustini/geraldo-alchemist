void add_counter(Map *map, float x, float z, Vector3 dir) {
  map->counter_list[map->counter_list_size].item = IT_UNINITIALIZED;
  map->counter_list[map->counter_list_size].pos = (Vector3){ x, 0.0f, z };
  map->counter_list[map->counter_list_size].dir = dir;
  map->counter_list[map->counter_list_size].model = global_counter_model;
  float angle = atan2f(-dir.z, dir.x);
  map->counter_list[map->counter_list_size].model.transform = MatrixRotate((Vector3){0,1,0}, angle);
  map->counter_list_size++;
}

void add_scale(Map *map, float x, float z, Vector3 dir) {
  map->scale_list[map->scale_list_size].item = IT_UNINITIALIZED;
  map->scale_list[map->scale_list_size].progress = 5;
  map->scale_list[map->scale_list_size].pos = (Vector3){ x, 0.0f, z };
  map->scale_list[map->scale_list_size].dir = dir;
  map->scale_list[map->scale_list_size].model = global_scale_model;
  float angle = atan2f(-dir.z, dir.x);
  map->scale_list[map->scale_list_size].model.transform = MatrixRotate((Vector3){0,1,0}, angle);
  map->scale_list_size++;
}

void add_item_to_counter(Counter *c, ItemType it) {
  c->item = it;
}

void init_data(Map *map, GUI *gui) {
  Vector3 dir;

  dir = (Vector3){ 1.0f, 0.0f, 0.0f };
  add_counter(map, -5.0, -3.0, dir);
  add_counter(map, -5.0, -2.0, dir);
  add_counter(map, -5.0, -1.0, dir);
  add_counter(map, -5.0, 0.0, dir);
  add_counter(map, -5.0, 1.0, dir);
  add_counter(map, -5.0, 2.0, dir);
  add_counter(map, -5.0, 3.0, dir);

  dir = (Vector3){ 0.0f, 0.0f, 1.0f };
  add_counter(map, -4.0, -3.0, dir);
  add_counter(map, -3.0, -3.0, dir);
  add_counter(map, -2.0, -3.0, dir);
  add_counter(map, -1.0, -3.0, dir);
  add_counter(map, 0.0, -3.0, dir);
  add_counter(map, 1.0, -3.0, dir);
  add_counter(map, 2.0, -3.0, dir);
  add_counter(map, 3.0, -3.0, dir);
  add_counter(map, 4.0, -3.0, dir);

  dir = (Vector3){ -1.0f, 0.0f, 0.0f };
  add_counter(map, 5.0, -3.0, dir);
  add_counter(map, 5.0, -2.0, dir);
  add_scale(map, 5.0, -1.0, dir);
  add_counter(map, 5.0, 0.0, dir);
  add_counter(map, 5.0, 1.0, dir);
  add_counter(map, 5.0, 2.0, dir);
  add_counter(map, 5.0, 3.0, dir);

  dir = (Vector3){ 0.0f, 0.0f, -1.0f };
  add_counter(map, -4.0, 3.0, dir);
  add_counter(map, -3.0, 3.0, dir);
  add_counter(map, -2.0, 3.0, dir);
  add_counter(map, -1.0, 3.0, dir);
  add_counter(map, 0.0, 3.0, dir);
  add_counter(map, 1.0, 3.0, dir);
  add_counter(map, 2.0, 3.0, dir);
  add_counter(map, 3.0, 3.0, dir);
  add_counter(map, 4.0, 3.0, dir);

  add_item_to_counter(&map->counter_list[4], IT_INGREDIENT1);
  add_item_to_counter(&map->counter_list[7], IT_INGREDIENT2);
  add_item_to_counter(&map->counter_list[8], IT_INGREDIENT3);

  ItemType list[5] = {1, 1, 1, 1, 1};
  add_card(gui, list, 2);
  add_card(gui, list, 3);
  add_card(gui, list, 5);
}
