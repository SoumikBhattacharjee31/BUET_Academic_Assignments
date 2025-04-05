#include<iostream>
using namespace std;

int main()
{
    int i, j;
    int rows = 3;
    int columns = 4;
    int count = 1;

    // using new and delete

    int **matrix;

    matrix = new int*[rows];

    for (i = 0; i < rows; i++)
    {
        matrix[i] = new int[columns];
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
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    for (i = 0; i < rows; i++)
    {
        delete [] matrix[i];
    }

    delete[] matrix;


    return 0;
}

