#include<stdio.h>

int main() {
    // creating a character array to store the value of
    // our string, notice the size of array is
    // 11 = length("HelloWorld") + 1
    char *ptr = "HelloWorld";


    // pointer variable
    // char *ptr = str;

    // creating a while loop till we don't find
    // a null character in the string
    int len = 0;
    while (*ptr != '\0') {
        // the current character is not \0
        // so we will print the character
        printf("%c", *ptr);

        // move to the next character.
        ptr++;
        len++;
    }

    printf("String length %d",len);
    return 0;
}
