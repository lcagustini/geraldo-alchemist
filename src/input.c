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
  map->players[player_id].pos.z -= PLAYER_SPEED * GetFrameTime();
  if (collides_with_map(*map, player_id) ||
      collides_with_player(*map, player_id)) map->players[player_id].pos.z += PLAYER_SPEED * GetFrameTime();

  if (map->players[player_id].item_pickup_cooldown >= 0) return;
  map->players[player_id].current_action = DT_NONE;
}

void down_button(Map *map, int player_id, Vector3 *new_dir) {
  new_dir->z += 1.0f;
  map->players[player_id].pos.z += PLAYER_SPEED * GetFrameTime();
  if (collides_with_map(*map, player_id) ||
      collides_with_player(*map, player_id)) map->players[player_id].pos.z -= PLAYER_SPEED * GetFrameTime();

  if (map->players[player_id].item_pickup_cooldown >= 0) return;
  map->players[player_id].current_action = DT_NONE;
}

void left_button(Map *map, int player_id, Vector3 *new_dir) {
  new_dir->x -= 1.0f;
  map->players[player_id].pos.x -= PLAYER_SPEED * GetFrameTime();
  if (collides_with_map(*map, player_id) ||
      collides_with_player(*map, player_id)) map->players[player_id].pos.x += PLAYER_SPEED * GetFrameTime();

  if (map->players[player_id].item_pickup_cooldown >= 0) return;
  map->players[player_id].current_action = DT_NONE;
}

void right_button(Map *map, int player_id, Vector3 *new_dir) {
  new_dir->x += 1.0f;
  map->players[player_id].pos.x += PLAYER_SPEED * GetFrameTime();
  if (collides_with_map(*map, player_id) ||
      collides_with_player(*map, player_id)) map->players[player_id].pos.x -= PLAYER_SPEED * GetFrameTime();

  if (map->players[player_id].item_pickup_cooldown >= 0) return;
  map->players[player_id].current_action = DT_NONE;
}

