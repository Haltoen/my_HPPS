#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <omp.h>
#include "util.h"

struct vec3 add_vec3(struct vec3 pi, struct vec3 pj) {
  double ix = pi.x;
  double iy = pi.y;
  double iz = pi.z;
  double jx = pj.x;
  double jy = pj.y;
  double jz = pj.z;
  struct vec3 newVec;
  newVec.x = ix + jx; 
  newVec.y = iy + jy; 
  newVec.z = iz + jz; 
  return newVec;
}


struct vec3 scalar_mul_vec3(double c, struct vec3 p) {
  double x = p.x;
  double y = p.y;
  double z = p.z;
  struct vec3 newVec;
  newVec.x = c * x; 
  newVec.y = c * y; 
  newVec.z = c * z; 
  return newVec;
}

// Naive solution O(n^2)
void nbody(int n, struct particle *ps, int steps) {
  for(int s = 0; s < steps; s++) {

    #pragma omp parallel for 
    for (int i = 0; i < n; i++){
      struct vec3 pos_i = ps[i].pos;
      //struct vec3 acc = (struct vec3){0.0, 0.0, 0.0}; // accumelate total force on point i
      double acc_x = 0;
      double acc_y = 0;
      double acc_z = 0;
      #pragma omp parallel for reduction(+:acc_x, acc_y, acc_z)
      for (int j = 0; j < n; j++){
        if (i != j) {
          struct vec3 vec_j = ps[j].pos;
          double mass_j = ps[j].mass; 
          struct vec3 f = force(pos_i, vec_j, mass_j);
          acc_x += f.x;
          acc_y += f.y;
          acc_z += f.z;
          
          //acc = add_vec3(acc, f);
        }
      }
      //struct vec3 new_vel = add_vec3(ps[i].vel, acc); // update velocity with acceleration
      ps[i].vel.x += acc_x;
      ps[i].vel.y += acc_y;
      ps[i].vel.z += acc_z;
      //struct vec3 new_pos = add_vec3(pos_i, new_vel); // update position with velocity
      ps[i].pos.x += ps[i].vel.x;
      ps[i].pos.y += ps[i].vel.y;
      ps[i].pos.z += ps[i].vel.z;
    }
  }
}

int main(int argc, char** argv) {
  int steps = 1;
  if (argc < 3) {
    printf("Usage: \n");
    printf("%s <input> <output> [steps]\n", argv[0]);
    return 1;
  } else if (argc > 3) {
    steps = atoi(argv[3]);
  }

  int32_t n;
  struct particle *ps = read_particles(argv[1], &n);

  double bef = seconds();
  nbody(n, ps, steps);
  double aft = seconds();
  printf("%f\n", aft-bef);
  write_particles(argv[2], n, ps);
  free(ps);
}
