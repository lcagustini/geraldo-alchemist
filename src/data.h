#define MAX_COUNTERS 50
#define MAX_DEVICES 50
#define MAX_ITEMS 50

#define MAX_INGREDIENTS 5

typedef enum {
  IT_UNINITIALIZED,

  IT_EMPTY_BOTTLE,
  IT_GARBAGE_BOTTLE,

  IT_RED_POTION,
  IT_YELLOW_POTION,
  IT_BLACK_POTION,

  IT_PINK_POTION,
  IT_BLUE_POTION,

  IT_BOTTLED_ITEMS,

  IT_FLOWER,
  IT_RED_ROCK,
  IT_MUSHROOM,
  IT_BLUE_CRYSTAL,
  IT_BONE,

  IT_SMALL_FLOWER,
  IT_RED_POWDER,
  IT_SMALL_MUSHROOM,
  IT_BLUE_POWDER,
  IT_WHITE_POWDER,
  IT_GREEN_POWDER,
  IT_SMALL_RED_ROCK,
  IT_SMALL_BONE,

  IT_RED_CRYSTAL,
  IT_YELLOW_CRYSTAL,
  IT_LITTLE_MAN,

  IT_GARBAGE,

  IT_MAX
} ItemType;

typedef enum {
  DT_COUNTER,

  DT_SCALE,
  DT_MASHER,

  DT_CAULDRON,

  DT_CENTRIFUGE,

  DT_CHEST,
  DT_DELIVERY,
  DT_TRASHCAN,

  DT_BOTTLE,

  DT_NONE
} DeviceType;

typedef struct {
  ItemType before[MAX_INGREDIENTS];
  int before_len;

  DeviceType process;

  ItemType after;
} PotionProcess;

typedef struct {
  ItemType item;
  Vector3 pos;
} DroppedItem;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  ItemType item;
  Model model;
} Counter;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  ItemType item;
  float progress;

  Model model_open;
  Model model_closed;
} Centrifuge;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  ItemType item;
  float progress;

  Model model_full;
  Model model_empty;
} Scale;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  ItemType item;
  float progress;

  Model model;
} Masher;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  ItemType items[MAX_INGREDIENTS];
  int items_size;
  float progress;
  Model model;
} Cauldron;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  ItemType item;
  Model model;
} Chest;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  Model model;
} Delivery;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  Model model;
} Trashcan;

typedef struct {
  Vector3 pos;
  Vector3 dir;
  ItemType item;
  float item_pickup_cooldown; // less than 0 = good to go

  DeviceType current_action;
  int current_action_id;

  Model model;
} Player;

typedef struct {
  Trashcan trashcan_list[MAX_DEVICES];
  int trashcan_list_size;

  Delivery delivery_list[MAX_DEVICES];
  int delivery_list_size;

  Chest chest_list[MAX_DEVICES];
  int chest_list_size;

  Masher masher_list[MAX_DEVICES];
  int masher_list_size;

  Centrifuge centrifuge_list[MAX_DEVICES];
  int centrifuge_list_size;

  Cauldron cauldron_list[MAX_DEVICES];
  int cauldron_list_size;

  Scale scale_list[MAX_DEVICES];
  int scale_list_size;

  Counter counter_list[MAX_COUNTERS];
  int counter_list_size;

  DroppedItem dropped_item_list[MAX_ITEMS];
  int dropped_item_list_size;

  Player players[4];
  int player_count;
} Map;

typedef struct {
  Vector2 pos;

  ItemType product;
  ItemType ingredient_list[MAX_INGREDIENTS];
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
