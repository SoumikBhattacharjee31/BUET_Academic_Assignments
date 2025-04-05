#include <stdio.h>
#define PI 3.14159

int main() {
    float radius, volume;
    printf("Please enter the radius of the sphere: ");
    scanf("%f",&radius);
    volume = 4/3.0*PI*radius*radius*radius;
    printf("\nThe volume of the sphere is %.2f\n",volume);
    return 0;
}
