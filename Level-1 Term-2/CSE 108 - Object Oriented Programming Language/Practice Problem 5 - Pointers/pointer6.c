#include<stdio.h>

int main()
{
    int i;
    int N = 10;
    int *p;
    int a[] = {1, 2, 4, 5, 3, 0, 6, 8, 7, 9};

    printf("Using Array: ");
    for ( i = 0; i < N; i++)
    {
        printf("%d ", a[i]);
        //printf("%d ", i[a]); // same as a[i]
    }
    printf("\n");

    //p = &a[0];
    p = a; // same as p = &a[0]
    //p = &a[2]; // points to 3rd element of the array

    printf("Using Pointer: ");
    for ( i = 0; i < N; i++)
    {
        printf("%d ", *p++);
        // printf("%d ", *(p+i));
    }
    printf("\n");

    return 0;
}
