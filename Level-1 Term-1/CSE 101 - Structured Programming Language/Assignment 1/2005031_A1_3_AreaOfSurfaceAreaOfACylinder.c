#include <stdio.h>
#define PI 3.14159

int main() {
    float radius, height, area;
    printf("Please enter the radius of the Cylinder: ");
    scanf("%f",&radius);
    printf("\nPlease enter the height of the Cylinder: ");
    scanf("%f",&height);
    area = 2*PI*radius*(radius+height);
    printf("\nThe area of the surface area of the Cylinder is %.2f\n",area);
    return 0;
}
