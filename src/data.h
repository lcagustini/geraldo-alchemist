typedef struct {
  Vector3 pos;
} Counter;

typedef struct {
  Counter counter_list[MAX_COUNTERS];
  int counter_list_size;
} Map;

typedef struct {
  Vector2 pos;
} Card;
