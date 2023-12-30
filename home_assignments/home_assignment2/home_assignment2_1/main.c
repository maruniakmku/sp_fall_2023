#include <stdio.h>
#include <stdbool.h>

bool checkBrackets(char *line) {
    int roundBrackets = 0, squareBrackets = 0, curlyBrackets = 0;

    for (int i = 0; line[i] != '\0'; ++i) {
        switch (line[i]) {
            case '(': roundBrackets++; break;
            case ')': roundBrackets--; break;
            case '[': squareBrackets++; break;
            case ']': squareBrackets--; break;
            case '{': curlyBrackets++; break;
            case '}': curlyBrackets--; break;
        }

        if (roundBrackets < 0 || squareBrackets < 0 || curlyBrackets < 0) {
            return false;
        }
    }

    return (roundBrackets == 0 && squareBrackets == 0 && curlyBrackets == 0);
}

int main() {
    int N;
    scanf("%d", &N);
    
    while (getchar() != '\n');

    for (int i = 0; i < N; ++i) {
        char line[102];
        fgets(line, 102, stdin);

        if (checkBrackets(line)) {
            printf("valid\n");
        } else {
            printf("invalid\n");
        }
    }

    return 0;
}