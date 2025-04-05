#include <stdio.h>
int main()
{
    int n,i;
    printf("Enter a number: ");
    scanf("%d",&n);
    for(i=2;i<n;i++)
    {
        if(n%i==0)
        {
            printf("Not prime");
            return 0;
        }
    }
    printf("Prime");
    return 0;
}
