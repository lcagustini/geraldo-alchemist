int minf(float arr[], int len) {
  float min = arr[0];
  int min_i = 0;

  for (int i = 1; i < len; i++) {
    if(arr[i] < min) {
      min = arr[i];
      min_i = i;
    }
  }

  return min_i;
}

void swap(int *xp, int *yp) {
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
}

void sort(int arr[], int n) {
  int i, j;
  for (i = 0; i < n-1; i++) {
    for (j = 0; j < n-i-1; j++) {
      if (arr[j] > arr[j+1]) swap(&arr[j], &arr[j+1]);
    }
  }
}
