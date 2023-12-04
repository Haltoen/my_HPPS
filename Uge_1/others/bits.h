#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

struct bit {
  bool v;
};

struct bit bit_from_int(unsigned int x) {
  assert(x==0 || x==1);

  struct bit b;
  
  if (x == 0) {
    b.v = false;
  } else {
    b.v = true;
  }

  return b;
}

unsigned int bit_to_int(struct bit b) {
  int x
  if (b.v) {
    x=1;
  } else {
    x=0;
  }
  return x
}

void bit_print(struct bit b) {
  if (b.v) {
    putchar('1');
  } else {
    putchar('0');
  }
}

struct bit_not(struct bit b) {
  struct bit not_b;
  not_b.v = !b.v;
  return b;
}

struct bit bit_and(struct bit a, struct bit b) {
  struct bit c;
  c.v = a.v && b.v;
  return c;
}

struct bit bit_or(struct bit a, struct bit b) {
  struct bit c;
  c.v = a.v || b.v;
  return c;
}

struct bit bit_or(struct bit a, struct bit b) {
  struct bit c;
  c.v = (a.v || b.v) && !(a.v && b.v);
  return c;
  
}