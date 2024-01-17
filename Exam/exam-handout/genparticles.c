#include <stdio.h>
#include <stdlib.h>
#include "util.h"

double random_d() {
  return ((double)rand()) / RAND_MAX;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s N FILE\n", argv[0]);
    exit(1);
  }

  int n = atoi(argv[1]);
  const char* fname = argv[2];

  struct particle* ps = malloc(n * sizeof(struct particle));

  printf("Generating %d particles...\n", n);
  for (int i = 0; i < n; i++) {
    // Initial points in cube around (0,0,0).
    ps[i].pos.x = random_d() * 2 - 1;
    ps[i].pos.y = random_d() * 2 - 1;
    ps[i].pos.z = random_d() * 2 - 1;
    // Low masses so things don't interact too quickly.
    ps[i].mass = random_d() / 100000;
    ps[i].vel.x = 0;
    ps[i].vel.y = 0;
    ps[i].vel.z = 0;
  }

  printf("Writing particles to %s...\n", fname);
  write_particles(fname, n, ps);
}
