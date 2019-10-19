void add_cube(Map *map, float x, float z) {
  map->counter_list[map->counter_list_size].pos = (Vector3){ x, 0.0f, z };
  map->counter_list_size++;
}

void add_card(Card cards[], int *len) {
  if (*len == 0) {
    cards[0].pos = (Vector2) { CARD_PADDING, CARD_PADDING };
  }
  else {
    Card last_card = cards[*len - 1];
    cards[*len].pos = (Vector2) {
      last_card.pos.x + CARD_SIZE + CARD_PADDING,
      last_card.pos.y
    };
  }
  (*len)++;
}

void init_data(Map *map, Card cards[], int *len) {
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

  add_card(cards, len);
  add_card(cards, len);
  add_card(cards, len);
}
