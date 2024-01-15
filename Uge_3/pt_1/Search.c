#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int main (int argc, char* argv[]) {
    assert(argc == 2);
    const char *fname = argv[1];
    FILE *f = fopen(fname, "r");
    assert(f != NULL);
    char c;
    bool a_found = 0;
    
    while (fread(&c, sizeof(c), 1, f ) == 1)
    {
        if (c == 'A') {
            a_found = 1;
            break;
        }
    }

    if (a_found) {
        printf("A found");
    }
    else {
        printf("No A found");
    }
    return 0;
}
    