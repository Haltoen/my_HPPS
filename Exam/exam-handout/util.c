#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <sys/time.h>
#include "util.h"




double seconds(void) {
  struct timeval tv;
  gettimeofday(&tv, NULL); // The NULL is for timezone information.
  return tv.tv_sec + tv.tv_usec/1000000.0;
}

double dist(struct vec3 a, struct vec3 b) {
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  double dz = a.z - b.z;

  return sqrt(dx*dx + dy*dy + dz*dz);
}

struct vec3 force(struct vec3 pi, struct vec3 pj, double mj) {
  static const double epsilon = 1;

  double d = dist(pi, pj);
  double d_sqr = d * d;
  double inv_d = 1.0 / sqrt(d_sqr + epsilon*epsilon);
  double inv_d3 = inv_d * inv_d * inv_d;

  struct vec3 f;
  f.x = (pj.x - pi.x) * mj * inv_d3;
  f.y = (pj.y - pi.y) * mj * inv_d3;
  f.z = (pj.z - pi.z) * mj * inv_d3;
  return f;
}

struct particle* read_particles(const char *fname, int32_t *n) {
  FILE *f = fopen(fname, "rb");
  assert(f != NULL);
  assert(fread(n, sizeof(*n), 1, f) == 1); // Read n  
  struct particle* particles = malloc(*n * sizeof(struct particle));
  struct particle p;   
  for (int i = 0; i < *n; i++) {
    assert(fread(&p, sizeof(struct particle), 1, f ) == 1);
    particles[i] = p;
  }
  fclose(f);
  return particles;
}

void write_particles(const char *fname, int32_t n, struct particle *ps) {
  FILE *f = fopen(fname, "wb");
  assert(f != NULL);
  assert(fwrite(&n, sizeof(n), 1, f) == 1);
  for (int i = 0; i < n; i++) {
    assert(fwrite(&ps[i], sizeof(struct particle), 1, f) == 1);
  }
  fclose(f);
  //free(ps); 
}


