#include <stdio.h>
int main()
{
    int n, i, j, k;
    printf("Enter the number of rows: ");
    scanf("%d",&n);
    for(i=1; i<=n; i++)
    {
        for(j=n;j>=i;j--) printf("   ");
        for(j=1,k=1; j<=i; j++)
        {
        printf("%6d",k);
        k=k*(i-j)/j;
        }
    printf("\n");
    }
}
