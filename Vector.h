//
// Created by DVD on 16.04.2019.
//

#ifndef SORTS_VECTOR_H
#define SORTS_VECTOR_H

#include "imgui\imgui.h"

#include <iostream>
#include <vector>

typedef unsigned int uint;

enum PTR_STATE {
    IDLE, READ, NOTE, SET, STATE_AMOUNT
};

// Return one of the colors used for displaying states.
ImVec4 StateColor(PTR_STATE color);

class Vector {

protected:
    std::vector<int> vector;
    uint position;
    PTR_STATE state;

    // Basic "pause execution" function. Note that `paused` needs to be set to `true` from another thread for the execution to unpause.
    void Pause();

    // Same as the previous function, but only works while both `paused` and `active` are `true`. Note that `active` isn't set to `true` at the start of this function.
    void Pause(bool &active);

    // Sets the position of the "execution pointer" in the vector and pauses the execution if `simActive` is `true`.
    void SetState(uint newPosition, bool &active);

    // Sets the state of the "execution pointer" in the vector and pauses the execution if `simActive` is `true`.
    void SetState(PTR_STATE newState, bool &active);

    // Sets both the position and the state of the "execution pointer" in the vector and pauses the execution if `simActive` is `true`.
    void SetState(uint newPosition, PTR_STATE newState, bool &active);

public:
    bool paused;

    // Fills the vector with a shuffled set of the first `size` non-negative integers.
    void Shuffle(const uint &size);

    // Fills the vector with a set of the first `size` integers separated by commas from `values`, appended with zeroes if needed, and shuffled if `shuffle` is set to `true`.
    void Shuffle(const uint &size, const std::string &values, bool shuffle = true);

    // Clears the vector.
    void Clear();

    // Returns the size of the vector.
    uint GetSize() const;

    // Returns the element at position `i`.
    int Get(uint i) const;

    // Returns the largest of the amounts of characters needed to display an integer from the vector.
    // Used to prepend the shorter integers by spaces when displaying.
    uint GetMaxLength() const;

    // Returns `true` if the vector is empty.
    bool IsEmpty() const;

    // Returns the position of the "execution pointer".
    uint GetPosition() const;

    // Returns the state of the "execution pointer".
    PTR_STATE GetState() const;

};

#endif //SORTS_VECTOR_H
