void up_button(Map map, Player *player, Vector3 *new_dir) {
  new_dir->z -= 1.0f;
  player->pos.z -= 0.1f;
  if (collides_with_counters(*player, map)) player->pos.z += 0.1f;
}

void down_button(Map map, Player *player, Vector3 *new_dir) {
  new_dir->z += 1.0f;
  player->pos.z += 0.1f;
  if (collides_with_counters(*player, map)) player->pos.z -= 0.1f;
}

void left_button(Map map, Player *player, Vector3 *new_dir) {
  new_dir->x -= 1.0f;
  player->pos.x -= 0.1f;
  if (collides_with_counters(*player, map)) player->pos.x += 0.1f;
}

void right_button(Map map, Player *player, Vector3 *new_dir) {
  new_dir->x += 1.0f;
  player->pos.x += 0.1f;
  if (collides_with_counters(*player, map)) player->pos.x -= 0.1f;
}

void action_button(Map *map, Player *player) {
  if (player->item_pickup_cooldown >= 0) return;

  float nearest_dist;
  int nearest = get_aimed_counter(player, map, &nearest_dist);
  if (!map->counter_list[nearest].item.type && nearest_dist < 0.6f && player->item.type) { // try to put item on the counter
    map->counter_list[nearest].item = player->item;
    player->item.type = IT_UNINITIALIZED;
  }
  else if (!get_item(player, map) && player->item.type) { // drop item if holding any
    map->dropped_item_list[map->dropped_item_list_size].item = player->item;
    map->dropped_item_list[map->dropped_item_list_size].pos = player->pos;
    map->dropped_item_list_size++;
    player->item.type = IT_UNINITIALIZED;
  }
  player->item_pickup_cooldown = PLAYER_ITEM_PICKUP_COOLDOWN;
}

void keyboard_input(Map *map, Player *player, Vector3 *new_dir) {
  if (IsKeyDown(KEY_UP)) {
    up_button(*map, player, new_dir);
  }
  if (IsKeyDown(KEY_DOWN)) {
    down_button(*map, player, new_dir);
  }
  if (IsKeyDown(KEY_LEFT)) {
    left_button(*map, player, new_dir);
  }
  if (IsKeyDown(KEY_RIGHT)) {
    right_button(*map, player, new_dir);
  }
  if (IsKeyDown(KEY_Z)) {
    action_button(map, player);
  }
}
