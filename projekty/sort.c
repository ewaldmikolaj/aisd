#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int ns[] = { 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000 };

void fill_random(int *A, int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % n;
    }
}

void fill_increasing(int *A, int n) {
    for (int i = 0; i < n; i++) {
      A[i] = i;
    }
}

void fill_decreasing(int *A, int n) {
    for (int i = n; i >= 0; i--) {
      A[n-i] = i;
    }
}

void fill_vshape(int *A, int n) {
    for (int i = n; i >= n/2; i--) {
      A[n-i] = i;
    }
    for (int i = n / 2 + 1; i < n; i++) {
      A[i] = i;
    }
}

int argmin(int *A, int n, int m) {
  int min_argument = n;
  for (int i = n + 1; i < m; i++) {
    if (A[i] < A[min_argument]) {
      min_argument = i;
    }
  }
  return min_argument;
}

void swap(int *A, int i, int j) {
  int tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}

int random(int n, int m) {
    return n + rand() % (m - n);
}

//########################################################################################

// działanie: zamiana obecnego elementu z najmniejszym elementem
void selection_sort(int *A, int n) {
  int j;
  for (int i = 0; i < n; i++) {
    j = argmin(A, i, n);
    swap(A, i, j);
  }
}

//#######################################################################################

// działanie: szukamy odpowiedniego miejsca dla elementu poprzez zamiane z poprzednimi dopóki nie będzie w odpowiednim miejscu
void insertion_sort(int *A, int n) {
  int i, key;
  for (int j = 1; j < n; j++) {
    key = A[j];
    i = j - 1;
    while (i >= 0 && A[i] > key) {
      A[i + 1] = A[i];
      i = i - 1;
    }
    A[i + 1] = key;
  }
}

//######################################################################################

int partition(int *A, int n, int m) {
    int pivot = A[m];
    int i = n - 1;
    for (int j = n; j < m; j++) {
        if(A[j] <= pivot) {
            i++;
            swap(A, i, j);
        }
    }
    i++;
    swap(A, i, m);
    return i;
}

//bez tego nie działa: gcc -O2 -Wall "-Wl,--stack=268435456" sort.c -o sort.exe
void quick_sort(int *A, int n, int m) {
    int q;
    if (n < m) {
        q = partition(A, n, m);
        quick_sort(A, n, q - 1);
        quick_sort(A, q + 1, m);
    }
}

void quick_sort_all(int *A, int n) {
    quick_sort(A, 0, n - 1);
}

//######################################################################################

int randomized_partition(int *A, int n, int m) {
    int i = random(n, m);
    swap(A, i, m);
    return partition(A, n, m);
}

void randomized_quick_sort(int *A, int n, int m) {
    int q;
    if (n < m) {
        q = randomized_partition(A, n, m);
        randomized_quick_sort(A, n, q - 1);
        randomized_quick_sort(A, q + 1, m);
    }
}

void randomized_quick_sort_all(int *A, int n) {
    randomized_quick_sort(A, 0, n - 1);
}

//######################################################################################

int left(int i)
{
    return (2 * i + 1);
}
//Branie prawego i-tego node'a
int right(int i)
{
    return (2 * i + 2);
}

//Dla i-tego node'a
void maxHeapIfy(int *A, int i, int size)
{
    int l = left(i),  //Lewa strona i-tego node-a
        r = right(i), //Prawa strona i-tego node'a
        largest = i;  //Korzen i-tego node'a
    if (l < size && A[l] > A[largest])
    {
        largest = l; //Jesli lewa strona jest wieksza od korzenia to podmieniamy
    }
    if (r < size && A[r] > A[largest])
    {
        largest = r; //Jesli prawa strona jest wieksza od korzenia to podmieniamy
    }
    if (largest != i)
    {
        swap(A, i, largest);          //Jesli podmienilismy korzen z lewym czy prawym swapujemy je
        maxHeapIfy(A, largest, size); //Rekurencyjnie schodzimy nizej
    }
}

void buildMaxHeap(int *A, int n)
{
    int i;
    for (i = n / 2 - 1; i >= 0; i--)
    {
        maxHeapIfy(A, i, n);
    }
}

void heap_sort(int *A, int n)
{
    buildMaxHeap(A, n);
    int i;
    for (i = n - 1; i > 0; i--)
    {
        swap(A, 0, i);
        maxHeapIfy(A, 0, i);
    }
}

bool is_random(int *A, int n) {
    return true;
}

bool is_increasing(int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i] <= A[i - 1]) {
            return false;
        }
    }
    return true;
}

bool is_decreasing(int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i] >= A[i - 1]) {
            return false;
        }
    }
    return true;
}

bool is_vshape(int *A, int n) {
    if (n % 2 == 0) {
        return is_decreasing(A, n/2) && is_increasing(A + n/2, n/2);
    }
    return is_decreasing(A, n/2 + 1) && is_increasing(A + n/2, n/2 + 1);
}

bool is_sorted(int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1]) {
            return false;
        }
    }
    return true;
}

char *bool_to_string(bool b) {
    return b ? "Y" : "N";
}

void (*fill_functions[])(int *, int) = { fill_random, fill_increasing, fill_decreasing, fill_vshape };
bool (*check_functions[])(int *, int) = { is_random, is_increasing, is_decreasing, is_vshape };
void (*sort_functions[])(int *, int) = { selection_sort, insertion_sort, quick_sort_all, randomized_quick_sort_all, heap_sort };

char *fill_names[] = { "Random", "Increasing", "Decreasing", "V-Shape" };
char *sort_names[] = { "SelectionSort", "InsertionSort", "QuickSort", "RandomizedQuickSort", "HeapSort" };

int main() {
    for (unsigned int i = 0; i < sizeof(sort_functions) / sizeof(*sort_functions); i++) {
        void (*sort)(int *, int) = sort_functions[i];

        for (unsigned int j = 0; j < sizeof(fill_functions) / sizeof(*fill_functions); j++) {
            void (*fill)(int *, int) = fill_functions[j];
            bool (*check)(int *, int) = check_functions[j];

            for (unsigned int k = 0; k < sizeof(ns) / sizeof(*ns); k++) {
                int n = ns[k];
                int *A = calloc(n, sizeof(*A));

                fill(A, n);
                bool is_filled_ok = check(A, n);

                clock_t begin = clock();
                sort(A, n);
                clock_t end = clock();
                double seconds = (double)(end - begin) / (double) CLOCKS_PER_SEC;

                bool is_sorted_ok = is_sorted(A, n);

                printf("%-20s %-11s %-10d %-4s %-4s %g\n", sort_names[i], fill_names[j], n, bool_to_string(is_filled_ok), bool_to_string(is_sorted_ok), seconds);

                free(A);
            }
        }
    }
    return 0;
}