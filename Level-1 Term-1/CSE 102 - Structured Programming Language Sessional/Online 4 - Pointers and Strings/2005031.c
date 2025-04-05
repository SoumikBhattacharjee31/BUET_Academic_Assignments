#include <stdio.h>

int main()
{
    int i,j,ans,minans;
    char str[1000];

    printf("Enter the string: ");
    scanf("%s",&str);

    minans=ans=strlen(str);

    for(i=0;i<minans;i++)
    {
        for(j=i+1;j<minans;j++)
        {
            if(str[i]==str[j])
            {
                ans=j;

                if(ans<minans)
                {
                    minans=ans;
                }

                break;
            }
        }
    }

    printf("%d",minans);

    return 0;
}
