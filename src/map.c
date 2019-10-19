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
  assert(nearest != -1);

  return nearest;
}

bool get_item(Player *p, Map *map) {
  // try to get it from the counter
  {
    float nearest_dist;
    int nearest = get_aimed_counter(p, map, &nearest_dist);

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
      return true;
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
      return true;
    }
  }
  return false;
}
