//
// Created by DVD on 17.04.2019.
//

#include "sorts_vector.h"

#include <utility>

typedef unsigned int uint;

void SelectionSort(std::vector <int> &vector, uint &pointer, PTR_STATE &state, bool &paused) {
    SetState(pointer, 0, state, IDLE, paused);
    uint min;
    for (uint i = 0; i < vector.size() - 1; i++) {
        min = i;
        state = NOTE;
        SetState(pointer, i, paused);
        for (uint j = i + 1; j < vector.size(); j++) {
            state = READ;
            if (vector[j] < vector[min]) {
                state = NOTE;
                min = j;
            }
            SetState(pointer, j, paused);
        }
        state = SET;
        SetState(pointer, i, paused);
        std::swap(vector[i], vector[min]);
        SetState(pointer, min, paused);
    }
    pointer = 0;
    state = IDLE;
}

void BubbleSort(std::vector <int> &vector, uint &pointer, PTR_STATE &state, bool &paused) {
    SetState(pointer, 0, state, IDLE, paused);
    bool swapOccured;
    for (uint i = 0; i < vector.size() - 1; i++) {
        swapOccured = false;
        for (uint j = 0; j < vector.size() - i - 1; j++) {
            state = READ;
            SetState(pointer, j, paused);
            if (vector[j] > vector[j + 1]) {
                state = SET;
                SetState(pointer, j, paused);
                std::swap(vector[j], vector[j + 1]);
                SetState(pointer, j + 1, paused);
                swapOccured = true;
            }
        }
        if (!swapOccured) {
            pointer = 0;
            state = IDLE;
            return;
        }
    }
    pointer = 0;
    state = IDLE;
}