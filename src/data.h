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
  Vector3 pos;
  Item item;
} Counter;

typedef struct {
  Counter counter_list[MAX_COUNTERS];
  int counter_list_size;

  Item item_list[MAX_ITEMS];
  int item_list_size;
} Map;

typedef struct {
  Vector2 pos;
} Card;
