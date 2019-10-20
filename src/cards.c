int get_card_width(Card card) {
  if (card.ingredient_list_len <= 2) return CARD_SIZE;
  return card.ingredient_list_len * INGREDIENT_CARD_SIZE;
}

void add_card(GUI *gui, ItemType product, ItemType ingredient_list[5], int ingredient_list_len) {
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
         ingredient_list_len * sizeof(ItemType));
  gui->cards[gui->cards_len].ingredient_list_len = ingredient_list_len;
  gui->cards[gui->cards_len].product = product;

  gui->cards_width += get_card_width(gui->cards[gui->cards_len]) + CARD_PADDING;
  gui->cards_len++;
}

void add_available_recipes(ItemType a) {
  global_available_recipes[global_available_recipes_count] = a;
  global_available_recipes_count++;
}

void remove_available_recipes(ItemType a) {
}

// Chooses a random recipe based on difficulty and adds to the list of wanted products
// cannot be used to generate more than MAX_WANTED_ITEMS, it will just fail silently (intended behavior)
void generate_recipe(GUI *gui) {
  if (((global_wanted_items_end+1)%MAX_WANTED_ITEMS) == global_wanted_items_start) {
    printf("Full of recipes, cannot generate more\n");
    return;
  }

  ItemType recipe = global_available_recipes[rand() % global_available_recipes_count];
  global_wanted_items[global_wanted_items_end++] = recipe;
  if (global_wanted_items_end == MAX_WANTED_ITEMS) global_wanted_items_end = 0;

  ItemType ingredient_list[5];
  int ingredient_list_len = -1;
  for (int i = 0; global_potion_process_list_len; i++) {
    if (global_potion_process_list[i].after == recipe) {
      memcpy(ingredient_list, global_potion_process_list[i].before, global_potion_process_list[i].before_len);
      ingredient_list_len = global_potion_process_list[i].before_len;
      break;
    }
  }
  assert(ingredient_list_len != -1);

  add_card(gui, recipe, ingredient_list, ingredient_list_len);
}

// Receives an item and checks if it is expected. If so, remove it from the list of wanted recipes.
void consume_recipe() {

}
