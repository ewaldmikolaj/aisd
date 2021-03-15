#include <iostream>
#include <algorithm>

//sortowania przez wybieranie działa tak, że:
// program iteruje od zera
// szuka najmniejszego elementu
// zamienia go z obecnym elementem, a następnie przechodzi na następny element
// cykl się powtarza
// O(n^2)

int argmin(int arr[], int n, int m) {
  int min_argument = n;
  for (int i = n + 1; i < m; i++) {
    if (arr[i] < arr[min_argument]) {
      min_argument = i;
    }
  }
  return min_argument;
}

int main () {
  int arr[] = {4, 2, 0, 1, 3};
  int length = sizeof(arr)/sizeof(arr[0]);
  
  int j;
  for (int i = 0; i < length; i++) {
    j = argmin(arr, i, length);
    std::swap(arr[i], arr[j]);
  }
  for (int i = 0; i < length; i++) {
    std::cout << arr[i] << std::endl;
  }
  return 0;
}