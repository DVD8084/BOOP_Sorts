//
// Created by DVD on 17.04.2019.
//

#ifndef SORTS_SORTS_VECTOR_H
#define SORTS_SORTS_VECTOR_H

#include <vector>

#include "auxil_vector.h"

void SelectionSort(std::vector <int> &vector, uint &pointer, PTR_STATE &state, bool &paused);
void BubbleSort(std::vector <int> &vector, uint &pointer, PTR_STATE &state, bool &paused);
void InsertionSort(std::vector <int> &vector, uint &pointer, PTR_STATE &state, bool &paused);
void QuickSort(std::vector <int> &vector, uint &pointer, PTR_STATE &state, bool &paused);
void MergeSort(std::vector <int> &vector, uint &pointer, PTR_STATE &state, bool &paused);

#endif //SORTS_SORTS_VECTOR_H
