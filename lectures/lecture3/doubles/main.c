#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void explain_double(double d);
void bits_str(double d, char* s);
unsigned long long int sign(double d);
unsigned long long int biased_exponent(double d);
double significand(double d);
const char* class_str(double d);
int is_normal_or_subnormal(double d);

int main(int argc, char** argv)
{
    int i;
    double d;
    if (argc > 1) {
        for (i = 1; i < argc; ++i) {
            sscanf(argv[i], "%lf", &d);
            explain_double(d);
        }
    }
    else {
        while (1) {
            printf("Enter double value: ");
            if (scanf("%lf", &d) == EOF)
                break;
            else
                explain_double(d);
        }
    }
    return 0;
}

/*
 * Ouputs the following view on the provided value of type double.
 * 5.000000
 *
 *       0100000000010100000000000000000000000000000000000000000000000000
 * |1 bit||11 bits  ||                52 bits                           |
 * | sign||bias exp.||               significand                        |
 *
 * classification:     normal
 *
 * biased exponent:    1025
 * significand:        0.250000
 *
 * exponent:           2
 * fraction:           1.250000
 *
 * formula:            (-1)^0 * 1.250000 * 2^2
 * WolframAlpha:       https://www.wolframalpha.com/input?i=(-1)^0*1.250000*2^2
 *
 */
void explain_double(double d)
{
    char bits[65];
    bits_str(d, &bits[0]);
    unsigned long long int b = biased_exponent(d);
    double s = significand(d);
    long long int e = b > 0 ? b - 1023ll : -1022ll;
    double f = b > 0 ? 1.0 + s : s;
    unsigned long long int _sign = sign(d);

    printf("%0.20lf\n", d);
    printf("\n");
    printf("      %s\n", bits);
    printf("|1 bit||11 bits  ||                52 bits                           |\n");
    printf("| sign||bias exp.||               significand                        |\n");
    printf("\n");
    printf("%-20s%s\n", "classification:", class_str(d));
    printf("\n");

    if (!is_normal_or_subnormal(d))
        return;

    printf("%-20s%llu\n", "biased exponent:", b);
    printf("%-20s%0.20lf\n", "significand:", s);
    printf("\n");
    printf("%-20s%lld\n", "exponent:", e);
    printf("%-20s%0.20lf\n", "fraction:", f);
    printf("\n");
    printf("%-20s(-1)^%lld * %0.20lf * 2^%lld\n", "formula: ", _sign, f, e);
    printf("%-20shttps://wolframalpha.com/input?i=(-1)%%5E%lld*%0.20lf*2%%5E%lld\n", "WolframAlpha: ", _sign, f, e);
    printf("\n");
}

void bits_str(double d, char* s)
{
    unsigned long long int d_as_int = *(unsigned long long int*)(&d);
    unsigned long long int mask = 1ull << 63;
    int i;
    for (i = 0; i <= 63; ++i) {
        s[i] = d_as_int & mask ? '1' : '0';
        mask >>= 1;
    }
    s[64] = '\0';
}

unsigned long long int sign(double d)
{
    unsigned long long int d_as_int = *(unsigned long long int*)(&d);
    return d_as_int >> 63;
}

unsigned long long int biased_exponent(double d)
{
    unsigned long long int d_as_int = *(unsigned long long int*)(&d);
    return (d_as_int & ((1ull << (62 + 1)) - 1)) >> 52;
}

double significand(double d)
{
    unsigned long long int d_as_int = *(unsigned long long int*)(&d);
    double significand = 0.0;
    double f = 1.0;
    int i;
    for (i = 51; i >= 0; --i) {
        f /= 2.0;
        if (d_as_int & (1ll << i)) {
            significand += f;
        }
    }
    return significand;
}

const char* class_str(double d) {
    switch (fpclassify(d)) {
    case FP_INFINITE:  return "Inf";
    case FP_NAN:       return "NaN";
    case FP_NORMAL:    return "normal";
    case FP_SUBNORMAL: return "subnormal";
    case FP_ZERO:      return "zero";
    default:           return "unknown";
    }
}

int is_normal_or_subnormal(double d) {
    switch (fpclassify(d)) {
    case FP_INFINITE:  return 0;
    case FP_NAN:       return 0;
    case FP_NORMAL:    return 1;
    case FP_SUBNORMAL: return 1;
    case FP_ZERO:      return 0;
    default:           return 0;
    }
}
