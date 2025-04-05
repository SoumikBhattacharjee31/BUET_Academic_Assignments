#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char name[100];
    char *description;

    strcpy(name, "John Doe");

    // allocate memory dynamically
    description = (char *) malloc( 50 * sizeof(char) );
    if( description == NULL )
    {
        printf("Allocation Error!\n");
        exit(1);
    }
    strcpy( description, "John Doe is an undergraduate student. ");

    // suppose you want to store bigger description
    description = realloc(description, 100 * sizeof(char) );

    if( description == NULL )
    {
        printf("ReAllocation Error!\n");
        exit(1);
    }

    strcat( description, "He is in Level 1 Term 1.");

    printf("Name: %s\n", name );
    printf("Description: %s\n", description );

    // release memory using free() function
    free(description);
}
