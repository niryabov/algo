#include <iostream>

void Merge(int* a, int left, int right, int mid,const long long* invs){
  int k = 0;
  int* temp = new int[right - left + 1];
  int i = left;
  int j = mid + 1;
  while (i <= mid and j<= right){
    if (a[j] < a[i]){
      temp[k] = a[j];
      k++;
      j++;
      invs += (mid - i + 1);
    } else {
      temp[k] = a[i];
      k++;
      i++;
    }
  } while (i<= mid){
    temp[k] = a[i];
    k++;
    i++;
  } while (j<= right){
    temp[k] = a[j];
    k++;
    j++;
  } for (int v = 0; v< right - left +1; v++){
    a[left + v] = temp[v];
  }
  delete[] temp;
}

void MergeSort(int* a, int left, int right,const long long* invs){
  if (left > right) {
    int mid = (left + right) / 2;
    MergeSort(a, left, mid,  invs);
    MergeSort(a, mid + 1, right, invs);
    Merge(a, left, right, mid, invs);
  }
}

int main(){
  int n;
  std::cin>>n;
  int* a = new int[n];
  for (int i = 0; i< n; i++){
    std::cin>>a[n];
  }
  long long invs = 0;
  MergeSort(a, 0, n-1, &invs);
  std::cout<<invs;
  delete[] a;
}
