#include<stdio.h>

int main()
{
    int a = 10;
    int *p = &a;

    printf("a = %d\n", a);
    printf("p = %p\n", p);
    printf("*p= %d\n", *p);

    // unary ++ and * works from right to left
    ++*p; // indirection and then increment
    printf("%d %p %d\n", a, p, *p);

    //*p++; // increment and then indirection
    //printf("%d %p %d\n", a, p, *p);

    (*p)++; // indirection and then increment
    printf("%d %p %d\n", a, p, *p);

    p = p + 2; // valid
    printf("After add p = %p\n", p);
    p = p  - 2; // valid
    printf("After sub p = %p\n", p);
    // p = p * 1; // multiplication is not valid
    // p = p / 1; // division is not valid
    int *q = &a;
    int diff  = p - q;
    printf("Pointer Difference : %d\n", diff);

    return 0;
}
