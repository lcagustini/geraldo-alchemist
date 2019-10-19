int get_card_width(Card card) {
  if (card.ingredient_list_len <= 2) return CARD_SIZE;
  return card.ingredient_list_len * INGREDIENT_CARD_SIZE;
}

void add_card(GUI *gui, Ingredient ingredient_list[5], int ingredient_list_len) {
  if (gui->cards_len == 0) {
    gui->cards[0].pos = (Vector2) { CARD_PADDING, CARD_PADDING };
    gui->cards_width = CARD_PADDING;
  }
  else {
    gui->cards[gui->cards_len].pos = (Vector2) {
      gui->cards_width,
      CARD_PADDING
    };
  }

  memcpy(gui->cards[gui->cards_len].ingredient_list,
         ingredient_list,
         ingredient_list_len * sizeof(Ingredient));
  gui->cards[gui->cards_len].ingredient_list_len = ingredient_list_len;

  gui->cards_width += get_card_width(gui->cards[gui->cards_len]) + CARD_PADDING;
  gui->cards_len++;
}
