#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define REPETITIONS 20

int dynamic_value = 0, greed_value = 0;

//help functions

int random(int n, int m) {
  return n + rand() % (m - n);
}

void swap(int *A, int i, int j) {
  double tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}

int get_weight(int *w, int n) {
  int weight = 0;
  for (unsigned int i = 0; i < n; i++) {
    weight += w[i];
  }
  return weight;
}

int max(int a, int b) {
  return a > b ? a : b;
}

//dynamic programming algorithm

void read_solution(int **M, int *v, int *w, int n, int W) {
  int j = W, i = n, solution = 0, weight = 0;
  while (i > 0) {
    if (M[i][j] == M[i - 1][j]) {
      i--;
    } else {
      solution += v[i - 1];
      weight += w[i - 1];
      j = j - w[i - 1];
      i--;
    }
  }
  dynamic_value = solution;
}

void dynamic_programming(int *v, int *w, int n, int W) {
  int **M = calloc(n + 1, sizeof(int*));
  for (unsigned int ii = 0; ii < n + 1; ii++) {
    M[ii] = calloc(W + 1, sizeof(int));
  }
  for (unsigned int i = 0; i < n + 1; i++) {
    for (unsigned int j = 0; j < W + 1; j++) {
      M[i][j] = 0;
    }
  }
  for (unsigned int i = 0; i < n + 1; i++) {
    for (unsigned int j = 0; j < W + 1; j++) {
      if (i == 0 || j == 0) {
        M[i][j] = 0;
      } else if (w[i - 1] > j) {
        M[i][j] = M[i - 1][j];
      } else if (w[i - 1] <= j) {
        M[i][j] = max(M[i - 1][j], v[i - 1] + M[i - 1][j - w[i - 1]]);
      }
    }
  }
  read_solution(M, v, w, n, W);
  for (unsigned int i = 0; i < n; i++) {
    free(M[i]);
  }
  free(M);
}

//greedy algorithm

void insertion_sort(double *A, int *v, int *w, int n) {
  int i;
  double key;
  int value, weight;
  for (int j = 1; j < n; j++) {
    key = A[j];
    value = v[j];
    weight = w[j];
    i = j - 1;
    while (i >= 0 && A[i] > key) {
      A[i + 1] = A[i];
      v[i + 1] = v[i];
      w[i + 1] = w[i];
      i = i - 1;
    }
    A[i + 1] = key;
    v[i + 1] = value;
    w[i + 1] = weight;
  }
}

void greed_algorithm(int *v, int *w, int n, int W) {
  double *profit = calloc(n, sizeof(double));
  for (unsigned int i = 0; i < n; i++) {
    profit[i] = (double)v[i] / (double)w[i];
  }
  insertion_sort(profit, v, w, n);
  for (unsigned int i = 0; i < n / 2; i++) {
    swap(v, i, n - 1 - i);
    swap(w, i, n - 1 - i);
  }
  int weight = 0, solution = 0;
  for (unsigned int i = 0; i < n; i++) {
    if (weight + w[i] <= W) {
      solution += v[i];
      weight += w[i];
    }
  }
  greed_value = solution;
  free(profit);
}

//greedy algorithm v2

void insertion_sort2(int *A, int *B, int n) {
  int i, key, temp;
  for (int j = 1; j < n; j++) {
    key = A[j];
    temp = B[j];
    i = j - 1;
    while (i >= 0 && A[i] > key) {
      A[i + 1] = A[i];
      B[i + 1] = B[i];
      i = i - 1;
    }
    A[i + 1] = key;
    B[i + 1] = temp;
  }
}

void greed_algorithm2(int *v, int *w, int n, int W) {
  insertion_sort2(v, w, n);
  int weight = 0, solution = 0;
  for (unsigned int i = 0; i < n / 2; i++) {
    swap(v, i, n - 1 - i);
    swap(w, i, n - 1 - i);
  }
  for (unsigned int i = 0; i < n; i++) {
    if (weight + w[i] <= W) {
      solution += v[i];
      weight += w[i];
    }
  }
  greed_value = solution;
}

//greedy algorithm v3

void greed_algorithm3(int *v, int *w, int n, int W) {
  insertion_sort2(w, v, n);
  int weight = 0, solution = 0;
  for (unsigned int i = 0; i < n; i++) {
    if (weight + w[i] <= W) {
      solution += v[i];
      weight += w[i];
    }
  }
  greed_value = solution;
}

