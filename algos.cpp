#include "algos.h"

int get_random(int p, int r) {
    return p + rand() % (r - p + 1);
}

void merge(std::vector<int> &a, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;

    std::vector<int> left(n1 + 1), right(n2 + 1);
    for(int i = 0; i < n1; i++) {
        left[i] = a[i + p];
    }
    for(int i = 0; i < n2; i++) {
        right[i] = a[i + q + 1];
    }
    
    left[n1] = INT_MAX;
    right[n2] = INT_MAX;

    int i = 0, j = 0;

    for(int k = p; k <= r; k++) {
        if(left[i] <= right[j]) {
            a[k] = left[i];
            i++;
        } else {
            a[k] = right[j];
            j++;
        }
    }
}

int partition(std::vector<int> &a, int p, int r) {
    int x = a[r];
    int i = p - 1;
    for(int j = p; j < r; j++) {
        if(a[j] <= x) {
            i++;
            std::swap(a[i],a[j]);
        }
    }
    std::swap(a[i + 1], a[r]);
    return i + 1;
}

int random_partition(std::vector<int> &a, int p, int r) {
    int i = get_random(p, r);
    std::swap(a[i], a[r]);
    return partition(a, p, r);
}

void random_quick_sort(std::vector<int> &a, int p, int r) {
    if(p < r) {
        int q = random_partition(a, p, r);
        random_quick_sort(a, p, q - 1);
        random_quick_sort(a, q + 1, r);
    }
}

// Part 1

void merge_sort(std::vector<int> &a, int left, int right) {
    if(left >= right) {
        return;
    }
    if(left < right) {
        int mid = (left + right) / 2;
        merge_sort(a, left, mid);
        merge_sort(a, mid + 1, right);
        merge(a, left, mid, right);
    }
}

void heap_sort(std::vector<int> &a){
    heap max_heap(a);
    for(int i = (int)max_heap.tree.size() - 1; i >= 1; i--) {
        std::swap(max_heap.tree[0], max_heap.tree[i]);
        max_heap.heapSize--;
        max_heap.heapify(0);
    }
}

void bubble_sort(std::vector<int> &a) {
    int good_pairs = 0;
    while(good_pairs < (int)a.size() - 1){
        good_pairs = 0;
        for(int i = 0; i < (int)a.size() - 1; i++) {
            if(a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
            } else {
                good_pairs++;
            }
        }
    }
}

void insertion_sort(std::vector<int> &a, int left, int right) {
    for(int j = left + 1; j <= right; j++) {
        int key = a[j];
        int i = j - 1;
        while(i >= left && a[i] > key) {
            a[i + 1] = a[i];
            i--;
        }
        a[i + 1] = key;
    }
}

void selection_sort(std::vector<int> &a) {
    for(int j = (int)a.size() - 1; j >= 0 ; j--) {
        int max_elem = a[j];
        int inx = j;
        for(int i = j; i >= 0; i--) {
            if(a[i] > max_elem) {
                max_elem = a[i];
                inx = i;
            }
        }
        std::swap(a[j], a[inx]);
    }
}

void quick_sort(std::vector<int> &a, int p, int r) {
    if(p < r) {
        int q = partition(a, p, r);
        quick_sort(a, p, q-1);
        quick_sort(a, q + 1, r);
    }
}

// Part 2

