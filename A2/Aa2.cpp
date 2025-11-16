#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>


void insertion_sort(std::vector<long long> &a, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        long long key = a[i];
        int j = i - 1;
        while (j >= left && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

void merge_arrays(std::vector<long long> &a, std::vector<long long> &tmp,
                  int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else             tmp[k++] = a[j++];
    }
    while (i <= mid)   tmp[k++] = a[i++];
    while (j <= right) tmp[k++] = a[j++];

    for (int t = left; t <= right; ++t) a[t] = tmp[t];
}

void merge_sort(std::vector<long long> &a, std::vector<long long> &tmp,
                int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    merge_sort(a, tmp, left, mid);
    merge_sort(a, tmp, mid + 1, right);
    merge_arrays(a, tmp, left, mid, right);
}

void merge_insertion_sort(std::vector<long long> &a, std::vector<long long> &tmp,
                          int left, int right, int threshold) {
    int len = right - left + 1;
    if (len <= threshold) {
        insertion_sort(a, left, right);
        return;
    }

    int mid = left + (right - left) / 2;
    merge_insertion_sort(a, tmp, left, mid, threshold);
    merge_insertion_sort(a, tmp, mid + 1, right, threshold);
    merge_arrays(a, tmp, left, mid, right);
}



class ArrayGenerator {
public:
    ArrayGenerator(int maxSize = 100000, int maxValue = 6000)
        : maxSize(maxSize), maxValue(maxValue), rng(std::random_device{}()) {

        std::uniform_int_distribution<long long> dist(0, maxValue);

        baseRandom.resize(maxSize);
        for (int i = 0; i < maxSize; i++)
            baseRandom[i] = dist(rng);

        baseSorted = baseRandom;
        std::sort(baseSorted.begin(), baseSorted.end());

        baseReversed = baseSorted;
        std::reverse(baseReversed.begin(), baseReversed.end());
    }

    std::vector<long long> getRandomArray(int n) {
        return std::vector<long long>(baseRandom.begin(), baseRandom.begin() + n);
    }

    std::vector<long long> getReversedArray(int n) {
        return std::vector<long long>(baseReversed.begin(), baseReversed.begin() + n);
    }

    std::vector<long long> getAlmostSortedArray(int n, int swaps = 200) {
        std::vector<long long> a(baseSorted.begin(), baseSorted.begin() + n);
        std::uniform_int_distribution<int> dist(0, n - 1);

        for (int s = 0; s < swaps; s++) {
            int i = dist(rng);
            int j = dist(rng);
            std::swap(a[i], a[j]);
        }
        return a;
    }

private:
    int maxSize;
    int maxValue;
    std::mt19937_64 rng;

    std::vector<long long> baseRandom;
    std::vector<long long> baseSorted;
    std::vector<long long> baseReversed;
};


class SortTester {
public:
    SortTester(int repeats, std::ostream &out)
        : repeats(repeats), gen(100000, 6000), out(out) {}

    void runTests(const std::vector<int> &thresholds) {
        out << "size,type,threshold,time_ms\n";

        for (int n = 500; n <= 100000; n += 100) {
            testForSize(n, thresholds);
        }
    }

private:
    int repeats;
    ArrayGenerator gen;
    std::ostream &out;

    long long measureMergeSort(std::vector<long long> a) {
        std::vector<long long> tmp(a.size());
        long long total = 0;

        for (int i = 0; i < repeats; i++) {
            std::vector<long long> copy = a;

            auto start = std::chrono::high_resolution_clock::now();
            merge_sort(copy, tmp, 0, copy.size() - 1);
            auto end = std::chrono::high_resolution_clock::now();

            total += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }
        return total / repeats;
    }

    long long measureHybrid(std::vector<long long> a, int threshold) {
        std::vector<long long> tmp(a.size());
        long long total = 0;

        for (int i = 0; i < repeats; i++) {
            std::vector<long long> copy = a;

            auto start = std::chrono::high_resolution_clock::now();
            merge_insertion_sort(copy, tmp, 0, copy.size() - 1, threshold);
            auto end = std::chrono::high_resolution_clock::now();

            total += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }
        return total / repeats;
    }

    void testForSize(int n, const std::vector<int> &thresholds) {

        {
            std::vector<long long> arr = gen.getRandomArray(n);

            long long t = measureMergeSort(arr);
            out << n << ",random,0," << t / 1000.0 << "\n";

            for (int th : thresholds) {
                long long thTime = measureHybrid(arr, th);
                out << n << ",random," << th << "," << thTime / 1000.0 << "\n";
            }
        }

        {
            std::vector<long long> arr = gen.getReversedArray(n);

            long long t = measureMergeSort(arr);
            out << n << ",reversed,0," << t / 1000.0 << "\n";

            for (int th : thresholds) {
                long long thTime = measureHybrid(arr, th);
                out << n << ",reversed," << th << "," << thTime / 1000.0 << "\n";
            }
        }

        {
            std::vector<long long> arr = gen.getAlmostSortedArray(n);

            long long t = measureMergeSort(arr);
            out << n << ",almost,0," << t / 1000.0 << "\n";

            for (int th : thresholds) {
                long long thTime = measureHybrid(arr, th);
                out << n << ",almost," << th << "," << thTime / 1000.0 << "\n";
            }
        }
    }
};



int main() {
    std::vector<int> thresholds = {5, 10, 20, 30, 50};

    std::ofstream fout("results.csv");
    if (!fout.is_open()) {
        std::cerr << "Error: cannot open results.csv\n";
        return 1;
    }

    SortTester tester(3, fout);
    tester.runTests(thresholds);

    fout.close();
    return 0;
}
