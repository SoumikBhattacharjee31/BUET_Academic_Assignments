#include <stdio.h>

int main()
{
    int n, i, fact=1;
    printf("Enter a valid number to get its factorial: ");
    scanf("%d",&n);
    for (i=1;i<=n;i++) fact*=i;
    printf("%d!=%d",n,fact);
    return 0;
}
