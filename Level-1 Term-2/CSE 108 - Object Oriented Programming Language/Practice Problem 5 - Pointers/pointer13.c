#include<stdio.h>
#include<stdlib.h>

int main()
{
    int * p;
    int N, M, i = 0;

    printf("How many integers: ");
    scanf("%d", &N);

    p = malloc(N * sizeof(int));
    //p = calloc(N, sizeof(int));

    if ( p == NULL)
    {
        printf("Allocation Error!\n");
        exit(1);
    }

    printf("Enter %d integers: ", N);
    for ( i = 0; i < N; i++)
    {
        scanf("%d", p+i);
    }

    printf("Your integers: ");
    for ( i = 0; i < N; i++)
    {
        printf("%d ", *(p+i));
    }
    printf("\n");

    printf("How many more integers: ");
    scanf("%d", &M);

    p = (int*) realloc(p, (M + N)* sizeof(int));
    if ( p == NULL)
    {
        printf("ReAllocation Error!\n");
        exit(1);
    }

    printf("Enter %d integers: ", M);
    for ( i = 0; i < M; i++)
    {
        scanf("%d", p + N + i);
    }

    printf("Your integers: ");
    for ( i = 0; i < N + M; i++)
    {
        printf("%d ", *(p + i));
    }
    printf("\n");

    free(p);

    return 0;
}
