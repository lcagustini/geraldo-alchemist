#define MAX_COUNTERS 50
#define MAX_ITEMS 50

typedef enum {
  IT_UNINITIALIZED,
  IT_FLASK,

  IT_MAX
} ItemType;

typedef struct {
  ItemType type;
  Color color;
} Item;

typedef struct {
  Item item;
  Vector3 pos;
} DroppedItem;

typedef struct {
  Vector3 pos;
  Item item;
} Counter;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  Item item;
} Player;

typedef struct {
  Counter counter_list[MAX_COUNTERS];
  int counter_list_size;

  DroppedItem dropped_item_list[MAX_ITEMS];
  int dropped_item_list_size;
} Map;

typedef enum {
  I_BLUE,
  I_RED
} Ingredient;

typedef struct {
  Vector2 pos;

  Ingredient ingredient_list[5];
  int ingredient_list_len;
} Card;

typedef struct {
  Card cards[20];
  int cards_len;

  int cards_width;
} GUI;
