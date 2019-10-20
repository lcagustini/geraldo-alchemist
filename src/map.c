bool collides_with_counters(Map map, int player_id) {
  for (int i = 0; i < map.counter_list_size; i++) {
    if (CheckCollisionBoxes(GET_COUNTER_BBOX(map.players[player_id]),
                            GET_COUNTER_BBOX(map.counter_list[i]))) {
      return true;
    }
  }
  return false;
}

bool collides_with_player(Map map, int player_id) {
  for (int j = 0; j < map.player_count; j++) {
    if (player_id == j) continue;

    if (CheckCollisionBoxes(GET_COUNTER_BBOX(map.players[player_id]),
                            GET_COUNTER_BBOX(map.players[j]))) {
      return true;
    }
  }
  return false;
}

int get_aimed_scale(Player *p, Map *map, float *nearest_dist) {
  assert(nearest_dist);

  Ray ray = { p->pos, p->dir };
  Vector3 collision_point = { 0 };
  int nearest = -1;
  *nearest_dist = 999999.0f;
  for (int i = 0; i < map->scale_list_size; i++) {
    if (CheckCollisionRaySphereEx(ray, map->scale_list[i].pos, 1.0f, &collision_point)) { // migue pq n tem isso pra BBOX
      float distance = Vector3Distance(collision_point, p->pos);
      if (distance < *nearest_dist) {
        nearest = i;
        *nearest_dist = distance;
      }
    }
  }
  //printf("nearest_dist %f\n", *nearest_dist);

  return nearest;
}

int get_aimed_counter(Player *p, Map *map, float *nearest_dist) {
  assert(nearest_dist);

  Ray ray = { p->pos, p->dir };
  Vector3 collision_point = { 0 };
  int nearest = -1;
  *nearest_dist = 999999.0f;
  for (int i = 0; i < map->counter_list_size; i++) {
    if (CheckCollisionRaySphereEx(ray, map->counter_list[i].pos, 1.0f, &collision_point)) { // migue pq n tem isso pra BBOX
      float distance = Vector3Distance(collision_point, p->pos);
      if (distance < *nearest_dist) {
        nearest = i;
        *nearest_dist = distance;
      }
    }
  }
  //printf("nearest_dist %f\n", *nearest_dist);

  return nearest;
}
