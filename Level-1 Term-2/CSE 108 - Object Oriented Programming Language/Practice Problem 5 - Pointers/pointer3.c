#include<stdio.h>

int main()
{
    int a = 10;
    int *p = &a;

    printf("a = %d\n", a);
    printf("p = %p\n", p);
    printf("*p= %d\n", *p);

    p++;

    printf("a = %d\n", a);
    printf("p = %p\n", p);
    printf("*p= %d\n", *p);
    /*
    double a = 10;
    double *p = &a;

    printf("a = %lf\n", a);
    printf("p = %p\n", p);
    printf("*p= %lf\n", *p);

    p++;

    printf("a = %lf\n", a);
    printf("p = %p\n", p);
    printf("*p= %lf\n", *p);
    */
    return 0;
}
