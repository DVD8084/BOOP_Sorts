//
// Created by DVD on 17.04.2019.
//

#include "sorts_vector.h"

#include <utility>

typedef unsigned int uint;

void SelectionSort(std::vector<int> &vector, uint &position, PTR_STATE &state, bool &paused, bool &simActive) {
    SetState(position, 0, state, IDLE, paused, simActive);
    uint min;
    for (uint i = 0; i + 1 < vector.size(); i++) {
        min = i;
        SetState(position, i, state, NOTE, paused, simActive);
        for (uint j = i + 1; j < vector.size(); j++) {
            state = READ;
            if (vector[j] < vector[min]) {
                state = NOTE;
                min = j;
            }
            SetState(position, j, paused, simActive);
        }
        SetState(position, i, state, SET, paused, simActive);
        std::swap(vector[i], vector[min]);
        SetState(position, min, paused, simActive);
    }
    SetState(position, 0, state, IDLE, paused, simActive);
}

void BubbleSort(std::vector<int> &vector, uint &position, PTR_STATE &state, bool &paused, bool &simActive) {
    SetState(position, 0, state, IDLE, paused, simActive);
    bool swapOccured;
    for (uint i = 0; i + 1 < vector.size(); i++) {
        swapOccured = false;
        for (uint j = 0; j + i + 1 < vector.size(); j++) {
            SetState(position, j, state, READ, paused, simActive);
            if (vector[j] > vector[j + 1]) {
                SetState(position, j, state, SET, paused, simActive);
                std::swap(vector[j], vector[j + 1]);
                SetState(position, j + 1, paused, simActive);
                swapOccured = true;
            }
        }
        if (!swapOccured) {
            SetState(position, 0, state, IDLE, paused, simActive);
            return;
        }
    }
    SetState(position, 0, state, IDLE, paused, simActive);
}