//brute force 

int get_items_value(int *v, int n, int number) {
  int value = 0;
  for (unsigned int i = 0; i < n; i++) {
    value += v[i] * (number % 2);
    number = number / 2;
  }
  return value;
}

int get_items_weight(int *w, int n, int number) {
  int weight = 0;
  for (unsigned int i = 0; i < n; i++) {
    weight += w[i] * (number % 2);
    number = number / 2;
  }
  return weight;
}

void brute_force(int *v, int *w, int n, int W) {
  int solution = 0;
  int value, weight;
  for (unsigned int i = 0; i < pow(2, n); i++) {
    value = get_items_value(v, n, i);
    weight = get_items_weight(w, n, i);
    if (value > solution && weight < W) {
      solution = value;
    }
  }
}

//generating instance

void genetator_basic(int *v, int *w, int n, int X) {
  for (unsigned int i = 0; i < n; i++) {
    v[i] = random(0, 9);
    w[i] = X / n;
  }
}

void generator_advanced(int *v, int *w, int n) {
  for (unsigned int i = 0; i < n; i++) {
    v[i] = random(1, 9);
    w[i] = random(1, 9);
  }
}

char *names[] = { "greed_algorithm", "dynamic_programming" };
void (*functions[])(int * v, int *w, int n, int W) = { greed_algorithm, dynamic_programming };

int main() {
  //brute main
  /*for (unsigned int i = 2; i <= 30; i++) {
    for (unsigned int j = 30; j <= 300; j += 30) {

      int *v = calloc(i, sizeof(int));
      int *w = calloc(i, sizeof(int));
      for (unsigned int ii = 0; ii < i; ii++) {
        v[ii] = 0;
        w[ii] = 0;
      }

      genetator_basic(v, w, i, j);
      int W = 0.75 * j;

      clock_t begin = clock();
      brute_force(v, w, i, W);
      clock_t end = clock();
      double seconds = (double)(end - begin) / (double) CLOCKS_PER_SEC;

      printf("%-14s %-6d %-6d %-6g\n", "brute force", i, j, seconds);

      free(v);
      free(w);

    }
  }*/

  //greed and dynamic main
  /*for (unsigned int f = 0; f < sizeof(functions)/sizeof(*functions); f++) {
    void (*find_solution)(int *v, int *w, int n, int W) = functions[f];
    for (unsigned int i = 100; i <= 1000; i += 100) {
      for (unsigned int j = 1000; j <= 10000; j += 1000) {
        
        int *v = calloc(i, sizeof(int));
        int *w = calloc(i, sizeof(int));
        for (unsigned int ii = 0; ii < i; ii++) {
          v[ii] = 0;
          w[ii] = 0;
        }

        genetator_basic(v, w, i, j);
        int W = 0.75 * j;

        clock_t begin = clock();
        for (unsigned int k = 0; k < REPETITIONS; k++) {
          find_solution(v, w, i, W);
        }
        clock_t end = clock();
        double seconds = (double)(end - begin) / (double) CLOCKS_PER_SEC;

        printf("%-20s %-6d %-6d %-6g\n", names[f], i, j, seconds);

        free(v);
        free(w);

      }

    }
  }*/

  //greed and dynamic extended main
  int n = 10000;
  double g1, g2, g3;
  for (unsigned int i = 0; i < 100; i++) {

    int *v = calloc(n, sizeof(int));
    int *w = calloc(n, sizeof(int));
    for (unsigned int ii = 0; ii < n; ii++) {
      v[ii] = 0;
      w[ii] = 0;
    }

    generator_advanced(v, w, n);
    int W = 0.75 * get_weight(w, n);

    dynamic_programming(v, w, n, W);

    greed_algorithm(v, w, n, W);
    g1 = (double)greed_value / (double)dynamic_value;

    greed_algorithm2(v, w, n, W);
    g2 = (double)greed_value / (double)dynamic_value;

    greed_algorithm3(v, w, n, W);
    g3 = (double)greed_value / (double)dynamic_value;

    printf("%-10d %-10.4g %-10.4g %-10.4g\n", i + 1, g1, g2, g3);
  }
  return 0;
}