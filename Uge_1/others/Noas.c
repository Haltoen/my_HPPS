#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc == 2) {
        char *word = argv[1];
        char first_letter = word[0];
        if (first_letter == 'A') {
            printf("No beginning A's allowed");
        }
        else {
            printf("%s\n", word);
        }
    }
    else {
        printf("wrong number of arguments");
    }
    return 0;
}