std::vector<int> library_sort(std::vector<int>& a) {
    if (a.empty()) return {};
    
    const int size = a.size();
    const double eps = 1.0; 
    const int EMPTY = INT_MIN;
    
    int total_slots = std::ceil((1 + eps) * size);
    std::vector<int> S(total_slots, EMPTY);
    
    S[0] = a[0];
    int filled = 1; 
    
    for (int i = 1; i < size; i++) {
        int current = a[i];
        int pos = 0;
        std::vector<int> temp;
        for (int j = 0; j < total_slots; j++) {
            if (S[j] != EMPTY) {
                temp.push_back(S[j]);
            }
        }

        int left = 0, right = temp.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (temp[mid] < current) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        pos = left;
    
        if (filled >= total_slots / (1 + eps/2)) {
            std::vector<int> all = temp;
            all.insert(all.begin() + pos, current);
            
            total_slots = std::ceil((1 + eps) * size);
            S.assign(total_slots, EMPTY);
            
            for (size_t j = 0; j < all.size(); j++) {
                int position = (j * total_slots) / all.size();
                S[position] = all[j];
            }
            
            filled = all.size();
        } else {
            int real_pos = 0;
            int count = 0;
            for (int j = 0; j < total_slots; j++) {
                if (S[j] != EMPTY) {
                    if (count == pos) {
                        real_pos = j;
                        break;
                    }
                    count++;
                }
            }
            
            if (count < pos) {
                real_pos = total_slots;
                for (int j = total_slots - 1; j >= 0; j--) {
                    if (S[j] != EMPTY) {
                        real_pos = j + 1;
                        break;
                    }
                }
            }
            
            int free_slot = real_pos;
            while (free_slot < total_slots && S[free_slot] != EMPTY) {
                free_slot++;
            }
            
            if (free_slot >= total_slots) {
                free_slot = total_slots - 1;
            }
            
            for (int j = free_slot; j > real_pos; j--) {
                S[j] = S[j-1];
            }
            
            S[real_pos] = current;
            filled++;
        }
    }
    
    std::vector<int> result;
    for (int num : S) {
        if (num != EMPTY) {
            result.push_back(num);
        }
    }
    
    return result;
}


void tim_sort(std::vector<int> &a) {
    int n = a.size();
    int run = 32;

    for (int i = 0; i < n; i += run) {
        insertion_sort(a, i, std::min(i + run - 1, n - 1));
    }

    for (int s = run; s < n; s = 2 * s) {
        for (int l = 0; l < n; l += 2 * s) {
            int mid = l + s - 1;
            int r = std::min((l + 2 * s - 1), (n - 1));
            if (mid < r)
                merge(a, l, mid, r);
        }
    }
}

void cocktail_shaker_sort(std::vector<int> &a) {
    bool flag = true;
    int left = 0, right = a.size() - 1;

    while(flag) {
        flag = false;
        for(int i = left; i < right; i++) {
            if(a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
                flag = true;
            }
        }
        if(flag == false) break;
        flag = false; right--;
        for(int i = right - 1; i >= left; i--) {
            if(a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]); 
                flag = true;
            }
        }
        left++;
    }
}

void comb_sort(std::vector<int> &a) {
    double k = 1.3;
    int gap = a.size();
    bool flag = true;

    while (gap != 1 || flag) {
        gap = std::max((int)(gap/k), 1);
        flag = false;

        for (int i = 0; i < (int)a.size() - gap; ++i) {
            if (a[i] > a[i + gap]) {
                std::swap(a[i], a[i + gap]);
                flag = true;
            }
        }
    }
}

void tournament_sort(std::vector<int> &a) {
    tour_tree tr(a);
    for(int i = 0; i < (int)a.size(); i++) {
        a[i] = tr.min();
        tr.pop_min();
    }
}

void heapify_intro(std::vector<int>& a, int left, int n, int i) {
    int largest = i;
    int l = 2 * (i - left) + 1 + left;
    int r = 2 * (i - left) + 2 + left;

    if (l < n && a[l] > a[largest])
        largest = l;

    if (r < n && a[r] > a[largest])
        largest = r;

    if (largest != i) {
        std::swap(a[i], a[largest]);
        heapify_intro(a, left, n, largest);
    }
}

void heap_intro(std::vector<int>& a, int left, int right) {
    int n = right - left;

    for (int i = left + n / 2 - 1; i >= left; i--)
        heapify_intro(a, left, right, i);

    for (int i = right - 1; i > left; i--) {
        std::swap(a[left], a[i]);
        heapify_intro(a, left, i, left);
    }
}

void intro_help(std::vector<int>& a, int left, int right, int depth_limit) {
    if (right - left < 16) {
        insertion_sort(a, left, right - 1);
        return;
    }
    
    if (depth_limit == 0) {
        heap_intro(a, left, right);
        return;
    }
    
    int pivot = partition(a, left, right - 1);
    intro_help(a, left, pivot, depth_limit - 1);
    intro_help(a, pivot + 1, right, depth_limit - 1);
}

void intro_sort(std::vector<int>& a) {
    int depth = 2 * log2((int)a.size());
    intro_help(a, 0, (int)a.size(), depth);
}