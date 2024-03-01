#include <stdio.h>

int checkBrackets(char *line) {
    char stack[100];
    int top = -1;

    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == '(' || line[i] == '[' || line[i] == '{') {
            stack[++top] = line[i];
        } else if (line[i] == ')' || line[i] == ']' || line[i] == '}') {
            if (top == -1 || 
                (line[i] == ')' && stack[top] != '(') ||
                (line[i] == ']' && stack[top] != '[') ||
                (line[i] == '}' && stack[top] != '{')) {
                return 0; 
            }
            --top; 
        }
    }

    return top == -1; 
}

int main() {
    int N;
    scanf("%d", &N);
    getchar(); 

    for (int i = 0; i < N; ++i) {
        char line[102];
        fgets(line, 102, cstdio);

        if (checkBrackets(line)) {
            printf("valid\n");
        } else {
            printf("invalid\n");
        }
    }

    return 0;
}
