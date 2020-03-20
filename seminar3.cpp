#include <iostream>
#include <vector>

template <class T, class Comparator = std::less<T> >
int partition(T* arr, int l, int r, int k, Comparator comp) {
    int pivot_idx = l + (r - l) / 2;
    int pivot = arr[pivot_idx];
    
    std::swap(arr[pivot_idx], arr[r - 1]);
    int i = l;
    int j = r - 1;
    
    while (i < j) {
        for (; comp(arr[i], pivot); ++i); // i указывает на эл-т >= pivot
        while ( !comp(arr[j], pivot) && j >= i ) j--; // j указывает на эл-т < pivot
        
        if (i < j) {
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i], arr[r - 1]);
    return i;
}


template <class T, class Comparator = std::less<T> >
void kth_element(T* arr, int size, int k, Comparator comp = Comparator()) {
    int l = 0;
    int r = size;
    while (l < r) {
        int m = partition(arr, l, r, k, comp);
        if (m < k) {
            l = m + 1;
        } else {
            r = m;
        }
    }
}

int main2(int argc, const char * argv[]) {
    std::vector <int> arr;
    int k = 0;
    int n = 0;
    std::cin >> n >> k;
    for (int i = 0; i < n; ++i) {
        int val = 0;
        std::cin >> val;
        arr.push_back(val);
    }
    kth_element(&arr[0], arr.size(), k);
    std::cout << arr[k] << std::endl; //2
    
    return 0;
}

void count_sort(int* arr, int size, int R) {
    int counters_size = R + 1;
    
    int* counters = new int[counters_size];
    for(int i = 0; i < counters_size; ++i) {
        counters[i] = 0;
    }
    
    for (int i = 0; i < size; ++i) {
        counters[arr[i]]++;
    }
    
    for (int i = 0; i < counters_size - 1; ++i) {
        counters[i + 1] += counters[i];
    }
    
    int* aux = new int[size];
    for (int i = size - 1; i >= 0; --i) {
        aux[ --counters[ arr[i] ] ] = arr[i];
    }
    
    for (int i = 0; i < size; ++i) {
        arr[i] = aux[i];
    }
    
    delete[] aux;
    delete[] counters;
}

int main(int argc, const char * argv[]) {
    std::vector<int> arr = {1,3,5,7,2,4,6,1,2,3};
    count_sort(&arr[0], arr.size(), 30);
    for (int i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    
    
    while (true) {
        int val = 0;
        std::cin >> val;
        if (std::cin.fail()) {
            break;
        }
    }
    
}
