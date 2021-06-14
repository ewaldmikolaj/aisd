#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

enum COLOR{ WHITE, GRAY, BLACK };
#define REPETITIONS 20

//int ns[] = { 10, 12, 14, 16, 18 };
int ns[] = { 50, 75, 100, 125, 150, 175, 200, 225};
double ds[] = { 1, 2, 3, 4, 5 };

struct S {
    int top;
    int *tab;
};

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

//support functions

int random(int n, int m) {
  return n + rand() % (m - n);
}

bool find(int *arr, int a, int n) {
  for (unsigned int i = 0; i < n; i++) {
    if (arr[i] == a) {
      return true;
    }
  }
  return false;
}

int edge_count(int *arr, int n) {
  int counter = 0;
  for (unsigned int i = 0; i < n; i++) {
    if (arr[i] == 1) {
      counter++;
    }
  }
  return counter;
}

int edge_count_all(int **arr, int n) {
  int counter = 0;
  for (unsigned int i = 0; i < n; i++) {
    counter += edge_count(arr[i], n);
  }
  return counter;
}

void dfs_visit(int **G, int u, int *colors, int n) {
  colors[u] = GRAY;
  for (unsigned int v = 0; v < n; v++) {
    if (G[u][v] == 1 && colors[v] == WHITE) {
      dfs_visit(G, v, colors, n);
    }
  }
  colors[u] = BLACK;
}

int dfs_counter(int **G, int n, int u) {
  int *colors = calloc(n, sizeof(int));
  for (unsigned int i = 0; i < n; i++) {
    colors[i] = WHITE;
  }
  dfs_visit(G, u, colors, n);
  int counter = 0;
  for (unsigned int i = 0; i < n; i++) {
    if (colors[i] == BLACK)
      counter++;
  }
  return counter;
}

bool is_bridge(int **G, int n, int u, int v) {
  int with_edge = dfs_counter(G, n, u);
  G[u][v] = 0; G[v][u] = 0;
  int without_edge = dfs_counter(G, n, u);
  G[u][v] = 1; G[v][u] = 1;
  return without_edge != with_edge ? true : false;
}

//Hierholzer

void find_cycle_hierholzer(int **G, int n) {
  int edges = edge_count_all(G, n);
  struct S* stack = S_new(edges + 1);
  struct S* solution = S_new(edges + 1);
  push(stack, 0);
  while (!isEmptyS(stack)) {
    if (edge_count(G[ stack->tab[stack->top - 1] ], n) > 0) {
      for (unsigned int i = 0; i < n; i++) {
        if (G[ stack->tab[stack->top - 1] ][i] == 1) { //edge exist
          G[ stack->tab[stack->top - 1] ][i] = 0;
          G[i][ stack->tab[stack->top - 1] ] = 0;
          push(stack, i);
        }
      }
    } else {
      push(solution, pop(stack));
    }
  }
  free(stack);
  free(solution);
}

//fallback algorithm Hamiltonian cycle

bool find_cycle_fallback(int **G, int n, int *path, int m) {
  int u = path[m - 1];
  if (m == n) {
    int v = path[0];
    return G[u][v] == 1 ? true : false;
  } else {
    for (unsigned int v = 0; v < n; v++) {
      if (G[u][v] == 1 && !find(path, v, n)) {
        path[m] = v;
        if (find_cycle_fallback(G, n, path, m + 1)) {
          return true;
        } else {
          path[m] = -1;
        }
      }
    }
  }
  return false;
}

void find_hamiltonian_cycle(int **G, int n) {
  int *path = calloc(n, sizeof(int));
  for (unsigned int i = 0; i < n; i++) {
    path[i] = -1;
  }
  path[0] = 0;
  find_cycle_fallback(G, n, path, 1);
  free(path);
}

//Fleury

void find_cycle_fleury(int **G, int n) {
  struct S* solution = S_new(edge_count_all(G, n));
  push(solution, 0);
  int stack_top, vertex;
  while(edge_count_all(G, n) != 0) {
    stack_top = solution->tab[solution->top - 1];
    vertex = -1;
    for (unsigned int i = 0; i < n; i++) {
      if (G[stack_top][i] == 1 && !is_bridge(G, n, stack_top, i)) {
        G[stack_top][i] = 0; G[i][stack_top] = 0;
        push(solution, i);
        vertex = -1;
        break;
      } else if (G[stack_top][i] == 1) {
        vertex = i;
      }
    }
    if (vertex != -1) {
      G[stack_top][vertex] = 0; G[vertex][stack_top] = 0;
      push(solution, vertex);
    }
  }
  // while (!isEmptyS(solution)) {
  //   printf("%d, ", pop(solution));
  // }
  free(solution);
}

