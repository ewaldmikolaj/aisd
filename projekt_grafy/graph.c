#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

enum COLOR{ WHITE, GREY, BLACK };

int ns[] = { 10 };
double ds[] = { 1, 2, 3, 4, 5, 6, 7 };

//do poprawienia ten dfs
void dfs_visit(int **G, int u1, int u2, int **colors, int n) {
  colors[u1][u2] = GREY;
  for (int i = u1; i < n; i++) {
    for (int j = u2; j < n; j++) {
      if (colors[i][j] == WHITE && G[i][j] == 1) {
        dfs_visit(G, i, j, colors, n);
      }
    }
  }
  colors[u1][u2] = BLACK;
}

void dfs(int **G, int n) {
  //creating array of sizes G
  int **colors = calloc(n, sizeof(int *));
  for (int ii = 0; ii < n; ii++) {
    colors[ii] = calloc(n, sizeof(int));
  }
  //dfs
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      colors[i][j] = WHITE;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (colors[i][j] == WHITE) {
        dfs_visit(G, i, j, colors, n);
      }
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", G[i][j]);
    }
  }
}

int random(int n, int m) {
  return n + rand() % (m - n);
}

void random_directed_graph(int **G, int n, double b) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      G[i][j] = 0;
    }
  }
  double m = b * (n * n);
  int u, v;
  for (int i = 0; i < m; i++) {
    do {
      u = random(0, n);
      v = random(0, n);
    } while (u == v || G[u][v] == 1);
    G[u][v] = 1;
  }
}

//char *passing_name[] = { "dfs_matrix", "dfs_adjacent", "bfs_matrix", "bfs_adjacent" };

int main () {
  for (unsigned int i = 0; i < sizeof(ns) / sizeof(*ns); i++) {
    int n = ns[i];

    for (unsigned int j = 0; j < sizeof(ds) / sizeof(*ds); j++) {
      double b = ds[j] / (double)8;
      clock_t passing_time = 0;

      //array initialization
      int **G = calloc(n, sizeof(int *));
      for (int ii = 0; ii < n; ii++) {
        G[ii] = calloc(n, sizeof(int));
      }
      random_directed_graph(G, n, b);

      dfs(G, n);

      //array clear
      for (int ii = 0; ii < n; ii++) {
        int* currPtr = G[ii];
        free(currPtr);
      }
    }
  }
  return 0;
}