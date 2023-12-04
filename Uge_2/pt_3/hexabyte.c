#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv) {
  if (argc == 2) {
    char* filename = argv[1];
    FILE *f = fopen(filename, "r");
    assert(f != NULL);

    char c;

    while (fread(&c, sizeof(c), 1, f) == 1) {
        printf("%.2x\n", (int)c);   
    }
    
    return 0;

  }
}