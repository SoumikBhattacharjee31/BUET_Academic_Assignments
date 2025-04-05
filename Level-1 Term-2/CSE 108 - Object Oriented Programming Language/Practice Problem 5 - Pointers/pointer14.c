#include<stdio.h>
#include<stdlib.h>

int main()
{
    int i, j;
    int rows = 3;
    int columns = 4;
    int count = 1;

    // first way using only a pointer
    /*
    int * matrix  = (int *)malloc(rows * columns * sizeof(int));

    for ( i = 0; i <  rows; i++ ) {
      for ( j = 0; j < columns; j++ ) {
            *(matrix + i*columns + j) = count++;
      }
    }

    for ( i = 0; i <  rows; i++ ) {
      for ( j = 0; j < columns; j++ ) {
          printf("%d ", *(matrix + i*columns + j) );
      }
      printf("\n");
    }

    free(matrix);
    */
    // end first way


    // second way using array of pointers
    /*
    int *matrix[rows];
    for ( i=0; i< rows; i++ )
         matrix[i] = (int *)malloc(columns * sizeof(int));

    for ( i = 0; i <  rows; i++ )
      for ( j = 0; j < columns; j++ )
            matrix[i][j] = count++;

    for ( i = 0; i <  rows; i++ ) {
      for ( j = 0; j < columns; j++ ) {
          printf("%d ", matrix[i][j] );
      }
      printf("\n");
    }

    for (i=0; i<rows; i++) {
        free(matrix[i]);
    }
    */
    // end second way


    // third way using poiner to pointer

    int **matrix;
    // allocate rows numbers pointers, thats why sizeof(int *) in parameter and returns (int **)
    matrix = (int **) malloc(rows * sizeof(int *));

    for (i = 0; i < rows; i++)
    {
        matrix[i] = (int *) malloc(columns * sizeof(int));
    }

    for ( i = 0; i < rows; i++)
    {
        for ( j =0; j < columns; j++ )
        {
            matrix[i][j] = count++;
        }
    }

    for ( i = 0; i < rows; i++)
    {
        for ( j =0; j < columns; j++ )
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
    // end third way


    return 0;
}

