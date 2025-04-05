#include <stdio.h>
int main()
{
    int n, i;
    printf("Enter the number of numbers: ");
    scanf("%d",&n);
    double x[n], f[n], sumx=0, sumf=0;
    for (i=0;i<n;i++)
    {
        printf("Enter the number: ");
        scanf("%lf",&x[i]);
        printf("Enter its weight: ");
        scanf("%lf",&f[i]);
        sumx+=x[i]*f[i];
        sumf+=f[i];
    }
    printf("The weighted average: ");
    if (sumf==1) printf("%lf", sumx/sumf);
    else printf("%lf", sumx/sumf);
    return 0;
}
