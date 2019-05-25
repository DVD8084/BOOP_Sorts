//
// Created by DVD on 17.04.2019.
//

#include "SortVector.h"

#include <chrono>
#include <cmath>
#include <random>

typedef unsigned int uint;

void SortVector::SelectionSort() {
    ResetIterators();
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
    ResetIterators();
}

void SortVector::BubbleSort() {
    ResetIterators();
    bool swapped;
    for (uint i = 0; i + 1 < GetSize(); i++) {
        swapped = false;
        for (uint j = 0; j + i + 1 < GetSize(); j++) {
            if (More(j, j + 1)) {
                Swap(j, j + 1);
                swapped = true;
            }
        }
        if (!swapped) {
            ResetIterators();
            return;
        }
    }
    ResetIterators();
}

void SortVector::CocktailSort() {
    ResetIterators();

    bool swapped = true;
    uint start = 0;
    uint end = GetSize() - 1;

    while (swapped) {
        swapped = false;

        for (uint i = start; i < end; i++) {
            if (More(i, i + 1)) {
                Swap(i, i + 1);
                swapped = true;
            }
        }

        if (!swapped)
            break;

        swapped = false;

        --end;

        for (uint i = end; i > start; i--) {
            if (More(i - 1, i)) {
                Swap(i - 1, i);
                swapped = true;
            }
        }

        ++start;
    }

    ResetIterators();
}

void SortVector::CombSort() {
    ResetIterators();

    uint gap = GetSize();

    bool swapped = true;

    while (gap != 1 || swapped) {
        gap = gap * 10 / 13;
        if (gap < 1)
            gap = 1;

        swapped = false;

        for (uint i = 0; i < GetSize() - gap; i++) {
            if (More(i, i + gap)) {
                Swap(i, i + gap);
                swapped = true;
            }
        }
    }

    ResetIterators();
}

void SortVector::InsertionSort() {
    ResetIterators();
    for (uint i = 1; i < GetSize(); i++) {
        uint j = i - 1;
        while (j + 1 > 0 && More(j, j + 1)) {
            Swap(j, j + 1);
            j--;
        }
    }
    ResetIterators();
}

void SortVector::ShellSort() {
    ResetIterators();
    for (uint gap = GetSize() / 2; gap > 0; gap /= 2) {
        for (uint i = gap; i < GetSize(); i++) {
            uint j = i - gap;
            while (j + gap > j && More(j, j + gap)) {
                Swap(j, j + gap);
                j -= gap;
            }
        }
    }
    ResetIterators();
}

void SortVector::Merge(uint l, uint m, uint r) {
    uint i = 0, j = 0, k = 0;
    uint n1 = m - l;
    uint n2 = r - m;

    std::vector<int> copy(r - l);

    while (i < n1 && j < n2) {
        if (Less(m + j, l + i)) {
            copy[k] = Read(m + j);
            j++;
        } else {
            copy[k] = Read(l + i);
            i++;
        }
        k++;
    }

    while (i < n1) {
        copy[k] = Read(l + i);
        i++;
        k++;
    }

    while (j < n2) {
        copy[k] = Read(m + j);
        j++;
        k++;
    }

    for (k = 0; k < r - l; k++) {
        Write(k + l, copy[k]);
    }
}

void SortVector::MergeSort(uint l, uint r) {
    if (l == 0 && r == GetSize()) {
        ResetIterators();
    }
    if (l + 1 < r) {
        uint m = l + (r - l) / 2;

        MergeSort(l, m);
        MergeSort(m, r);

        Merge(l, m, r);
    }
    if (l == 0 && r == GetSize()) {
        ResetIterators();
    }
}

uint SortVector::Partition(uint l, uint r) {
    uint pivot = r;
    uint i = l - 1;

    for (uint j = l; j < r; j++) {
        if (LessOrEqual(j, pivot)) {
            i++;
            Swap(i, j);
        }
    }

    i++;
    Swap(i, r);
    return i;
}

