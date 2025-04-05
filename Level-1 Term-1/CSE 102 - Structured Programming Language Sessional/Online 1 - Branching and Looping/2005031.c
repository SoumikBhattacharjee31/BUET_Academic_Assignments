#include <stdio.h>

int main()
{
    int n, digit, ans = 0, i = 1;
    scanf("%d", &n);
    while (n != 0)
    {
        digit = n % 10;
        n /= 10;
        if (digit % 2 == 0)
        {
            ans += ((digit + 1) * i);
            i *= 10;
        }
    }
    printf("%d", ans);
    return 0;
}
