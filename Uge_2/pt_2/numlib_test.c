#include "numlib.h"
#include <stdio.h>

int main(void) {
    printf("Enter the first number: ");
    int number1 = get_number();
    printf("Enter the second number: ");
    int number2 = get_number();

    printf("The first number entered is: %d\n", number1);
    printf("The second number entered is: %d\n", number2);
}