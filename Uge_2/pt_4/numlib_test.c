#include "numlib.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h> 


int main() {
    FILE *file = fopen("file.txt", "r"); // Replace "testfile.txt" with your file name
    assert(file != NULL && "Error opening file");

    uint32_t value;
    int count = 0;

    while (!feof(file)) {
        skipspaces(file); // Skip spaces before reading
        int result = read_uint_ascii(file, &value);
        if (result != 0) {
            printf("Read value: %u\n", value);
            count++;
        }
    }

    printf("Total values read: %d\n", count);

    fclose(file);
    return 0;
}