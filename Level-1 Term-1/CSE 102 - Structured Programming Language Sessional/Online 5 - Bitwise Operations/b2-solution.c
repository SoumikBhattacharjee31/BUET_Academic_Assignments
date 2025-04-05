#include <stdio.h>

int get_int_no_of_bits()
{
    /// e.g. 32
    return sizeof(int) * 8;
}

void print_int_bits(int e)
{
    int no_of_bits = get_int_no_of_bits(); /// e.g. 32

    /// extracts Sign Bit
    printf("%d", ((e & (1 << (no_of_bits - 1))) != 0));

    for (int i = (1 << (no_of_bits - 2)); i != 0; i >>= 1)
    {
        /// extracts MSB digits one after another
        int bit_i = ((e & i) != 0);
        printf("%d", bit_i);
    }
}

int get_day(unsigned int N)
{
    /// Here, day will require rightmost (LSB) 5 bits.
    /// 31 => 00000000000000000000000000011111
    /// e.g. For 978750 => 00000000000000000000000000011110 + 1
    return (N & 31) + 1;
}

int get_month(unsigned int N)
{
    /// Here, month will require 4 bits one left (MSB) of day bits.
    /// 15 => 00000000000000000000000000001111
    /// e.g. For 978750 => 00000000000000000000000000001001 + 1
    return ((N >> 5) & 15) + 1;
}

int get_year(unsigned int N)
{
    /// Here, year will consist of all the bits except 9 (=5+4) LSB bits.
    /// e.g. For 978750 => 00000000000000000000011101110111
    return (N >> 9);
}

int main()
{
    unsigned int N;

    scanf("%u", &N);
    printf("%u => ", N);

    /// e.g. 978750 => 00000000000011101110111100111110
    print_int_bits(N);
    printf("\n");

    // printf("%.2d/%.2d/%.2d", get_year(N), get_month(N), get_day(N));

    int day = get_day(N);
    printf("extracted day bits => ");
    print_int_bits(day - 1);
    printf("\n");

    int month = get_month(N);
    printf("extracted month bits => ");
    print_int_bits(month - 1);
    printf("\n");

    int year = get_year(N);
    printf("extracted year bits => ");
    print_int_bits(year);
    printf("\n");

    printf("Date: %.2d/%.2d/%.2d", year, month, day);

    return 0;
}