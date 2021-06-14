#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

enum COLOR{ WHITE, GRAY, BLACK };
enum TYPE{ MATRIX, LIST };

int ns[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };
double ds[] = { 1, 2, 3, 4, 5, 6, 7 };

struct Q {
    int length;
    int head;
    int tail;
    int *tab;
};

struct S {
    int top;
    int *tab;
};

//queue methods

struct Q *Q_new(int _length) {
    struct Q *temp = (struct Q *)malloc(sizeof(struct Q));
    temp->length = _length;
    temp->tab = (int *)malloc(sizeof(int) * (_length));
    temp->head = 0;
    temp->tail = 0;
    return temp;
}

void Q_del(struct Q *temp) {
    free(temp->tab);
    free(temp);
}

void enQueue(struct Q *temp, int x) {
    temp->tab[temp->tail] = x;
    if (temp->tail == temp->length - 1) {
        temp->tail = 0;
    }
    else {
        temp->tail++;
    }
}

int deQueue(struct Q *temp) {
    int x = temp->tab[temp->head];
    if (temp->head == temp->length - 1) {
        temp->head = 0;
    }
    else {
        temp->head++;
    }
    return x;
}

bool isEmpty(struct Q *temp) {
    if (temp->head == temp->tail)
        return true;
    return false;
}

//stack methods

struct S *S_new(int _length) {
    struct S *temp = (struct S *)malloc(sizeof(struct S));
    temp->tab = (int *)malloc(_length * sizeof(int));
    temp->top = 0;
    return temp;
}

void S_del(struct S *temp) {
    free(temp->tab);
    free(temp);
}

void push(struct S *temp, int x) {
    temp->tab[temp->top] = x;
    temp->top++;
}

int pop(struct S *temp) {
    temp->top--;
    return temp->tab[temp->top];
}

bool isEmptyS(struct S *temp) {
    if (temp->top == 0)
        return true;
    return false;
}

// algorithms

//dfs
void dfs_visit(int **G, int u, int *colors, int n, int type) {
  colors[u] = GRAY;
  for (unsigned int v = 0; v < n; v++) {
    if (G[u][v] == 1 && colors[v] == WHITE && type == MATRIX) {
      dfs_visit(G, v, colors, n, type);
    }
    if (G[u][v] > 0 && colors[G[u][v] - 1] == WHITE && type == LIST) {
      dfs_visit(G, G[u][v] - 1, colors, n, type);
    }
  }
  colors[u] = BLACK;
}

void dfs(int **G, int n, int type) {
  int *colors = calloc(n, sizeof(int));
  for (unsigned int i = 0; i < n; i++) {
    colors[i] = WHITE;
  }
  for (unsigned int u = 0; u < n; u++) {
    if (colors[u] == WHITE) {
      dfs_visit(G, u, colors, n, type);
    }
  }
}

//bfs
void bfs(int **G, int s, int n, int type) {
  int *colors = calloc(n, sizeof(int));
  for (unsigned int i = 0; i < n; i++) {
    if (i != s) {
      colors[i] = WHITE;
    }
  }
  colors[s] = GRAY;
  struct Q* queue = Q_new(n);
  enQueue(queue, s);
  while (!isEmpty(queue)) {
    int u = deQueue(queue);
    for (unsigned int v = 0; v < n; v++) {
      if (colors[v] == WHITE && G[u][v] == 1 && type == MATRIX) {
        colors[v] = GRAY;
        enQueue(queue, v);
      }
      if (colors[G[u][v] - 1] == WHITE && G[u][v] > 0 && type == LIST) {
        colors[G[u][v] - 1] = GRAY;
        enQueue(queue, G[u][v] - 1);
      }
    }
    colors[u] = BLACK;
  }
  Q_del(queue);
}

