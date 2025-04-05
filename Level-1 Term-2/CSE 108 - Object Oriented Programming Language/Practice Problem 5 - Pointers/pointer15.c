#include<stdio.h>
#include<stdlib.h>

int * f1()
{
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // locally declared array not visible after this function
    return a; // bad idea to return
}

int * f2()
{
    static int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // static so will be visible by the main function
    return a;
}

int * f3()
{
    int * a = (int *) malloc(10); // dynamically allocated memory, will be visible by the main function
    for ( int i = 0; i < 10; i++)
    {
        *(a+i) = i + 1;
    }
    return a;
}

int main()
{
    //int * a = f1(); // not right
    //int * a = f2(); // OK
    int * a = f3();  // OK
    for ( int i = 0; i < 10; i++)
    {
        printf("%d ", *a++);
    }
    printf("\n");
    return 0;
}
