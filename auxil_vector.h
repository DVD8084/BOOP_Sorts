//
// Created by DVD on 16.04.2019.
//

#ifndef SORTS_AUXIL_VECTOR_H
#define SORTS_AUXIL_VECTOR_H

#include "imgui\imgui.h"

#include <iostream>
#include <vector>

typedef unsigned int uint;

enum PTR_STATE {
    IDLE, READ, NOTE, SET, STATE_AMOUNT
};

// Return one of the colors used for displaying states.
ImVec4 StateColor(PTR_STATE color);

// Returns a vector filled with a shuffled set of the first `size` non-negative integers.
std::vector<int> Shuffle(const uint &size);

// Returns a vector filled with a set of the first `size` integers separated by commas from `values`, appended with zeroes if needed, and shuffled if `shuffle` is set to true.
std::vector<int> Shuffle(const uint &size, const std::string &values, bool shuffle = true);

// Basic "pause execution" function. Note that `paused` needs to be set to `true` from another thread for the execution to unpause.
void Pause(bool &paused);

// Same as the previous function, but only works while both `paused` and `active` are `true`. Note that `active` isn't set to `true` at the start of this function.
void Pause(bool &paused, bool &simActive);

// Sets the position of the "current operational pointer" in the vector and pauses the execution if `simActive` is `true`.
void SetState(uint &position, int newPosition, bool &paused, bool &simActive);

// Sets the state of the "current operational pointer" in the vector and pauses the execution if `simActive` is `true`.
void SetState(PTR_STATE &state, PTR_STATE newState, bool &paused, bool &simActive);

// Sets both the position and the state of the "current operational pointer" in the vector and pauses the execution if `simActive` is `true`.
void SetState(uint &position, int newPosition, PTR_STATE &state, PTR_STATE newState, bool &paused, bool &simActive);

// Returns the largest of the amounts of characters needed to display an integer from the vector.
// Used to prepend the shorter integers by spaces when displaying.
int getMaxLength(const std::vector<int> &vector);

#include "auxil_vector.tpp"

#endif //SORTS_AUXIL_VECTOR_H
