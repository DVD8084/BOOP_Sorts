//
// Created by DVD on 17.04.2019.
//

#ifndef SORTS_SORTVECTOR_H
#define SORTS_SORTVECTOR_H

#include <vector>

#include "Vector.h"

enum ALGORITHM {
    NONE, SELECTION, BUBBLE, INSERTION, QUICKSORT, MERGESORT, COUNTING, RADIXLSD, RADIXMSD, COCKTAIL, COMB, SHELLSORT,
    STOOGE, BOGOSORT
};

class SortVector : public Vector {

private:
    uint Partition(uint l, uint r);

    void Merge(uint l, uint m, uint r);

    void CountingLSDSubSort(int min, uint base, int exp);

    void RadixMSDSubSort(int min, uint base, int exp, uint l, uint r);

    void BogoShuffle();

public:
    void SelectionSort();

    void BubbleSort();

    void InsertionSort();

    void QuickSort(uint l, uint r);

    void MergeSort(uint l, uint r);

    void CountingSort();

    void RadixLSDSort(uint base);

    void RadixMSDSort(uint base, uint l, uint r);

    void CocktailSort();

    void CombSort();

    void ShellSort();

    void StoogeSort(uint l, uint r);

    void BogoSort();

};

#endif //SORTS_SORTVECTOR_H
