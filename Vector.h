//
// Created by DVD on 16.04.2019.
//

#ifndef SORTS_VECTOR_H
#define SORTS_VECTOR_H

#include "imgui\imgui.h"

#include <iostream>
#include <map>
#include <vector>

#define FRAMESKIP 2;

typedef unsigned int uint;

enum PTR_STATE {
    IDLE, READ, WRITE, COMPARE, STATE_AMOUNT
};

typedef std::pair<const uint, PTR_STATE> ExecPointer;

// Return one of the colors used for displaying states.
ImVec4 StateColor(PTR_STATE color);

class Vector {

private:
    std::vector<int> vector;
    std::vector<int> backup;

    void Pause();

    void SetState(const std::initializer_list<ExecPointer> &newPointers);

    void Compare(uint i, uint j);

protected:
    std::map<uint, PTR_STATE> pointers;
    bool paused = false;
    int frameskip = 0;
    int frameskipCount = 0;
    bool active = false;
    uint read = 0;
    uint write = 0;
    uint compare = 0;
    uint swap = 0;

    int Read(uint i);

    void Write(uint i, int x);

    bool Less(uint i, uint j);

    bool More(uint i, uint j);

    bool Equal(uint i, uint j);

    bool LessOrEqual(uint i, uint j);

    bool MoreOrEqual(uint i, uint j);

    void Swap(uint i, uint j);

public:

    void Activate();

    void Deactivate();

    void SetFrameskip(int i);

    void Resume();

    void Reload();

    bool IsActive() const;

    // Fills the vector with a shuffled set of the first `size` non-negative integers.
    void Shuffle(const uint &size);

    // Fills the vector with a set of the first `size` integers separated by commas from `values`, appended with zeroes if needed, and shuffled if `shuffle` is set to `true`.
    void Shuffle(const uint &size, const std::string &values, bool shuffle = true);

    // Clears and resets the vector.
    void Clear();

    // Resets the "execution pointer".
    void ResetPointers();

    // Returns the size of the vector.
    uint GetSize() const;

    // Returns the element at position `i`.
    int GetElement(uint i) const;

    // Returns the largest of the amounts of characters needed to display an integer from the vector.
    // Used to prepend the shorter integers by spaces when displaying.
    uint GetMaxLength() const;

    // Returns `true` if the vector is empty.
    bool IsEmpty() const;

    // Returns the positions and states of the "execution pointers".
    std::map<uint, PTR_STATE> GetPointers() const;

    // Returns the amount of read operations.
    uint GetRead() const;

    // Returns the amount of write operations.
    uint GetWrite() const;

    // Returns the amount of compare operations.
    uint GetCompare() const;

    // Returns the amount of swap operations.
    uint GetSwap() const;

    bool StateHasNotChanged() const;

};

#endif //SORTS_VECTOR_H
