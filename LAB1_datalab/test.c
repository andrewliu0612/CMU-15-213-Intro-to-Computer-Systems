#include <stdio.h>



int bitParity(int x) {
  x = x ^ (x << 16);
  x = x ^ (x << 8);
  x = x ^ (x << 4);
  x = x ^ (x << 2);
  x = x ^ (x << 1);
  return (x >> 31) & 1;
}
int main() {
  int y = bitParity(0x80000001);
  printf("%d\n", y);
}