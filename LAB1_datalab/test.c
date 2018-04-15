#include <stdio.h>

int float_f2i(unsigned uf) {
  int sign = uf & 0x80000000;
  int exp = (uf >> 23) & 255;
  // int frac = uf << 8 | 0x80000000;
  int frac = (uf & 0x7fffff) | 0x800000;
  int result;
  if(exp < 127) return 0;
  else if(exp >= 158) return 0x80000000;
  else result = frac >> (158 - 8 - exp);
  if(sign) result = ~result + 1;
  return result;
}

int main() {
  printf("%d\n", float_f2i(1065353216));
}