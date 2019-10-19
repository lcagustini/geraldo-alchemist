void add_cube(Map *map, float x, float z) {
  map->counter_list[map->counter_list_size].pos = (Vector3){ x, 0.0f, z };
  map->counter_list_size++;
}

void init_data(&map) {
  add_cube(&map, -5.0, -3.0);
  add_cube(&map, -5.0, -2.0);
  add_cube(&map, -5.0, -1.0);
  add_cube(&map, -5.0, 0.0);
  add_cube(&map, -5.0, 1.0);
  add_cube(&map, -5.0, 2.0);
  add_cube(&map, -5.0, 3.0);

  add_cube(&map, -4.0, -3.0);
  add_cube(&map, -3.0, -3.0);
  add_cube(&map, -2.0, -3.0);
  add_cube(&map, -1.0, -3.0);
  add_cube(&map, 0.0, -3.0);
  add_cube(&map, 1.0, -3.0);
  add_cube(&map, 2.0, -3.0);
  add_cube(&map, 3.0, -3.0);
  add_cube(&map, 4.0, -3.0);

  add_cube(&map, 5.0, -3.0);
  add_cube(&map, 5.0, -2.0);
  add_cube(&map, 5.0, -1.0);
  add_cube(&map, 5.0, 0.0);
  add_cube(&map, 5.0, 1.0);
  add_cube(&map, 5.0, 2.0);
  add_cube(&map, 5.0, 3.0);

  add_cube(&map, -4.0, 3.0);
  add_cube(&map, -3.0, 3.0);
  add_cube(&map, -2.0, 3.0);
  add_cube(&map, -1.0, 3.0);
  add_cube(&map, 0.0, 3.0);
  add_cube(&map, 1.0, 3.0);
  add_cube(&map, 2.0, 3.0);
  add_cube(&map, 3.0, 3.0);
  add_cube(&map, 4.0, 3.0);
}
