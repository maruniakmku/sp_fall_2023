#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr = malloc(1000000000000); 
    if (ptr == NULL) {
        printf("Нестача пам'яті. Програма завершується.\n");
        exit(EXIT_FAILURE);
    }

    free(ptr);

    return 0;
}