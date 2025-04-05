#include<stdio.h>

void swap(int a, int b)
{
    printf("In swap, location of a = %p, location of b = %p\n", &a, &b);
    int t = a;
    a = b;
    b = t;
    printf("Inside swap: a = %d, b = %d\n", a, b);
}

void swap2(int *a, int *b)
{
    printf("In swap, location of a = %p, location of b = %p\n", a, b);
    int t = *a;
    *a = *b;
    *b = t;
    printf("Inside swap: a = %d, b = %d\n", *a, *b);
}

int main()
{
    int a, b;
    printf("Enter a: ");
    scanf("%d", &a);
    printf("Enter b: ");
    scanf("%d", &b);
    printf("Before swap: a = %d, b = %d\n", a, b);
    printf("In main, location of a = %p, location of b = %p\n", &a, &b);
    //swap(a, b); // call by value
    swap2(&a, &b); // call by reference
    printf("After swap: a = %d, b = %d\n", a, b);
    return 0;
}
