#include <stdio.h>

void printGrid(int N, int grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c", (grid[i][j] == 1) ? '1' : '0');
        }
        printf("\n");
    }
}

void iterate(int N, int grid[N][N]) {
    int newGrid[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int neighbors = 0;

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (i + x >= 0 && i + x < N && j + y >= 0 && j + y < N) {
                        neighbors += grid[i + x][j + y];
                    }
                }
            }

            if (grid[i][j] == 1) {
                neighbors--; 

                if (neighbors < 2 || neighbors > 3) {
                    newGrid[i][j] = 0; 
                } else {
                    newGrid[i][j] = 1; 
                }
            } else {
                if (neighbors == 3) {
                    newGrid[i][j] = 1; 
                } else {
                    newGrid[i][j] = 0; 
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = newGrid[i][j];
        }
    }
}

int main() {
    int N;
    scanf("%d", &N);

    int grid[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char cell;
            scanf(" %c", &cell);
            grid[i][j] = (cell == '1') ? 1 : 0;
        }
    }

    int iterations;
    scanf("%d", &iterations);

    for (int i = 0; i < iterations; i++) {
        iterate(N, grid);
    }

    printGrid(N, grid);

    return 0;
}
