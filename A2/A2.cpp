#include <iostream>
#include <vector>
#include <iomanip>

const int INSERTION_LIMIT = 15;

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

void merge(std::vector<long long> &a, std::vector<long long> &tmp,
           int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else             tmp[k++] = a[j++];
    }
    while (i <= mid)  tmp[k++] = a[i++];
    while (j <= right) tmp[k++] = a[j++];

    for (int t = left; t <= right; ++t) a[t] = tmp[t];
}

void merge_insertion_sort(std::vector<long long> &a, std::vector<long long> &tmp,
                          int left, int right) {
    int len = right - left + 1;
    if (len <= 1) return;

    if (len <= INSERTION_LIMIT) {
        insertion_sort(a, left, right);
        return;
    }

    int mid = left + (right - left) / 2;
    merge_insertion_sort(a, tmp, left, mid);
    merge_insertion_sort(a, tmp, mid + 1, right);
    merge(a, tmp, left, mid, right);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    if (n > 0) {
        std::vector<long long> tmp(n);
        merge_insertion_sort(a, tmp, 0, n - 1);
    }

    for (int i = 0; i < n; ++i) {
        if (i) std::cout << ' ';
        std::cout << a[i];
    }
    std::cout << '\n';

    return 0;
}
