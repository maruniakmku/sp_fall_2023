#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    Coordinate point;
    int steps;
} QueueNode;

typedef struct {
    QueueNode items[MAX_SIZE];
    int front;
    int rear;
} Queue;

void initQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(Queue *q) {
    return q->front == -1;
}

void enqueue(Queue *q, QueueNode node) {
    if (q->rear == MAX_SIZE - 1) {
        printf("Queue is full\n");
        exit(1);
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = node;
}

QueueNode dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        exit(1);
    }
    QueueNode node = q->items[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }
    return node;
}

int isValid(int x, int y, int M, int N, char map[][N]) {
    return (x >= 0 && x < M && y >= 0 && y < N && map[x][y] != '1');
}

int findMinSteps(int M, int N, char map[][N], Coordinate start, Coordinate end) {
    int dx[] = {0, 0, 1, -1}; 
    int dy[] = {1, -1, 0, 0}; 

    Queue q;
    initQueue(&q);

    QueueNode startNode = {start, 0};
    enqueue(&q, startNode);

    int visited[M][N];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            visited[i][j] = 0;
        }
    }

    visited[start.x][start.y] = 1;

    while (!isEmpty(&q)) {
        QueueNode currentNode = dequeue(&q);

        if (currentNode.point.x == end.x && currentNode.point.y == end.y) {
            return currentNode.steps;
        }

        for (int i = 0; i < 4; i++) {
            int newX = currentNode.point.x + dx[i];
            int newY = currentNode.point.y + dy[i];

            if (isValid(newX, newY, M, N, map) && !visited[newX][newY]) {
                visited[newX][newY] = 1;
                Coordinate neighbor = {newX, newY};
                QueueNode neighborNode = {neighbor, currentNode.steps + 1};
                enqueue(&q, neighborNode);
            }
        }
    }

    return -1;
}

int main() {
    int M, N;
    scanf("%d %d", &M, &N);

    char map[MAX_SIZE][MAX_SIZE];

    for (int i = 0; i < M; i++) {
        scanf("%s", map[i]);
    }

    Coordinate start, end;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (map[i][j] == 'A') {
                start.x = i;
                start.y = j;
            } else if (map[i][j] == 'B') {
                end.x = i;
                end.y = j;
            }
        }
    }

    int result = findMinSteps(M, N, map, start, end);

    printf("%d\n", result);

    return 0;
}
