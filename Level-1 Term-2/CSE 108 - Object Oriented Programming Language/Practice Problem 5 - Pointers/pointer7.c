#include<stdio.h>

int main()
{
    int i, j;
    int M = 3, N = 4;
    int *p;
    int a[3][4] = {1, 2, 4, 5, 3, 0, 6, 8, 7, 9, 11, 10};

    printf("Using Array:\n");
    for ( i = 0; i < M; i++)
    {
        for ( j =0; j < N; j++ )
        {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }

    //p = a; // not recommended
    //p = (int *) a ; //  better than p = a
    p = a[0];
    //p = &a[0][0];

    printf("Using Pointer:");
    for ( i = 0; i < M * N; i++)
    {
        if ( i%N == 0) printf("\n");
        printf("%d ", *p++);
    }
    printf("\n");

    p = a[0];
    printf("Using Pointer2:\n");
    for ( i = 0; i < M; i++)
    {
        for ( j =0; j < N; j++ )
        {
            printf("%d ",  * ( p + N * i + j ) );
        }
        printf("\n");
    }

    return 0;
}