void SortVector::QuickSort(uint l, uint r) {
    if (l == 0 && r == GetSize()) {
        ResetIterators();
    }
    if (l + 1 < r) {
        uint p = Partition(l, r - 1);

        QuickSort(l, p);
        QuickSort(p + 1, r);
    }
    if (l == 0 && r == GetSize()) {
        ResetIterators();
    }
}

void SortVector::CountingSort() {
    ResetIterators();

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

    for (uint i = 1; i < range; i++)
        count[i] += count[i - 1];

    for (uint i = 0; i < GetSize(); i++) {
        int cur = Read(i);
        output[count[cur - min] - 1] = cur;
        count[cur - min]--;
    }

    for (uint i = 0; i < GetSize(); i++)
        Write(i, output[i]);

    ResetIterators();
}

void SortVector::CountingLSDSubSort(int min, uint base, int exp) {
    std::vector<int> output(GetSize()), count(base);
    int mod = 0;

    for (uint i = 0; i < GetSize(); i++) {
        count[((Read(i) - min) / exp) % base]++;
    }

    for (uint i = 1; i < base; i++)
        count[i] += count[i - 1];

    for (uint i = GetSize(); i > 0; i--) {
        int cur = Read(i - 1);
        mod = ((cur - min) / exp) % base;
        output[count[mod] - 1] = cur;
        count[mod]--;
    }

    for (uint i = 0; i < GetSize(); i++)
        Write(i, output[i]);
}

void SortVector::RadixLSDSort(uint base) {
    ResetIterators();

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

    max -= min;

    for (int exp = 1; max / exp > 0; exp *= base)
        CountingLSDSubSort(min, base, exp);

    ResetIterators();
}

void SortVector::RadixMSDSubSort(int min, uint base, int exp, uint l, uint r) {
    if (l + 1 < r) {
        std::vector<int> output(GetSize()), count(base), lefts(base, l), rights(base, r);
        int mod = 0;

        for (uint i = l; i < r; i++) {
            count[((Read(i) - min) / exp) % base]++;
        }

        for (uint i = 1; i < base; i++) {
            lefts[i] = l + count[i - 1];
            rights[i - 1] = l + count[i - 1];
            count[i] += count[i - 1];
        }

        for (uint i = r; i > l; i--) {
            int cur = Read(i - 1);
            mod = ((cur - min) / exp) % base;
            output[count[mod] - 1] = cur;
            count[mod]--;
        }

        for (uint i = l; i < r; i++)
            Write(i, output[i - l]);

        for (uint i = 0; i < base; i++)
            RadixMSDSubSort(min, base, exp / base, lefts[i], rights[i]);
    }
}

void SortVector::RadixMSDSort(uint base, uint l, uint r) {
    ResetIterators();

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

    max -= min;

    int exp = (int) pow(base, trunc(log(max) / log(base)));
    RadixMSDSubSort(min, base, exp, l, r);

    ResetIterators();
}

void SortVector::StoogeSort(uint l, uint r) {
    if (l == 0 && r == GetSize()) {
        ResetIterators();
    }

    if (l + 2 > r || !IsActive())
        return;

    if (More(l, r - 1))
        Swap(l, r - 1);

    if (r - l > 2) {
        uint t = (r - l) / 3;

        StoogeSort(l, r - t);
        StoogeSort(l + t, r);
        StoogeSort(l, r - t);
    }

    if (l == 0 && r == GetSize()) {
        ResetIterators();
    }
}

void SortVector::BogoSort() {
    ResetIterators();

    bool sorted;

    do {
        sorted = true;

        for (uint i = 1; i < GetSize(); i++)
            if (More(i - 1, i)) {
                sorted = false;
                break;
            }

        if (!sorted)
            BogoShuffle();
    } while (!sorted && IsActive());


    ResetIterators();
}

void SortVector::BogoShuffle() {
    std::default_random_engine rnd(static_cast<uint>(std::chrono::system_clock::now().time_since_epoch().count()));
    double d;

    for (uint i = 0; i < GetSize(); i++) {
        d = std::generate_canonical<double, std::numeric_limits<double>::digits>(rnd);
        d *= (GetSize() - i);
        Swap(i, i + (uint) d);
    }
}
