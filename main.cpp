#include "algos.h"

void merge_sort_wrap(std::vector<int>& a) {
    merge_sort(a, 0, (int)a.size() - 1);
}

void quick_sort_wrap(std::vector<int>& a) {
    quick_sort(a, 0, (int)a.size() - 1);
}

void insertion_sort_wrap(std::vector<int>& a) {
    insertion_sort(a, 0, (int)a.size() - 1);
}

void library_sort_wrap(std::vector<int>& a) {
    library_sort(a);
}

std::string SortTypeNames[] = {
    "Merge Sort",
    "Heap Sort",
    "Bubble Sort",
    "Insertion Sort",
    "Selection Sort",
    "Quick Sort",
    "Library Sort", 
    "Tim Sort", 
    "Cocktail Shaker Sort", 
    "Comb Sort", 
    "Tournament Sort", 
    "Introsort",
};

std::string DataSets[] = {
    "Sorted (Ascending)",
    "Random",
    "Reverse Sorted",
    "Partially Sorted",
};

void print(std::vector<int> &a) {
    std::cout << ":\t";
    for(int i = 0; i < (int)a.size(); i++) {
        std::cout << a[i] << " ";
    }
    std::cout << "\n";
}

bool check_order(std::vector<int> &a) {
    for(int i = 1; i < (int)a.size(); i++) {
        if(a[i - 1] > a[i]) {
            return false;
        }
    }
    return true;
}

struct ComplexityResult {
    double timeMs;
    size_t spaceBytes;
    bool isCorrect;
};

void showProgress(int current, int total) {
    const int barWidth = 50;
    float progress = (float)current / total;
    int barPos = barWidth * progress;
    
    std::cout << "\r[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < barPos) std::cout << "=";
        else if (i == barPos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << "% ";
    std::cout << "(" << current << "/" << total << ")" << std::flush;
}

size_t estimateAlgorithmSpace(const std::string& algo_name, size_t data_size) {
    if (algo_name.find("Merge") != std::string::npos) {
        return data_size * sizeof(int);
    } 
    else if (algo_name.find("Quick") != std::string::npos) {
        return log2(data_size) * sizeof(int) * 4; 
    }
    else if (algo_name.find("Heap") != std::string::npos || 
             algo_name.find("Selection") != std::string::npos || 
             algo_name.find("Bubble") != std::string::npos) {
        return sizeof(int) * 3; 
    }
    else if (algo_name.find("Tim") != std::string::npos || 
             algo_name.find("Intro") != std::string::npos) {
        return data_size * sizeof(int) / 2; 
    }
    else {
        return data_size * sizeof(int) / 4; 
    }
}

ComplexityResult complexity(std::function<void(std::vector<int>&)> sort_func, 
    std::vector<int> data, 
    bool& is_correct,
    const std::string& algo_name) {
    std::vector<int> test_data = data;
    
    size_t initialCapacity = test_data.capacity() * sizeof(int);
    size_t peakMemory = initialCapacity;
    
    auto start = std::chrono::high_resolution_clock::now();
    sort_func(test_data);
    auto end = std::chrono::high_resolution_clock::now();
    
    size_t finalCapacity = test_data.capacity() * sizeof(int);
    peakMemory = std::max(peakMemory, finalCapacity);
    
    peakMemory += estimateAlgorithmSpace(algo_name, data.size());
    
    std::chrono::duration<double, std::milli> duration = end - start;
    is_correct = check_order(test_data);
    
    return {duration.count(), peakMemory, is_correct};
}

std::vector<int> generate(int size, int t) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, size * 10);

    switch(t) {
        case 0: // Sorted
            for(int i = 0; i < size; i++) data[i] = dist(gen);    
            merge_sort(data, 0, (int)data.size() - 1);
            break;
        case 1: // Random
            for(int i = 0; i < size; i++) data[i] = dist(gen);
            break;
        case 2: // Reverse Sorted
            for(int i = 0; i < size; i++) data[i] = size - i;
            break;
        case 3: // Partially Sorted
            for(int i = 0; i < size; i++) data[i] = dist(gen);
            merge_sort(data, 0, (int)data.size() - 1);
            int change = size*0.37;
            for(int i = 0; i < change; i++) {
                int inx1 = dist(gen) % size;
                int inx2 = dist(gen) % size;
                std::swap(data[inx1], data[inx2]);
            }
            break;
    }
    return data;
}

