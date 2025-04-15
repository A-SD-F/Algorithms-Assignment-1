#include <iostream>
#include <iomanip>
#include <fstream>

#include <climits>
#include <cmath>
#include <random>

#include <vector>
#include <string>

#include <functional>
#include <chrono>

int get_random(int p, int r);

class heap {
public:
    int heapSize = 0;
    std::vector<int> &tree;

    heap(std::vector<int> &a) : tree(a) {
        heapSize = a.size();
        for(int i = heapSize / 2 - 1; i >= 0; --i) {
            heapify(i);
        }
    }

    void heapify(int i) {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if(l < heapSize && tree[l] > tree[largest]) largest = l;
        if(r < heapSize && tree[r] > tree[largest]) largest = r;

        if(largest != i) {
            std::swap(tree[i], tree[largest]);
            heapify(largest);
        }
    }
};

void merge(std::vector<int> &a, int p, int q, int r);

int partition(std::vector<int> &a, int p, int r);
int random_partition(std::vector<int> &a, int p, int r);
void random_quick_sort(std::vector<int> &a, int p, int r);

void merge_sort(std::vector<int> &a, int left, int right);
void heap_sort(std::vector<int> &a);
void bubble_sort(std::vector<int> &a);
void insertion_sort(std::vector<int> &a, int, int);
void selection_sort(std::vector<int> &a);
void quick_sort(std::vector<int> &a, int p, int r);

class tour_tree {
public:
    std::vector<int> tree;
    int num;
    int leaf;

    tour_tree(std::vector<int> &a) {
        num = a.size();
        int N = 1;
        while(N < num) N *= 2;
        leaf = N;
        tree.resize(2 * N, INT_MAX);

        for(int i = 0; i < num; i++) {
            tree[leaf + i] = a[i];
        }

        for(int i = leaf - 1; i > 0; i--) {
            tree[i] = std::min(tree[2 * i], tree[2 * i + 1]);   
        }
    }

    int min() const {
        return tree[1];
    }

    void pop_min() {
        if(num == 0) return;
    
        int inx = 1;
        while(inx < leaf) {
            int l = 2 * inx, r = 2 * inx + 1;
            if(tree[inx] == tree[l]) {
                inx = l;
            } else {
                inx = r;
            }
        }
        tree[inx] = INT_MAX;
        inx = inx / 2;
        while(inx > 0) {
            tree[inx] = std::min(tree[2 * inx], tree[2 * inx + 1]);
            inx = inx / 2;
        }
        num--;
    }
    bool empty() {
        return num == 0;
    }

};

std::vector<int> library_sort(std::vector<int> &a);
void tim_sort(std::vector<int> &a);
void cocktail_shaker_sort(std::vector<int> &a); 
void comb_sort(std::vector<int> &a);
void tournament_sort(std::vector<int> &a);
void intro_sort(std::vector<int> &a);