//topological sort
void dfs_visit_sort(int **G, int u, int *colors, int n, struct S* stack, int type) {
  colors[u] = GRAY;
  for (unsigned int v = 0; v < n; v++) {
    if (G[u][v] == 1 && colors[v] == WHITE && type == MATRIX) {
      dfs_visit_sort(G, v, colors, n, stack, type);
    }
    if (G[u][v] > 0 && colors[G[u][v] - 1] == WHITE && type == LIST) {
      dfs_visit_sort(G, G[u][v] - 1, colors, n, stack, type);
    }
  }
  colors[u] = BLACK;
  push(stack, u);
}

void dfs_sort(int **G, int n, int type) {
  struct S* stack = S_new(n);
  int *colors = calloc(n, sizeof(int));
  for (unsigned int i = 0; i < n; i++) {
    colors[i] = WHITE;
  }
  for (unsigned int u = 0; u < n; u++) {
    if (colors[u] == WHITE) {
      dfs_visit_sort(G, u, colors, n, stack, type);
    }
  }
  S_del(stack);
}

//generating graph

int random(int n, int m) {
  return n + rand() % (m - n);
}

void random_directed_graph(int **G, int n, double b) {
  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < n; j++) {
      G[i][j] = 0;
    }
  }
  double m = b * (n * n);
  int u, v;
  for (unsigned int i = 0; i < m; i++) {
    do {
      u = random(0, n);
      v = random(0, n);
    } while (u == v || G[u][v] == 1);
    G[u][v] = 1;
  }
}

void matrix_to_list(int **G, int **L, int n) {
  for (unsigned int i = 0; i < n; i++) {
    int elements = 0;
    for (unsigned int j = 0; j < n; j++) {
      if (G[i][j] == 1) {
        L[i][elements] = j + 1;
        elements++;
      }
    }
  }
}

int main () {
  for (unsigned int i = 0; i < sizeof(ns) / sizeof(*ns); i++) {
    int n = ns[i];

    for (unsigned int j = 0; j < sizeof(ds) / sizeof(*ds); j++) {
      double b = ds[j] / (double)8;
      clock_t begin;
      clock_t end;

      //array initialization
      int **G = calloc(n, sizeof(int *));
      for (unsigned int ii = 0; ii < n; ii++) {
        G[ii] = calloc(n, sizeof(int));
      }
      random_directed_graph(G, n, b);
      //matrix algorithms
      begin = clock();
      dfs(G, n, MATRIX);
      end = clock();
      double seconds_dfs_matrix = (double)(end - begin) / (double) CLOCKS_PER_SEC;

      begin = clock();
      bfs(G, 0, n, MATRIX);
      end = clock();
      double seconds_bfs_matrix = (double)(end - begin) / (double) CLOCKS_PER_SEC;

      dfs_sort(G, n, MATRIX);

      //list initialization
      int **L = calloc(n, sizeof(int *));
      for (unsigned int ii = 0; ii < n; ii++) {
        L[ii] = calloc(n, sizeof(int));
      }
      matrix_to_list(G, L, n);
      //list algorithms
      begin = clock();
      dfs(L, n, LIST);
      end = clock();
      double seconds_dfs_list = (double)(end - begin) / (double) CLOCKS_PER_SEC;

      begin = clock();
      bfs(L, 0, n, LIST);
      end = clock();
      double seconds_bfs_list = (double)(end - begin) / (double) CLOCKS_PER_SEC;

      dfs_sort(L, n, LIST);

      //array clear
      for (unsigned int ii = 0; ii < n; ii++) {
        int* currPtr = G[ii];
        free(currPtr);
      }
      for (unsigned int ii = 0; ii < n; ii++) {
        int* currPtr = L[ii];
        free(currPtr);
      }
      printf("Macierz_sasiedztwa\tDFS\t%d\t%g\t%g\n", n, b, seconds_dfs_matrix);
      printf("Lista_nastepnikow\tDFS\t%d\t%g\t%g\n", n, b, seconds_dfs_list);
      printf("Macierz_sasiedztwa\tBFS\t%d\t%g\t%g\n", n, b, seconds_bfs_matrix);
      printf("Lista_nastepnikow\tBFS\t%d\t%g\t%g\n", n, b, seconds_bfs_list);
    }
  }
  return 0;
}