int main() {
    std::ofstream results("results.csv");
    results << "Algorithm,Dataset,Size,Time (ms),Space (KB),Correct\n";

    std::function<void(std::vector<int>&)> sort_functions[] = {
        merge_sort_wrap,
        heap_sort,
        bubble_sort,
        insertion_sort_wrap,
        selection_sort,
        quick_sort_wrap,
        [](std::vector<int>& a) { 
            std::vector<int> result = library_sort(a);
            a = result;
        },
        tim_sort,
        cocktail_shaker_sort,
        comb_sort,
        tournament_sort,
        intro_sort
    };

    int sizes[] = {1000, 10000, 100000, 1000000};
    const int num_runs = 10;

    int total_tests = 0;
    for (int size_idx = 0; size_idx < (int)(sizeof(sizes)/sizeof(sizes[0])); size_idx++) {
        for (int dataset_type = 0; dataset_type < 4; dataset_type++) {
            for (int algo_idx = 0; algo_idx < 12; algo_idx++) {
                int size = sizes[size_idx];
                if ((algo_idx == 2 || algo_idx == 3 || algo_idx == 4 || algo_idx == 8) && size > 100000) {
                    total_tests += 1;
                } else {
                    total_tests += num_runs;
                }
            }
        }
    }
    
    std::cout << "Starting benchmark with " << total_tests << " total test runs...\n";
    int completed_tests = 0;
    showProgress(0, total_tests);

    for (int size_idx = 0; size_idx < (int)(sizeof(sizes)/sizeof(sizes[0])); size_idx++) {
        int size = sizes[size_idx];
        
        for (int dataset_type = 0; dataset_type < 4; dataset_type++) {
            for (int algo_idx = 0; algo_idx < 12; algo_idx++) {
                double total_time = 0;
                size_t total_space = 0;
                bool all_correct = true;
                
                std::string algo_name = SortTypeNames[algo_idx];
                
                if ((algo_idx == 2 || algo_idx == 3 || algo_idx == 4 || algo_idx == 8) && size > 100000) {
                    results << algo_name << ","
                           << DataSets[dataset_type] << ","
                           << size << ","
                           << "Skipped" << ","
                           << "Skipped" << ","
                           << "N/A" << "\n";
                    completed_tests++;
                    showProgress(completed_tests, total_tests);
                    continue;
                }
                
                for (int run = 0; run < num_runs; run++) {
                    std::vector<int> data = generate(size, dataset_type);
                    bool is_correct;
                    
                    ComplexityResult result = complexity(sort_functions[algo_idx], data, is_correct, algo_name);
                    total_time += result.timeMs;
                    total_space += result.spaceBytes;
                    if (!result.isCorrect) all_correct = false;
                    
                    completed_tests++;
                    showProgress(completed_tests, total_tests);
                }
                
                double avg_time = total_time / num_runs;
                double avg_space_kb = (total_space / num_runs) / 1024.0;
                
                results << algo_name << ","
                       << DataSets[dataset_type] << ","
                       << size << ","
                       << std::fixed << std::setprecision(3) << avg_time << ","
                       << std::fixed << std::setprecision(3) << avg_space_kb << ","
                       << (all_correct ? "Yes" : "No") << "\n";
            }
        }
    }
    
    results.close();
    std::cout << "\nBenchmarking complete" << std::endl;
    
    return 0;
}