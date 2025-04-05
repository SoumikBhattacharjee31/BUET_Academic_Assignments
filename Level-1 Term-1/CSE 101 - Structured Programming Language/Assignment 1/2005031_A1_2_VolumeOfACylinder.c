#include <stdio.h>
#define PI 3.14159

int main() {
    float radius, height, volume;
    printf("Please enter the radius of the Cylinder: ");
    scanf("%f",&radius);
    printf("\nPlease enter the height of the Cylinder: ");
    scanf("%f",&height);
    volume = PI*radius*radius*height;
    printf("\nThe volume of the Cylinder is %.2f\n",volume);
    return 0;
}
