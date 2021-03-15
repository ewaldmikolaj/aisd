#include <iostream>
#include <algorithm>

void bubble_sort(int arr[], int n) {
  for (int i = 0; i < n; i++) {
     for (int j = n - 1; j > i; j--) {
       if (arr[j] < arr[j - 1]) {
         std::swap(arr[j], arr[j - 1]);
       }
     }
  }
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << std::endl;
  }
}

int main() {
  int arr[] = {4, 2, 0, 1, 3};
  int n = sizeof(arr) / sizeof(arr[0]);
  bubble_sort(arr, n);
  return 0;
}