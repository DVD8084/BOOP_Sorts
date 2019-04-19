//
// Created by DVD on 17.04.2019.
//

#ifndef SORTS_SORTS_VECTOR_H
#define SORTS_SORTS_VECTOR_H

#include <vector>

#include "auxil_vector.h"

enum ALGORITHM {
    NONE, SELECTION, BUBBLE, INSERTION, QUICKSORT, MERGESORT
};

// Note: In all sorting implementations, the current state of the supposed "execution pointer" needs to be displayed by calling SetState().
// If the position-only function is called, then the state is determined by the current value of `state`, and vice versa.
// At the start and the end of all implementations, call SetState(position, 0, state, IDLE, paused, simActive) to clear the current state.

void SelectionSort(std::vector<int> &vector, uint &position, PTR_STATE &state, bool &paused, bool &simActive);

void BubbleSort(std::vector<int> &vector, uint &position, PTR_STATE &state, bool &paused, bool &simActive);

void InsertionSort(std::vector<int> &vector, uint &position, PTR_STATE &state, bool &paused, bool &simActive);

void QuickSort(std::vector<int> &vector, uint &position, PTR_STATE &state, bool &paused, bool &simActive);

void MergeSort(std::vector<int> &vector, uint &position, PTR_STATE &state, bool &paused, bool &simActive);

#endif //SORTS_SORTS_VECTOR_H
