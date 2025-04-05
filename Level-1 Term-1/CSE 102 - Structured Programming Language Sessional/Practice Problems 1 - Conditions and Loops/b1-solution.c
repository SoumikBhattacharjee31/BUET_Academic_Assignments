#include <stdio.h>

int main()
{
    /*
     * A code to swap the last two digits of a number.
     */

    int numberA, lastDigitOfA, secondLastDigitOfA, modifiedNumber;

    // Take input of n.
    printf("Enter a number: ");
    scanf("%d", &numberA);

    // Extract the last and second last digit.
    lastDigitOfA = numberA % 10;
    secondLastDigitOfA = (numberA / 10) % 10;

    // construct the modified number.
    modifiedNumber = (numberA / 100) * 100 + lastDigitOfA * 10 + secondLastDigitOfA;

    printf("The number with last two digits swapped is: %d\n", modifiedNumber);

    return 0;
}