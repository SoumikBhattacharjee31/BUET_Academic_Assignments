#include<stdio.h>

int main()
{
    char aname[][10] = {"Invalid", "Jan", "Feb", "Mar"};
    // 0 - 9: Invalid, 10 - 19: Jan, 20 - 29: Feb ...
    char *pname[] = {"Invalid", "Jan", "Feb", "Mar"};
    // pname[0] points to Invalid, pname[1] points to Jan, pname[2] points to Feb ...

    int a[3][4]; // allocates a true 2D array, 3 rows and 4 columns for each row (uniform)

    int *b[3]; // allocates 3 pointers and doesn't initialize them

    int x = 1;
    int y[] = {1, 2};
    int z[] = {1, 2, 3};

    b[0] = &x;
    b[1] = y;
    b[2] = z;

    return 0;
}
