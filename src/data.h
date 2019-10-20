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
  Model model;
} Counter;

typedef struct {
  Vector3 pos;
  Vector3 dir; // must be sync'd with model.transform
  Item item;
  float item_pickup_cooldown; // less than 0 = good to go
  Model model;
} Player;

typedef struct {
  Counter counter_list[MAX_COUNTERS];
  int counter_list_size;

  DroppedItem dropped_item_list[MAX_ITEMS];
  int dropped_item_list_size;

  Player players[4];
  int player_count;
} Map;

typedef struct {
  Vector2 pos;

  ItemType ingredient_list[5];
  int ingredient_list_len;
} Card;

typedef struct {
  Card cards[20];
  int cards_len;

  int cards_width;
} GUI;

typedef enum {
  B_UP,
  B_RIGHT,
  B_LEFT,
  B_DOWN,
  B_ACTION
} Button;
