#include<stdio.h>
#include<string.h>

int main()
{
    char s [] = "hello world";
    printf("%s\n", s);
    char *p = "hello world";
    p+=6;
    // pointer can be used to get different substrings
    printf("%s\n", p);
    printf("%d\n", strlen(p));
    return 0;
}
