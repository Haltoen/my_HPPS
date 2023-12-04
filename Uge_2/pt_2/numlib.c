#include <stdio.h>
#include <ctype.h>
#include "numlib.h"



int get_digit(void) {
  int c = getchar();
  if (isdigit(c)) {
    return c - 48;
  } else {
    return -1;
  }
}

int get_number(void) {
  int number = 0;
  int digit = get_digit();

  while (digit != -1) {
    number = number*10 + digit;
    digit = get_digit();
  }
  return number;
}