#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "util.h"
#include <string.h>



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




// Figure out which child octant a particle belongs to. Returns a
// number from 0 to 7, inclusive.
//
// 'l' is the edge length of space.
int octant(struct vec3 corner, double l, const struct particle* p) {
  if (p->pos.x >= corner.x+l/2) {
    if (p->pos.y >= corner.y+l/2) {
      if (p->pos.z >= corner.z+l/2) {
        return 0;
      } else {
        return 1;
      }
    } else {
      if (p->pos.z >= corner.z+l/2) {
        return 2;
      } else {
        return 3;
      }
    }
  } else {
    if (p->pos.y >= corner.y+l/2) {
      if (p->pos.z >= corner.z+l/2) {
        return 4;
      } else {
        return 5;
      }
    } else {
      if (p->pos.z >= corner.z+l/2) {
        return 6;
      } else {
        return 7;
      }
    }
  }
}

// Given the index of a child octant (0-7), place in *ox/*oy/*oz the
// normalized corner coordinate.
void octant_offset(int j, double* ox, double* oy, double* oz) {
  switch (j) {
  case 0:
    *ox = 0.5;
    *oy = 0.5;
    *oz = 0.5;
    break;
  case 1:
    *ox = 0.5;
    *oy = 0.5;
    *oz = 0.0;
    break;
  case 2:
    *ox = 0.5;
    *oy = 0.0;
    *oz = 0.5;
    break;
  case 3:
    *ox = 0.5;
    *oy = 0.0;
    *oz = 0.0;
    break;
  case 4:
    *ox = 0.0;
    *oy = 0.5;
    *oz = 0.5;
    break;
  case 5:
    *ox = 0.0;
    *oy = 0.5;
    *oz = 0.0;
    break;
  case 6:
    *ox = 0.0;
    *oy = 0.0;
    *oz = 0.5;
    break;
  case 7:
    *ox = 0.0;
    *oy = 0.0;
    *oz = 0.0;
    break;
  }
}

// You do not need to modify this definition.
struct bh_node {
  bool internal; // False when external.

  struct vec3 corner;
  double l; // Edge length of space.

  // Fields for external nodes.
  int particle; // Index of particle in particle array; -1 if none.

  // Fields for internal nodes. Only have sensible values when
  // 'internal' is true.
  struct vec3 com; // Center of mass.
  double mass; // Total mass.
  struct bh_node* children[8];
};

// Turn an external node into an internal node containing no
// particles, and with 8 external node children.
// Turn an external node into an internal node containing no
// particles, and with 8 external node children.
void bh_mk_internal(struct bh_node* bh) {
  // Must not already be internal.
  assert(!bh->internal);

  struct bh_node** children = malloc(8 * sizeof(struct bh_node*)); // Allocate an array of pointers
  

  // Set corners and initialize other fields for each child
  for (int i = 0; i < 8; i++) {
    double x, y, z;
    octant_offset(i, &x, &y, &z);
    children[i] = malloc(sizeof(struct bh_node)); // Allocate memory for each child
    children[i]->corner.x = bh->corner.x + bh->l * x;
    children[i]->corner.y = bh->corner.y + bh->l * y;
    children[i]->corner.z = bh->corner.z + bh->l * z;
    children[i]->l = 0.5 * bh->l; // Set the edge length
    children[i]->mass = 0;
    children[i]->internal = false;
    children[i]->particle = -1;
    bh->children[i] = children[i];
  }

  // Additional initialization for internal node
  bh->internal = true;
  bh->particle = -1;
  bh->com.x = 0.0;
  bh->com.y = 0.0;
  bh->com.z = 0.0;
  bh->mass = 0;
}
// handles mass / center of mass updates for adding one particle
void add_mass(struct vec3* center, double* mass, struct particle p) {
  double total_mass = *mass + p.mass; // Total mass
  // averaged center of mass vectors 
  struct vec3 m1 = scalar_mul_vec3(*mass/total_mass, *center);
  struct vec3 m2 = scalar_mul_vec3(p.mass/total_mass, p.pos);
  *center = add_vec3(m1, m2);
  *mass = total_mass;
}




// Insert particle 'p' (which must be a valid index in 'ps') into our octree.
void bh_insert(struct bh_node* bh, struct particle* ps, int p) {

  // Internal case: keep traversing down the trree
  if (bh->internal) {
    // This is an internal node. Recursively insert the particle in
    // the appropriate child (computed with octant()), then update the
    // centre of mass.
    
    int child_ind = octant(bh->corner, bh->l, &ps[p]);
    printf("Internal q_ind %d \n", child_ind);
    struct bh_node* child = bh->children[child_ind];
    add_mass(&bh->com, &bh->mass, ps[p]);
    bh_insert(child, ps, p);
  
  // External case: place directly or make new split and place 
  } else {
    // This is an external node.
    if (bh->particle == -1) {
      // This is an external node currently with no particle, so we
      // can just insert the new particle.
      bh->particle = p;
      printf("placed %d \n", p);
    } else {
      // This is an external node that already has a particle. We must
      // convert it into an internal node with initially zero mass,
      // and then insert both the new particle *and* the one it
      // previously contained, using recursive calls to bh_insert.
      
      int bh_p = bh->particle; // copy bh particle index
      bh_mk_internal(bh); // make bh internal (resets com & mass)
      printf("Split %d, %d \n", p, bh_p);
      bh_insert(bh, ps, p);
      bh_insert(bh, ps, bh_p);
      /*
    

      int p_ind = octant(bh->corner, bh->l, &ps[bh_p]);
      struct bh_node* child = bh->children[p_ind];
      // Update the center of mass and mass of the internal node
      add_mass(&bh->com, &bh->mass, ps[bh_p]);
      bh_insert(bh, ps, p);
      bh_insert(child, ps, bh_p);
      */
    }
  }
}

