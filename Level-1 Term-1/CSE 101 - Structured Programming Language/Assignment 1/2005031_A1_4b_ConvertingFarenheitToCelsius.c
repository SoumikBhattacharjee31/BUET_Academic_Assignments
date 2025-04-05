#include <stdio.h>

int main() {
    float celsius, fahrenheit;
    printf("Please enter the temperature in Fahrenheit scale: ");
    scanf("%f",&fahrenheit);
    celsius = 5/9.0*(fahrenheit-32);
    printf("\nThe temperature in Celsius scale is %.2f\n",celsius);
    return 0;
}
