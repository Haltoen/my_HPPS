#pragma once

#include <stdio.h>

// Return the number of seconds since some unspecified time. Timing
// can be done by calling this function multiple times and subtracting
// the return values.
double seconds(void);

// A vector in three-dimensional space. We use this for representing
// not just points, but also accelerations, velocities, etc.
struct vec3 {
  double x, y, z;
};

struct particle {
  struct vec3 pos;    // Positions.
  struct vec3 vel;    // Velocity.
  double mass; // Mass.
};
// Compute sum of two vectors
// Compute Euclidean distance between two points in three-dimensional
// space.
double dist(struct vec3 a, struct vec3 b);

// Compute gravitational force of particle at position 'pj' with mass
// 'mj' on a particle at position 'pi'.
struct vec3 force(struct vec3 pi, struct vec3 pj, double mj);

// Read a particle file with name 'fname'. Returns an array of
// particles, and stores the number of particles in *n.
//
// This function does not report errors, but it *must* possible detect
// errors via assert().
struct particle* read_particles(const char *fname, int32_t *n);

// Write an array 'ps' of particles with the given size 'n' to a
// particle file with the name 'fname'.
//
// This function does not report errors, but it *must* possible detect
// errors via assert().
void write_particles(const char *fname, int32_t n, struct particle *ps);


//struct vec3 add_vec3(struct vec3 pi, struct vec3 pj);

//struct vec3 scalar_mul_vec3(double c, struct vec3 p);
