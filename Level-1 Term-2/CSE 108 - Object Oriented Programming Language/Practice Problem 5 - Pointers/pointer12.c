#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void * mymalloc(int size)
{
    void * ptr = malloc(size);
    if (ptr == NULL) return NULL;
    memset(ptr, 0, size); // initialize the newptr
    return ptr;
}

void * myrealloc(void *oldptr, int oldSize, int newSize)
{
    void * newptr = malloc(newSize);
    if (newptr == NULL) return NULL;
    memcpy(newptr, oldptr, oldSize); // copy from oldptr to newptr
    free(oldptr);
    return newptr;
}

int main()
{
    char name[100];
    char *description;

    strcpy(name, "John Doe");

    description = (char *) mymalloc( 50 * sizeof(char) );
    if( description == NULL )
    {
        printf("Allocation Error!\n");
        exit(1);
    }

    strcpy( description, "John Doe is an undergraduate student. ");

    description = myrealloc(description, 50 * sizeof(char), 100 * sizeof(char));

    if( description == NULL )
    {
        printf("ReAllocation Error!\n");
        exit(1);
    }

    strcat( description, "He is in Level 1 Term 1.");

    printf("Name: %s\n", name );
    printf("Description: %s\n", description );

    free(description);
}
