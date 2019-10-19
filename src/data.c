void add_cube(Map *map, float x, float z) {
  map->counter_list[map->counter_list_size].item.type = IT_UNINITIALIZED;
  map->counter_list[map->counter_list_size].pos = (Vector3){ x, 0.0f, z };
  map->counter_list_size++;
}

void add_item_to_counter(Counter *c, ItemType it, Color color) {
  c->item.type = it;
  c->item.color = color;
}

void init_data(Map *map, GUI *gui) {
  add_cube(map, -5.0, -3.0);
  add_cube(map, -5.0, -2.0);
  add_cube(map, -5.0, -1.0);
  add_cube(map, -5.0, 0.0);
  add_cube(map, -5.0, 1.0);
  add_cube(map, -5.0, 2.0);
  add_cube(map, -5.0, 3.0);

  add_cube(map, -4.0, -3.0);
  add_cube(map, -3.0, -3.0);
  add_cube(map, -2.0, -3.0);
  add_cube(map, -1.0, -3.0);
  add_cube(map, 0.0, -3.0);
  add_cube(map, 1.0, -3.0);
  add_cube(map, 2.0, -3.0);
  add_cube(map, 3.0, -3.0);
  add_cube(map, 4.0, -3.0);

  add_cube(map, 5.0, -3.0);
  add_cube(map, 5.0, -2.0);
  add_cube(map, 5.0, -1.0);
  add_cube(map, 5.0, 0.0);
  add_cube(map, 5.0, 1.0);
  add_cube(map, 5.0, 2.0);
  add_cube(map, 5.0, 3.0);

  add_cube(map, -4.0, 3.0);
  add_cube(map, -3.0, 3.0);
  add_cube(map, -2.0, 3.0);
  add_cube(map, -1.0, 3.0);
  add_cube(map, 0.0, 3.0);
  add_cube(map, 1.0, 3.0);
  add_cube(map, 2.0, 3.0);
  add_cube(map, 3.0, 3.0);
  add_cube(map, 4.0, 3.0);

  add_item_to_counter(&map->counter_list[4], IT_FLASK, LIME);
  add_item_to_counter(&map->counter_list[7], IT_FLASK, PINK);
  add_item_to_counter(&map->counter_list[8], IT_FLASK, RED);

  ItemType list[5] = {1, 1, 1, 1, 1};
  add_card(gui, list, 2);
  add_card(gui, list, 3);
  add_card(gui, list, 5);
}
