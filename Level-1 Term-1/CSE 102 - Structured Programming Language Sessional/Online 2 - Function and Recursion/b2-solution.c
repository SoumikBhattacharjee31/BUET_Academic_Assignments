int remove_n_digits(int n, int d, int c)
{
    if (c==0) return n;
    if (n==0) return 0;
    if (n%10==d) return remove_n_digits(n/10, d, c-1);
    return 10*remove_n_digits(n/10, d, c)+n%10;
}