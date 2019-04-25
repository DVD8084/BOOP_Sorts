//
// Created by DVD on 17.04.2019.
//

#include "SortVector.h"

#include <algorithm>
#include <utility>

typedef unsigned int uint;

void SortVector::SelectionSort() {
    ResetPointer();
    uint min;
    for (uint i = 0; i + 1 < GetSize(); i++) {
        min = i;
        for (uint j = i + 1; j < GetSize(); j++) {
            if (Less(j, min)) {
                min = j;
            }
        }
        Swap(i, min);
    }
    ResetPointer();
}

void SortVector::BubbleSort() {
    ResetPointer();
    bool swapOccured;
    for (uint i = 0; i + 1 < GetSize(); i++) {
        swapOccured = false;
        for (uint j = 0; j + i + 1 < GetSize(); j++) {
            if (More(j, j + 1)) {
                Swap(j, j + 1);
                swapOccured = true;
            }
        }
        if (!swapOccured) {
            ResetPointer();
            return;
        }
    }
    ResetPointer();
}

void SortVector::InsertionSort() {
    ResetPointer();
    int key;
    for (uint i = 1; i < GetSize(); i++) {
        key = Read(i);
        uint j = i - 1;
        while (j + 1 > 0 && Read(j) > key) {
            Swap(j, j + 1);
            j--;
        }
    }
    ResetPointer();
}

void SortVector::Merge(uint l, uint m, uint r) {
    uint i, j, k;
    uint n1 = m - l;
    uint n2 = r - m;

    std::vector<int> L(n1), R(n2);

    for (i = 0; i < n1; i++) {
        L[i] = Read(l + i);
    }
    for (j = 0; j < n2; j++) {
        R[j] = Read(m + j);
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            Write(k, L[i]);
            i++;
        } else {
            Write(k, R[j]);
            j++;
        }
        k++;
    }

    while (i < n1) {
        Write(k, L[i]);
        i++;
        k++;
    }

    while (j < n2) {
        Write(k, R[j]);
        j++;
        k++;
    }
}

void SortVector::MergeSort(uint l, uint r) {
    if (l == 0 && r == GetSize()) {
        ResetPointer();
    }
    if (l + 1 < r) {
        uint m = l + (r - l) / 2;

        MergeSort(l, m);
        MergeSort(m, r);

        Merge(l, m, r);
    }
    if (l == 0 && r == GetSize()) {
        ResetPointer();
    }
}

void SortVector::CountingSort() {
    ResetPointer();

    int max = 0;
    int min = 0;

    for (uint i = 0; i < GetSize(); i++) {
        int cur = Read(i);
        if (cur > max) {
            max = cur;
        }
        if (cur < min) {
            min = cur;
        }
    }

    uint range = static_cast<uint>(max - min + 1);

    std::vector<int> count(range), output(GetSize());
    for (uint i = 0; i < GetSize(); i++)
        count[Read(i) - min]++;

    for (uint i = 1; i < count.size(); i++)
        count[i] += count[i - 1];

    for (uint i = 0; i < GetSize(); i++) {
        int cur = Read(i);
        output[count[cur - min] - 1] = cur;
        count[cur - min]--;
    }

    for (uint i = 0; i < GetSize(); i++)
        Write(i, output[i]);

    ResetPointer();
}
