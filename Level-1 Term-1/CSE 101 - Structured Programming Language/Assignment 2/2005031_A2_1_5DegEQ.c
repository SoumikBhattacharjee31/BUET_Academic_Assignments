#include <stdio.h>
#define INTMIN -32768
#define INTMAX 32767
#define F(x) x*x*x*x*x+3*x*x-10

int main()
{
    float n, ans;
    for (n=INTMIN; n<INTMAX; n++)
    {
        if(F(n)>=0)
        {
            printf("%.1f\n",n-0.5);
            for(; n<INTMAX; n++)
            {
                if(F(n)<=0)
                {
                    if(n==0) printf("%.1f\n",n);
                    else printf("%.1f\n",n-0.5);
                }
            }
        }
    }
    return 0;
}
