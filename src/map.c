bool collides_with_map(Map map, int player_id) {
  for (int i = 0; i < map.counter_list_size; i++) {
    BoundingBox bbox = MeshBoundingBox(map.counter_list[i].model.meshes[0]);

    bbox.min = Vector3Add(bbox.min, map.counter_list[i].pos);
    bbox.max = Vector3Add(bbox.max, map.counter_list[i].pos);

    //DrawBoundingBox(bbox, BLUE);
    if (CheckCollisionBoxes(GET_COUNTER_BBOX(map.players[player_id]), bbox)) {
      return true;
    }
  }

  for (int i = 0; i < map.scale_list_size; i++) {
    BoundingBox bbox = MeshBoundingBox(map.scale_list[i].model.meshes[0]);

    bbox.min = Vector3Add(bbox.min, map.scale_list[i].pos);
    bbox.max = Vector3Add(bbox.max, map.scale_list[i].pos);

    //DrawBoundingBox(bbox, BLUE);
    if (CheckCollisionBoxes(GET_COUNTER_BBOX(map.players[player_id]), bbox)) {
      return true;
    }
  }

  for (int i = 0; i < map.cauldron_list_size; i++) {
    BoundingBox bbox = MeshBoundingBox(map.cauldron_list[i].model.meshes[0]);

    bbox.min = Vector3Add(bbox.min, map.cauldron_list[i].pos);
    bbox.max = Vector3Add(bbox.max, map.cauldron_list[i].pos);

    //DrawBoundingBox(bbox, BLUE);
    if (CheckCollisionBoxes(GET_COUNTER_BBOX(map.players[player_id]), bbox)) {
      return true;
    }
  }

  for (int i = 0; i < map.centrifuge_list_size; i++) {
    BoundingBox bbox = MeshBoundingBox(map.centrifuge_list[i].model_closed.meshes[0]);

    bbox.min = Vector3Add(bbox.min, map.centrifuge_list[i].pos);
    bbox.max = Vector3Add(bbox.max, map.centrifuge_list[i].pos);

    //DrawBoundingBox(bbox, BLUE);
    if (CheckCollisionBoxes(GET_COUNTER_BBOX(map.players[player_id]), bbox)) {
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

int get_aimed_cauldron(Player *p, Map *map, float *nearest_dist) {
  assert(nearest_dist);

  Vector3 pos = { p->pos.x, p->pos.y + 0.5f, p->pos.z };
  Ray ray = { pos, p->dir };
  int nearest = -1;
  *nearest_dist = 999999.0f;
  for (int i = 0; i < map->cauldron_list_size; i++) {
    RayHitInfo hit = GetCollisionRayModel(ray, map->cauldron_list[i].model);
    if (hit.hit) {
      if (hit.distance < *nearest_dist) {
        nearest = i;
        *nearest_dist = hit.distance;
      }
    }
  }

  return nearest;
}

int get_aimed_centrifuge(Player *p, Map *map, float *nearest_dist) {
  assert(nearest_dist);

  Vector3 pos = { p->pos.x, p->pos.y + 0.5f, p->pos.z };
  Ray ray = { pos, p->dir };
  int nearest = -1;
  *nearest_dist = 999999.0f;
  for (int i = 0; i < map->centrifuge_list_size; i++) {
    RayHitInfo hit = GetCollisionRayModel(ray, map->centrifuge_list[i].model_closed);
    if (hit.hit) {
      if (hit.distance < *nearest_dist) {
        nearest = i;
        *nearest_dist = hit.distance;
      }
    }
  }

  return nearest;
}

int get_aimed_scale(Player *p, Map *map, float *nearest_dist) {
  assert(nearest_dist);

  Ray ray = { p->pos, p->dir };
  int nearest = -1;
  *nearest_dist = 999999.0f;
  for (int i = 0; i < map->scale_list_size; i++) {
    RayHitInfo hit = GetCollisionRayModel(ray, map->scale_list[i].model);
    if (hit.hit) {
      if (hit.distance < *nearest_dist) {
        nearest = i;
        *nearest_dist = hit.distance;
      }
    }
  }

  return nearest;
}

int get_aimed_counter(Player *p, Map *map, float *nearest_dist) {
  assert(nearest_dist);

  Ray ray = { p->pos, p->dir };
  int nearest = -1;
  *nearest_dist = 999999.0f;
  for (int i = 0; i < map->counter_list_size; i++) {
    RayHitInfo hit = GetCollisionRayModel(ray, map->counter_list[i].model);
    if (hit.hit) {
      if (hit.distance < *nearest_dist) {
        nearest = i;
        *nearest_dist = hit.distance;
      }
    }
  }

  return nearest;
}
