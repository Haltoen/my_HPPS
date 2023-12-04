#include "numlib.h"
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


void skipspaces(FILE *f) 
{ 
  while (1) {
    int c = fgetc(f);
    if (!isspace(c)) {
      if (c != EOF) {
        ungetc(c, f);
      }
      return;
    }
  }
}

int read_uint_ascii(FILE *f, uint32_t *out) {
  uint32_t  number = 0;
  int read = 0;

  while (1)   {
    int c = fgetc(f);
    if (isdigit(c)) {
      number = number * 10 + (c - 48);
    }
    else {
      if (c != EOF) {
      ungetc(c, f);
      }
      break;
    }  
  }
  *out = (uint32_t)number;
  return number;
}