void free_children(struct bh_node* bh) {
  if (bh->internal) {
    for (int i = 0; i < 8; i++) {
      free_children(bh->children[i]);
    }
    free(*bh->children);
  }
}
// Free all memory used for the tree.
void bh_free(struct bh_node* bh) {
  if (bh->internal) {
    for (int i = 0; i < 8; i++) {
      bh_free(bh->children[i]);
    }
  }
  free(bh);
}


// Compute the accel acting on particle 'p'.  Increments *ax, *ay, and *az.
void bh_accel(double theta, struct bh_node* bh,
              struct particle* ps, int p,
              struct vec3 *a) {
  struct vec3 pos = ps[p].pos;              
  if (bh->internal) {
    double d = dist(bh->com, pos);
    // Use com if far awa
    if (bh->l / d < theta) {
      *a = add_vec3(*a, force(pos, bh->com, bh->mass));
    }
    else {
      for (int i = 0; i < 8; i++) {
        bh_accel(theta, bh->children[i], ps, p, a);
      }
    }
  } else if (bh->particle != -1 && bh->particle != p) {
    *a = add_vec3(*a, force(ps[p].pos, ps[bh->particle].pos, ps[bh->particle].mass));
  }
}

// Create a new octree that spans a space with the provided minimum
// and maximum coordinates.
struct bh_node* bh_new(double min_coord, double max_coord) {
  struct bh_node* bh = malloc(sizeof(struct bh_node));
  bh->corner.x = min_coord;
  bh->corner.y = min_coord;
  bh->corner.z = min_coord;
  bh->l = max_coord-min_coord;
  bh->internal = false;
  bh->particle = -1;
  return bh;
}

// Helper function to calculate the minimum and maximum coordinates
// among all particles.
void calculate_min_max(struct particle* ps, int n, double* min, double* max) {
  *min = INFINITY;
  *max = -INFINITY;
  for (int i = 0; i < n; i++) {
    if (ps[i].pos.x < *min) *min = ps[i].pos.x;
    if (ps[i].pos.y < *min) *min = ps[i].pos.y;
    if (ps[i].pos.z < *min) *min = ps[i].pos.z;

    if (ps[i].pos.x > *max) *max = ps[i].pos.x;
    if (ps[i].pos.y > *max) *max = ps[i].pos.y;
    if (ps[i].pos.z > *max) *max = ps[i].pos.z;
  }
}

void nbody(int n, struct particle *ps, int steps, double theta) {
    // Determine the minimum and maximum coordinates among all particles
    double min_coord, max_coord;
    

    struct bh_node* octree = NULL;  // Initialize octree pointer outside the loop

    for (int s = 0; s < steps; s++) {
        // Create a new octree
        calculate_min_max(ps, n, &min_coord, &max_coord);
        octree = bh_new(min_coord, max_coord);

        // Recursively insert particles into the octree
        for (int i = 0; i < n; i++) {
            bh_insert(octree, ps, i);
        }

        // Compute accelerations and update velocities
        for (int i = 0; i < n; i++) {
            struct vec3 acceleration = {0.0, 0.0, 0.0};
            bh_accel(theta, octree, ps, i, &acceleration);
            ps[i].vel = add_vec3(ps[i].vel, acceleration);
        }

        // Update positions based on velocities
        for (int i = 0; i < n; i++) {
            ps[i].pos = add_vec3(ps[i].pos, ps[i].vel);
        }
        // Print an update (if desired)
        printf("Step %d\n", s);
        bh_free(octree);
    }
}

int main(int argc, char** argv) {
  int steps = 1;
  double theta = 0.5;
  if (argc < 3) {
    printf("Usage: \n");
    printf("%s <input> <output> [steps]\n", argv[0]);
    return 1;
  }
  if (argc > 3) {
    steps = atoi(argv[3]);
  }
  if (argc > 4) {
    theta = atof(argv[4]);
  }

  int32_t n;
  struct particle *ps = read_particles(argv[1], &n);

  double bef = seconds();
  nbody(n, ps, steps, theta);
  double aft = seconds();
  printf("%f\n", aft-bef);
  write_particles(argv[2], n, ps);
  free(ps);
}
