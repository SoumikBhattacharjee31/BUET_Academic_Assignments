#include<stdio.h>

int main()
{
    int a;
    int *p; // a pointer to int

    a = 10;
    p = &a; // p is a pointer variable that points to a

    printf("a = %d\n", a);
    printf("p = %p\n", p);

    printf("*p = %d\n", *p); // *p is used to get the value of a

    *p = 100; // *p can also be used to set the value of a
    printf("a = %d\n", a);

    return 0;
}
