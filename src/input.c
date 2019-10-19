KeyboardKey get_key_for_player(int player_id, Button button) {
  switch (player_id) {
    case 0:
      {
        switch (button) {
          case B_UP:
            return KEY_W;
          case B_RIGHT:
            return KEY_D;
          case B_LEFT:
            return KEY_A;
          case B_DOWN:
            return KEY_S;
          case B_ACTION:
            return KEY_E;
        }
      }
      break;
    case 1:
      {
        switch (button) {
          case B_UP:
            return KEY_UP;
          case B_RIGHT:
            return KEY_RIGHT;
          case B_LEFT:
            return KEY_LEFT;
          case B_DOWN:
            return KEY_DOWN;
          case B_ACTION:
            return KEY_RIGHT_SHIFT;
        }
      }
      break;
    case 2:
      {
        // TODO: player 3?
      }
      break;
    case 3:
      {
        // TODO: player 4?
      }
      break;
  }
  return KEY_KP_0;
}

void up_button(Map *map, int player_id, Vector3 *new_dir) {
  new_dir->z -= 1.0f;
  map->players[player_id].pos.z -= 0.1f;
  if (collides_with_counters(*map, player_id) ||
      collides_with_player(*map, player_id)) map->players[player_id].pos.z += 0.1f;
}

void down_button(Map *map, int player_id, Vector3 *new_dir) {
  new_dir->z += 1.0f;
  map->players[player_id].pos.z += 0.1f;
  if (collides_with_counters(*map, player_id) ||
      collides_with_player(*map, player_id)) map->players[player_id].pos.z -= 0.1f;
}

void left_button(Map *map, int player_id, Vector3 *new_dir) {
  new_dir->x -= 1.0f;
  map->players[player_id].pos.x -= 0.1f;
  if (collides_with_counters(*map, player_id) ||
      collides_with_player(*map, player_id)) map->players[player_id].pos.x += 0.1f;
}

void right_button(Map *map, int player_id, Vector3 *new_dir) {
  new_dir->x += 1.0f;
  map->players[player_id].pos.x += 0.1f;
  if (collides_with_counters(*map, player_id) ||
      collides_with_player(*map, player_id)) map->players[player_id].pos.x -= 0.1f;
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

void keyboard_input(Map *map, int player_id) {
  Vector3 new_dir = { 0.0f, 0.0f, 0.0f };

  if (IsKeyDown(get_key_for_player(player_id, B_UP))) {
    up_button(map, player_id, &new_dir);
  }
  if (IsKeyDown(get_key_for_player(player_id, B_DOWN))) {
    down_button(map, player_id, &new_dir);
  }
  if (IsKeyDown(get_key_for_player(player_id, B_LEFT))) {
    left_button(map, player_id, &new_dir);
  }
  if (IsKeyDown(get_key_for_player(player_id, B_RIGHT))) {
    right_button(map, player_id, &new_dir);
  }
  if (IsKeyDown(get_key_for_player(player_id, B_ACTION))) {
    action_button(map, &map->players[player_id]);
  }

  if (Vector3Length(new_dir)) {
    map->players[player_id].dir = Vector3Normalize(new_dir);
  }
}
