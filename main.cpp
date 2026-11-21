#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using Clock = chrono::steady_clock;

struct Stats {
    unsigned long long comparisons = 0;
    unsigned long long swaps = 0;
};

void selectionSort(vector<int>& a, Stats& s) {
    for (size_t i = 0; i < a.size(); ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < a.size(); ++j) {
            ++s.comparisons;
            if (a[j] < a[minIndex]) minIndex = j;
        }
        if (minIndex != i) {
            swap(a[i], a[minIndex]);
            ++s.swaps;
        }
    }
}

void bubbleSort(vector<int>& a, Stats& s) {
    for (size_t i = 0; i < a.size(); ++i) {
        bool changed = false;
        for (size_t j = 0; j + 1 < a.size() - i; ++j) {
            ++s.comparisons;
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                ++s.swaps;
                changed = true;
            }
        }
        if (!changed) break;
    }
}

int partitionArray(vector<int>& a, int low, int high, Stats& s) {
    int pivot = a[high]; // Last-element pivot; behavior depends on input order.
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        ++s.comparisons;
        if (a[j] <= pivot) {
            ++i;
            if (i != j) {
                swap(a[i], a[j]);
                ++s.swaps;
            }
        }
    }
    if (i + 1 != high) {
        swap(a[i + 1], a[high]);
        ++s.swaps;
    }
    return i + 1;
}

void quickSort(vector<int>& a, int low, int high, Stats& s) {
    if (low < high) {
        int pivotIndex = partitionArray(a, low, high, s);
        quickSort(a, low, pivotIndex - 1, s);
        quickSort(a, pivotIndex + 1, high, s);
    }
}

void quickSortWrapper(vector<int>& a, Stats& s) {
    if (!a.empty()) quickSort(a, 0, static_cast<int>(a.size()) - 1, s);
}

void mergeParts(vector<int>& a, vector<int>& temp, int left, int mid, int right, Stats& s) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        ++s.comparisons;
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    while (i <= mid) temp[k++] = a[i++];
    while (j <= right) temp[k++] = a[j++];

    for (int p = left; p <= right; ++p) a[p] = temp[p];
}

void mergeSortRecursive(vector<int>& a, vector<int>& temp,
                        int left, int right, Stats& s) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortRecursive(a, temp, left, mid, s);
    mergeSortRecursive(a, temp, mid + 1, right, s);
    mergeParts(a, temp, left, mid, right, s);
}

void mergeSort(vector<int>& a, Stats& s) {
    if (a.empty()) return;
    vector<int> temp(a.size());
    mergeSortRecursive(a, temp, 0, static_cast<int>(a.size()) - 1, s);
}

struct Algorithm {
    string name;
    void (*sortFunction)(vector<int>&, Stats&);
};

int main() {
    const int MEASURED_RUNS = 3;
    const int REPETITIONS_PER_RUN = 10000;

    const vector<Algorithm> algorithms = {
        {"Selection Sort", selectionSort},
        {"Bubble Sort", bubbleSort},
        {"Quick Sort", quickSortWrapper},
        {"Merge Sort", mergeSort}
    };

    ofstream csv("assignment3_results.csv");
    if (!csv) {
        cerr << "Could not create assignment3_results.csv\n";
        return 1;
    }

    csv << "Algorithm,InputCase,N,Run,AverageTime_us,Comparisons,Swaps\n";

    const vector<int> sizes = {5, 100};
    for (int n : sizes) {
        vector<int> sorted(n);
        for (int i = 0; i < n; ++i) sorted[i] = i + 1;

        vector<int> reverseSorted = sorted;
        reverse(reverseSorted.begin(), reverseSorted.end());

        vector<pair<string, vector<int>>> cases = {
            {"Sorted", sorted},
            {"Reverse_Sorted", reverseSorted}
        };

        for (const auto& testCase : cases) {
            for (const auto& algorithm : algorithms) {
                for (int run = 1; run <= MEASURED_RUNS; ++run) {
                    double totalMicroseconds = 0.0;
                    Stats lastStats;

                    for (int rep = 0; rep < REPETITIONS_PER_RUN; ++rep) {
                        vector<int> data = testCase.second; // Copy excluded from timing.
                        Stats currentStats;

                        auto start = Clock::now();
                        algorithm.sortFunction(data, currentStats);
                        auto stop = Clock::now();

                        totalMicroseconds +=
                            chrono::duration<double, micro>(stop - start).count();

                        if (!is_sorted(data.begin(), data.end())) {
                            cerr << "Verification failed: " << algorithm.name << '\n';
                            return 1;
                        }
                        lastStats = currentStats;
                    }

                    double averageTime = totalMicroseconds / REPETITIONS_PER_RUN;
                    csv << '"' << algorithm.name << '"' << ','
                        << testCase.first << ','
                        << n << ','
                        << run << ','
                        << setprecision(8) << averageTime << ','
                        << lastStats.comparisons << ','
                        << lastStats.swaps << '\n';

                    cout << algorithm.name << " | " << testCase.first
                         << " | N=" << n << " | Run " << run
                         << " | " << fixed << setprecision(4)
                         << averageTime << " us\n";
                }
            }
        }
    }

    csv.close();
    cout << "\nAll 16 cases (4 algorithms x 4 inputs) completed.\n";
    cout << "Results saved in assignment3_results.csv\n";
    return 0;
}
