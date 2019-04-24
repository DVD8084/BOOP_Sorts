//
// Created by DVD on 17.04.2019.
//

#include "SortVector.h"

#include <utility>

typedef unsigned int uint;

void SortVector::SelectionSort(bool &simActive) {
    SetState(0, IDLE, simActive);
    uint min;
    for (uint i = 0; i + 1 < vector.size(); i++) {
        min = i;
        SetState(i, NOTE, simActive);
        for (uint j = i + 1; j < vector.size(); j++) {
            state = READ;
            if (vector[j] < vector[min]) {
                state = NOTE;
                min = j;
            }
            SetState(j, simActive);
        }
        SetState(i, SET, simActive);
        std::swap(vector[i], vector[min]);
        SetState(min, simActive);
    }
    SetState(0, IDLE, simActive);
}

void SortVector::BubbleSort(bool &simActive) {
    SetState(0, IDLE, simActive);
    bool swapOccured;
    for (uint i = 0; i + 1 < vector.size(); i++) {
        swapOccured = false;
        for (uint j = 0; j + i + 1 < vector.size(); j++) {
            SetState(j, READ, simActive);
            if (vector[j] > vector[j + 1]) {
                SetState(j, SET, simActive);
                std::swap(vector[j], vector[j + 1]);
                SetState(j + 1, simActive);
                swapOccured = true;
            }
        }
        if (!swapOccured) {
            SetState(0, IDLE, simActive);
            return;
        }
    }
    SetState(0, IDLE, simActive);
}

void SortVector::InsertionSort(bool &simActive)
{
    SetState(0, IDLE, simActive);
    int key;
    for (uint i = 1; i < vector.size(); i++) {
        key = vector[i];
        SetState(i, NOTE, simActive);
        uint j = i - 1;
        while (j + 1 > 0 && vector[j] > key) {
            SetState(j, NOTE, simActive);
            vector[j + 1] = vector[j];
            SetState(j + 1, SET, simActive);
            j = j - 1;
        }
        vector[j + 1] = key;
        SetState(j + 1, SET, simActive);
    }
    SetState(0, IDLE, simActive);
}