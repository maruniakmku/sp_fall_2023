#include <stdio.h>

union integer_in_memory {
    unsigned int value;
    unsigned char bytes[4];
};

void integer_demo(int demo_value)
{
    union integer_in_memory a;
    printf("Is your computer little-endian or big-endian?\n");
    a.value = demo_value;
    printf("integer\n");
    printf("address, size, value\n");
    printf("0x%p, %ld, 0x%x\n", &a.value, sizeof(a.value), a.value);
    int i;
    for (i = 0; i < sizeof(a.bytes); ++i) {
        printf("0x%p, %ld, 0x%x\n", &a.bytes[i], sizeof(a.bytes[i]), a.bytes[i]);
    }
}

int main()
{
    integer_demo(0x12345678);
}