void action_button(Map *map, Player *player) {
  if (player->item_pickup_cooldown >= 0) return;
  player->item_pickup_cooldown = PLAYER_ITEM_PICKUP_COOLDOWN;

  float nearest_dist[DT_NONE];
  int nearest[DT_NONE];

  nearest[DT_COUNTER] = get_aimed_counter(player, map, &nearest_dist[DT_COUNTER]);
  nearest[DT_SCALE] = get_aimed_scale(player, map, &nearest_dist[DT_SCALE]);
  nearest[DT_CAULDRON] = get_aimed_cauldron(player, map, &nearest_dist[DT_CAULDRON]);

  //printf("nearest_counter %d, dist %f\n", nearest[DT_COUNTER], nearest_dist[DT_COUNTER]);
  int nearest_id = minf(nearest_dist, 3);

  if (nearest_dist[nearest_id] > MIN_RANGE_TO_USE_DEVICE) {
    if (player->item) {
      map->dropped_item_list[map->dropped_item_list_size].item = player->item;
      map->dropped_item_list[map->dropped_item_list_size].pos = player->pos;
      map->dropped_item_list_size++;
      player->item = IT_UNINITIALIZED;
    }
    else if (!player->item) {
      int nearest = -1;
      float nearest_dist = 999999.0f;
      for (int i = 0; i < map->dropped_item_list_size; i++) {
        float distance = Vector3Distance(player->pos, map->dropped_item_list[i].pos);
        if (distance < nearest_dist) {
          nearest = i;
          nearest_dist = distance;
        }
      }

      // TODO: tune this tolerance better?
      if (nearest_dist < MIN_RANGE_TO_PICKUP_ITEM) {
        // drop item if holding any
        if (player->item) {
          map->dropped_item_list[map->dropped_item_list_size].item = player->item;
          map->dropped_item_list[map->dropped_item_list_size].pos = player->pos;
          map->dropped_item_list_size++;
        }

        // get new item
        player->item = map->dropped_item_list[nearest].item;
        map->dropped_item_list_size--;
        for (int i = nearest; i < map->dropped_item_list_size; i++) {
          map->dropped_item_list[i] = map->dropped_item_list[i+1];
        }
      }
    }

    return;
  }

  switch (nearest_id) {
    case DT_COUNTER:
      puts("counter");
      if (!map->counter_list[nearest[DT_COUNTER]].item && player->item) { // try to put item on the counter
        map->counter_list[nearest[DT_COUNTER]].item = player->item;
        player->item = IT_UNINITIALIZED;
      }
      else if (map->counter_list[nearest[DT_COUNTER]].item && player->item) { // drop item if holding any
          map->dropped_item_list[map->dropped_item_list_size].item = player->item;
          map->dropped_item_list[map->dropped_item_list_size].pos = player->pos;
          map->dropped_item_list_size++;
          player->item = map->counter_list[nearest[DT_COUNTER]].item;
          map->counter_list[nearest[DT_COUNTER]].item = IT_UNINITIALIZED;
      }
      else if (map->counter_list[nearest[DT_COUNTER]].item) {
        player->item = map->counter_list[nearest[DT_COUNTER]].item;
        map->counter_list[nearest[DT_COUNTER]].item = IT_UNINITIALIZED;
      }
      break;
    case DT_SCALE:
      puts("scale");
      if (!map->scale_list[nearest[DT_SCALE]].item && player->item) {
        map->scale_list[nearest[DT_SCALE]].item = player->item;
        map->scale_list[nearest[DT_SCALE]].progress = 5.0f;

        player->item = IT_UNINITIALIZED;

        player->current_action = DT_SCALE;
        player->current_action_id = nearest[DT_SCALE];
      }
      else if (map->scale_list[nearest[DT_SCALE]].item && !player->item) {
        if (map->scale_list[player->current_action_id].progress > 0) {
          player->current_action = DT_SCALE;
          player->current_action_id = nearest[DT_SCALE];
        }
        else {
          player->item = map->scale_list[player->current_action_id].item;
          map->scale_list[player->current_action_id].item = IT_UNINITIALIZED;

          player->current_action = DT_NONE;
        }
      }
      break;
    case DT_CAULDRON:
      puts("cauldron");
      if (!(map->cauldron_list[nearest[DT_CAULDRON]].items_size < MAX_INGREDIENTS) &&
          player->item) {
        map->cauldron_list[nearest[DT_CAULDRON]].items[map->cauldron_list[nearest[DT_CAULDRON]].items_size++] = player->item;
        map->cauldron_list[nearest[DT_CAULDRON]].progress = 5.0f;

        player->item = IT_UNINITIALIZED;

        player->current_action = DT_CAULDRON;
        player->current_action_id = nearest[DT_CAULDRON];
      }
      else if (map->cauldron_list[nearest[DT_CAULDRON]].items_size && !player->item) {
        if (map->cauldron_list[player->current_action_id].progress > 0) {
          player->current_action = DT_CAULDRON;
          player->current_action_id = nearest[DT_CAULDRON];
        }
        else {
          player->item = map->cauldron_list[player->current_action_id].items[0];
          map->cauldron_list[player->current_action_id].items_size = 0;

          player->current_action = DT_NONE;
        }
      }
      break;
    default:
      printf("%d\n", nearest_id);
      assert(false);
  }
}

void keyboard_input(Map *map, int player_id) {
  Vector3 new_dir = { 0.0f, 0.0f, 0.0f };

  Vector3 old_position = map->players[player_id].pos;
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
  Vector3 pos_offset = Vector3Subtract(map->players[player_id].pos, old_position);
  if (Vector3Length(pos_offset) > PLAYER_SPEED * GetFrameTime()) {
    map->players[player_id].pos = Vector3Add(old_position, Vector3Scale(Vector3Normalize(pos_offset), PLAYER_SPEED * GetFrameTime()));
  }


  if (IsKeyDown(get_key_for_player(player_id, B_ACTION))) {
    action_button(map, &map->players[player_id]);
  }

  if (Vector3Length(new_dir)) {
    map->players[player_id].dir = Vector3Normalize(new_dir);
    float angle = atan2f(-new_dir.z, new_dir.x);
    map->players[player_id].model.transform = MatrixRotate((Vector3){0,1,0}, angle);
  }
}
