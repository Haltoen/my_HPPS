#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    exit(1);
  }

  const char* fname = argv[1];

  int32_t n;
  struct particle* ps = read_particles(fname, &n);

  for (int i = 0; i < n; i++) {
    printf("%f %f %f %f %f %f %f\n",
           ps[i].mass,
           ps[i].pos.x, ps[i].pos.y, ps[i].pos.z,
           ps[i].vel.x, ps[i].vel.y, ps[i].vel.z);
  }
}
