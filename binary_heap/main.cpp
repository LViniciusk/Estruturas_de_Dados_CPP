#include <iostream>

#include "max_heap.h"

using namespace std;

int main() {
    int *v = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    max_heap<int> heap(v, 10);

    heap.print_tree();
    heap.push(11);

    delete[] v;



    return 0;
}