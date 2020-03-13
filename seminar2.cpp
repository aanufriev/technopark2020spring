#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <string>

template <class T, class Comparator = std::less<T> >
class Heap {
public:
    Heap(Comparator comp = Comparator());
    Heap(T* buf, size_t buf_size, Comparator comp = Comparator());
    ~Heap();
    
    Heap(const Heap&) = delete;
    Heap(Heap&&) = delete;
    
    Heap& operator= (const Heap&) = delete;
    Heap& operator= (Heap&&) = delete;
    
    const T& top() const;
    void pop();
    
    void push(const T&);
    void push(T&&);
    
    bool is_empty() const;
    size_t size() const;
    
private:
    void extend();
    void sift_up(size_t idx);
    void sift_down(size_t idx);
    
    void heapify();
    
    T* buffer;
    size_t buffer_size;
    size_t heap_size;
    
    Comparator comp;
};

template <class T>
class DefaultComparator {
public:
    bool operator() (const T& l, const T& r) {
        return l < r;
    }
};

template <class T, class Comparator = DefaultComparator<T>>
void sort(T* arr, size_t left, size_t right, Comparator comp = Comparator()) {
    for (size_t i = left; i < right; ++i) {
        for (size_t j = left; j < right - 1; ++j) {
            if (comp(arr[j + 1], arr[j])) {
                std::swap(arr[j + 1], arr[j]);
            }
        }
    }
}

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    
    bool operator< (const Point& r) const {
        return x < r.x;
    }
};

/*
bool operator< (const Point& l, const Point& r) {
    return l.x < r.x;
}
*/

bool is_less_points(const Point& l, const Point& r) {
    return l.x < r.x;
}

bool is_less(const int& l, const int& r) {
    return l < r;
}

class PointDistComparator {
public:
    PointDistComparator(const Point& p) : point(p) {}
    
    bool operator() (const Point& l, const Point& r) {
        return (pow(point.x - l.x, 2) + pow(point.y - l.y, 2)) <
            (pow(point.x - r.x, 2) + pow(point.y - r.y, 2));
    }
private:
    Point point;
};



int main(int argc, const char * argv[]) {
    const size_t buf_size = 100;
    
    int* arr = new int[buf_size];
    std::srand(0);
    for (int i = 0; i < buf_size; ++i) {
        arr[i] = std::rand() % 1000;
    }
    
    sort(arr, 0, buf_size);
    sort(arr, 0, buf_size, is_less);
    sort(arr, 0, buf_size,
         [](const int& l, const int& r) {
             return l < r;
         }
    );
    
    for (int i = 0; i < buf_size; ++i) {
        std::cout << arr[i] << std::endl;
    }
    delete[] arr;
    
    //--------------------
    Point* arr_points = new Point[buf_size];
    
    for (int i = 0; i < buf_size; ++i) {
        arr_points[i] = Point(std::rand() % 1000, std::rand() % 1000);
    }
    sort(arr_points, 0, buf_size);
    sort(arr_points, 0, buf_size, is_less_points);
    sort(arr_points, 0, buf_size, PointDistComparator(Point(0,0)));
    for (int i = 0; i < buf_size; ++i) {
        std::cout << arr_points[i].x << " " << arr_points[i].y << std::endl;
    }
    delete[] arr_points;
    
    std::string* arr_str = new std::string[4] {"xxx", "dssdsd", "sdsd", "asas"};
    sort(arr_str, 0, 4);
    for (int i = 0; i < 4; ++i) {
        std::cout << arr_str[i] << std::endl;
    }
    delete[] arr_str;
    
    return 0;
}
