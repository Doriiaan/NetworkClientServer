#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


int main(int argc, char const *argv[]) {
  Queue *q = createQueue();
  

  char elem1[50] = "Hello à tous, ça va ??\n";
  int elem2 = 56;

  q = pushQueue(q, elem1);
  q = pushQueue(q, &elem2);

  printf("%s", (char*)top(q));
  q = popQueue(q);
  printf("%d\n", *(int *)top(q)); 
  q = popQueue(q);

  free(q);

  return 0;
}
