#include <stdio.h>

int main() {
    float celsius, fahrenheit;
    printf("Please enter the temperature in Celsius scale: ");
    scanf("%f",&celsius);
    fahrenheit = 9/5.0*celsius+32;
    printf("\nThe temperature in Fahrenheit scale is %.2f\n",fahrenheit);
    return 0;
}