//generating graph

void hamiltonian_cycle(int **G, int n) {
  int *hamilton = calloc(n + 1, sizeof(int));
  hamilton[0] = 0;
  int u;
  for (unsigned int i = 1; i < n; i++) {
    do {
      u = random(0, n);
    } while (find(hamilton, u, n));
    hamilton[i] = u;
  }
  hamilton[n] = 0;
  for (unsigned int i = 0; i < n; i++) {
    G[ hamilton[i] ][ hamilton[i + 1] ] = 1;
    G[ hamilton[i + 1] ][ hamilton[i] ] = 1;
  }
}

//hamiltonian, euler graph
void create_graph(int **G, int n, double b) { 
  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < n; j++) {
      G[i][j] = 0;
    }
  }
  double m = (b * n * (n - 1)) / 2.0;
  int num_edge = n;
  int u, v, w;
  hamiltonian_cycle(G, n);
  while (num_edge < m) {
    do {
      u = random(0, n);
      v = random(0, n);
      w = random(0, n);
    } while (u == v || G[u][v] == 1 || u == w || v == w || G[u][w] == 1 || G[v][w] == 1);
    G[u][v] = 1; G[v][u] = 1;
    G[u][w] = 1; G[w][u] = 1;
    G[v][w] = 1; G[w][v] = 1;
    num_edge += 3;
  }
}

//random graph
void random_graph(int **G, int n, double b) {
  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < n; j++) {
      G[i][j] = 0;
    }
  }
  double m = (b * n * (n - 1)) / 2.0;
  int u, v;
  for (unsigned int i = 0; i < m; i++) {
    do {
      u = random(0, n);
      v = random(0, n);
    } while (u == v || G[u][v] == 1);
    G[u][v] = 1; G[v][u] = 1;
  }
  u = random(0, n);
  for (unsigned int i = 0; i < n; i++) {
    if (G[u][i] == 1) {
      G[u][i] = 0; G[i][u] = 0;
    }
  }
  do {
    v = random(0, n);
  } while (u == v);
  G[u][v] = 1; G[v][u] = 1;
}

//main
char *cycle_names[] = { "hierholzer", /*"fallback",*/ "fleury" };
void (*cycle_functions[])(int **G, int n) = { find_cycle_hierholzer, /*find_hamiltonian_cycle,*/  find_cycle_fleury};
// char *cycle_names[] = { "fallback" };
// void (*cycle_functions[])(int **G, int n) = { find_hamiltonian_cycle };

int main() {
  for (unsigned int i = 0; i < sizeof(cycle_functions)/sizeof(*cycle_functions); i++) {
    void (*find_cycle)(int **G, int n) = cycle_functions[i];

    for (unsigned int j = 0; j < sizeof(ns)/sizeof(*ns); j++) {
      int n = ns[j];

      for (unsigned int k = 0; k < (int)sizeof(ds)/sizeof(*ds); k++) {
        double b = ds[k] / 8.0;
        clock_t finding_time_total = 0;

        for (unsigned int l = 0; l < REPETITIONS; l++) {
          int **G = calloc(n, sizeof(int *));
          for (unsigned int ii = 0; ii < n; ii++) {
            G[ii] = calloc(n, sizeof(int));
          }

          //random_graph(G, n, b);
          create_graph(G, n, b);
          clock_t finding_time = clock();
          find_cycle(G, n);
          finding_time_total += clock() - finding_time;

          for (unsigned int ii = 0; ii < 0; ii++) {
            free(G[ii]);
          }
          free(G);
        }
        printf("%-6d %-6g %-15s %-12g\n", 
          n,
          b, 
          cycle_names[i], 
          //"creating",
          (double)finding_time_total / CLOCKS_PER_SEC / REPETITIONS);
      }
    }
  }     
  return 0;
}