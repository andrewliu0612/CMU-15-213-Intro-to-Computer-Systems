#include <stdio.h>



int subOK(int x, int y) {
  /* Compare the signs of x, y and z 
   * If +-- or -++ then overflows
   */
  int z = x - y;
  int mask = 1 << 31;
  return !(((x & mask) ^ (y & mask)) & ((x & mask) ^ (z & mask)));
}

int main() {
    return 0;
}