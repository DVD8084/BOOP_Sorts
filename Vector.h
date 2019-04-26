//
// Created by DVD on 16.04.2019.
//

#ifndef SORTS_VECTOR_H
#define SORTS_VECTOR_H

#include "imgui\imgui.h"

#include <iostream>
#include <vector>

#define FRAMESKIP 2;

typedef unsigned int uint;

enum PTR_STATE {
    IDLE, READ, WRITE, COMPARE, STATE_AMOUNT
};

// Return one of the colors used for displaying states.
ImVec4 StateColor(PTR_STATE color);

class Vector {

private:
    std::vector<int> vector;

    void Pause();

    void SetState(uint newPosition);

    void SetState(PTR_STATE newState);

    void SetState(uint newPosition, PTR_STATE newState);

    void Compare(uint i, uint j);

protected:
    uint position = 0;
    PTR_STATE state;
    bool paused = false;
    int frameskip = 0;
    int frameskipCount = 0;
    bool active = false;
    uint read = 0;
    uint write = 0;
    uint compare = 0;

    int Read(uint i);

    void Write(uint i, int x);

    bool Less(uint i, uint j);

    bool More(uint i, uint j);

    bool Equal(uint i, uint j);

    bool LessOrEqual(uint i, uint j);

    bool MoreOrEqual(uint i, uint j);

    void Swap(uint i, uint j);

    void RegisterRead();

    void RegisterWrite();

    void RegisterCompare();

public:

    void Activate();

    void Deactivate();

    void SetFrameskip(int i);

    void Resume();

    bool IsActive() const;

    // Fills the vector with a shuffled set of the first `size` non-negative integers.
    void Shuffle(const uint &size);

    // Fills the vector with a set of the first `size` integers separated by commas from `values`, appended with zeroes if needed, and shuffled if `shuffle` is set to `true`.
    void Shuffle(const uint &size, const std::string &values, bool shuffle = true);

    // Clears and resets the vector.
    void Clear();

    // Resets the "execution pointer".
    void ResetPointer();

    // Returns the size of the vector.
    uint GetSize() const;

    // Returns the element at position `i`.
    int GetElement(uint i) const;

    // Returns the largest of the amounts of characters needed to display an integer from the vector.
    // Used to prepend the shorter integers by spaces when displaying.
    uint GetMaxLength() const;

    // Returns `true` if the vector is empty.
    bool IsEmpty() const;

    // Returns the position of the "execution pointer".
    uint GetPosition() const;

    // Returns the state of the "execution pointer".
    PTR_STATE GetState() const;

    // Returns the amount of read operations.
    uint GetRead() const;

    // Returns the amount of write operations.
    uint GetWrite() const;

    // Returns the amount of compare operations.
    uint GetCompare() const;

};

#endif //SORTS_VECTOR_H
