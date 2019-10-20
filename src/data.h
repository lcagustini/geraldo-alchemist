#define MAX_COUNTERS 50
#define MAX_DEVICES 50
#define MAX_ITEMS 50

#define MAX_INGREDIENTS 5

typedef enum {
  IT_UNINITIALIZED,

  IT_INGREDIENT1,
  IT_INGREDIENT2,
  IT_INGREDIENT3,
  IT_INGREDIENT4,

  IT_INGREDIENT1_POWDER,
  IT_INGREDIENT2_POWDER,
  IT_INGREDIENT3_SMALL,
  IT_INGREDIENT4_SMALL,

  IT_INGREDIENT_COOKED_3_4,
  IT_INGREDIENT_SOLID_3,

  IT_GARBAGE,

  IT_MAX
} ItemType;

typedef enum {
  DT_COUNTER,

  DT_SCALE,
  DT_CAULDRON,
  DT_CENTRIFUGE,
  DT_MASHER,
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
  ItemType items[MAX_INGREDIENTS];
  int items_size;
  float progress;
  Model model;
} Cauldron;

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
