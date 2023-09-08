#include <stdio.h>

struct ieee_754_double {
    double number;
    unsigned long long sign;
    unsigned long long biased_exponent;
    unsigned long long fraction2;
};

struct ieee_754_double deconstruct_double(double number) {
    /*
     * Deconstructs an IEEE 754 double number into its parts.
     *
     * sign - 1 bit,
     * biased exponent - 11 bits,
     * fraction (base 2) - 52 bits.
     *
     * https://en.wikipedia.org/wiki/Double-precision_floating-point_format
     */

    unsigned long long* pnumber = (unsigned long long*)&number;

    unsigned long long sign = (*pnumber & 0x8000000000000000) >> 63;
    unsigned long long biased_exponent = ((*pnumber & 0x7FF0000000000000) >> 52);
    unsigned long long fraction2 = *pnumber & 0x000FFFFFFFFFFFFF;

    struct ieee_754_double result;
    result.number = number;
    result.sign = sign;
    result.biased_exponent = biased_exponent;
    result.fraction2 = fraction2;
    return result;
}

void explain_double(double number) {
    struct ieee_754_double d = deconstruct_double(number);

    unsigned long long binary = *(unsigned long long*)&d.number;

    printf("double number: %0.20f, 0x%llx\n", d.number, binary);

    printf("parts:\n");
    printf("  sign: %lld, 0x%llx\n", d.sign, d.sign);
    printf("  biased exponent: %lld, 0x%llx\n", d.biased_exponent, d.biased_exponent);
    printf("  exponent: %lld, 0x%llx\n", d.biased_exponent - 1023, d.biased_exponent - 1023);
    printf("  fraction (base 2): %lld, 0x%llx\n", d.fraction2, d.fraction2);

    printf("\n");
}

void demo1()
{
    printf("\nDouble numbers explained\n");
    explain_double(1.0);
    explain_double(2.0);
    explain_double(5.0);
    explain_double(-5.0);
}

void demo2()
{
    printf("\nSuprise #1\n");
    if (0.1 + 0.2 == 0.3) {
        printf("0.1 + 0.2 = 0.3\n");
    }
    else {
        printf("0.1 + 0.2 != 0.3\n");
        printf("%0.20lf + %0.20lf = %0.20lf != %0.20lf\n", 0.1, 0.2, 0.1 + 0.2, 0.3);
    }
}

void demo3()
{
    printf("\nSurprise #2\n");
    if (0.0 == -0.1)
        printf("0.0 = -0.0\n");
    else
        printf("0.0 != -0.0\n");
}

int main(int argc, char** argv) {
    demo1();
    demo2();
    demo3();
    return 0;
}
