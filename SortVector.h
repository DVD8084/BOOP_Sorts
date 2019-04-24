//
// Created by DVD on 17.04.2019.
//

#ifndef SORTS_SORTVECTOR_H
#define SORTS_SORTVECTOR_H

#include <vector>

#include "Vector.h"

enum ALGORITHM {
    NONE, SELECTION, BUBBLE, INSERTION, QUICKSORT, MERGESORT
};

class SortVector : public Vector {

// Note: In all sorting implementations, the current state of the supposed "execution pointer" needs to be displayed by calling SetState().
// If the position-only function is called, then the state is determined by the current value of `state`, and vice versa.
// At the start and the end of all implementations, call SetState(0, IDLE, simActive) to clear the current state.

public:
    void SelectionSort(bool &simActive);

    void BubbleSort(bool &simActive);

    void InsertionSort(bool &simActive);

    void QuickSort(bool &simActive);

    void MergeSort(bool &simActive);

};

#endif //SORTS_SORTVECTOR_H
