//
// Created by DVD on 16.04.2019.
//

#include "Vector.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <utility>
#include <vector>

// Return one of the colors used for displaying states.
ImVec4 StateColor(ITER_STATE color) {
    static const ImVec4 colors[STATE_AMOUNT] = {
            ImVec4(0.25f, 0.50f, 0.50f, 1.00f), //IDLE
            ImVec4(0.25f, 0.75f, 0.25f, 1.00f), //READ
            ImVec4(0.75f, 0.25f, 0.25f, 1.00f), //WRITE
            ImVec4(0.75f, 0.75f, 0.25f, 1.00f)  //COMPARE
    };
    return colors[color];
}

// Pauses the simulation if it is running.
// If the frameskip is positive, pauses only after the appropriate amount of frames has been skipped.
// If the frameskip is negative, inserts the corresponding number of "blank frames" instead.
void Vector::Pause() {
    if (frameskip < 0)
        frameskipCount = frameskip;
    if (frameskipCount <= 0) {
        while (frameskipCount <= 0) {
            paused = true;
            while (paused && active) {}
            frameskipCount++;
            if (frameskip >= 0)
                frameskipCount = 1;
        }
    } else {
        frameskipCount++;
    }
    if (frameskip >= 0)
        frameskipCount = frameskipCount % (frameskip + 1);
}

// Sets the state of the iterators and calls Pause().
void Vector::SetState(const std::initializer_list<ExecIterator> &newIterators) {
    iterators = std::map<uint, ITER_STATE>(newIterators);
    Pause();
}

// Fills the vector with a shuffled set of the first `size` non-negative integers.
void Vector::Shuffle(const uint &size) {
    vector = std::vector<int>(size, -1);
    for (uint i = 0; i < size; i++) {
        vector[i] = i;
    }
    std::default_random_engine rnd(static_cast<uint>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::shuffle(vector.begin(), vector.end(), rnd);
    backup = vector;
}

// Fills the vector with a set of the first `size` integers separated by commas from `values`, appended with zeroes if needed, and shuffled if `shuffle` is set to `true`.
void Vector::Shuffle(const uint &size, const std::string &values, const bool shuffle) {
    unsigned long long pos = 0;
    vector = std::vector<int>(size, 0);
    for (int &i : vector) {
        if (pos == std::string::npos)
            continue;
        if (pos)
            ++pos;
        std::string value = values.substr(pos, values.find(',', pos));
        try {
            i = stoi(value);
        }
        catch (const std::invalid_argument &) {
            vector = std::vector<int>();
        }
        pos = values.find(',', pos);
    }
    if (shuffle) {
        std::default_random_engine rnd(static_cast<uint>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::shuffle(vector.begin(), vector.end(), rnd);
    }
    backup = vector;
}

// Returns the largest of the amounts of characters needed to display an integer from the vector.
// Used to prepend the shorter integers by spaces when displaying.
uint Vector::GetMaxLength() const {
    if (vector.empty())
        return 0;
    uint length_max = static_cast<uint>(std::to_string(*max_element(vector.begin(), vector.end())).length());
    uint length_min = static_cast<uint>(std::to_string(*min_element(vector.begin(), vector.end())).length());
    return length_max > length_min ? length_max : length_min;
}

// Returns the size of the vector.
uint Vector::GetSize() const {
    return static_cast<uint>(vector.size());
}

// Returns the element at position `i`.
int Vector::GetElement(uint i) const {
    return vector.at(i);
}

// Clears and resets the vector.
void Vector::Clear() {
    vector.clear();
    iterators.clear();
    read = 0;
    write = 0;
    compare = 0;
    swap = 0;
    frameskip = 0;
    frameskipCount = 0;
    paused = false;
    active = false;
}

// Resets the iterators, leaving the vector alone.
void Vector::ResetIterators() {
    frameskip = 0;
    frameskipCount = 0;
    SetState({});
}

// Returns `true` if the vector is empty.
bool Vector::IsEmpty() const {
    return vector.empty();
}

// Returns the positions and states of the iterators as an `std::map`.
std::map<uint, ITER_STATE> Vector::GetIterators() const {
    return iterators;
}

// Conducts a read operation, updating the iterators. Returns the corresponding element.
int Vector::Read(uint i) {
    read++;
    SetState({{i, READ}});
    return vector.at(i);
}

// Conducts a write operation, updating the iterators.
void Vector::Write(uint i, int x) {
    write++;
    SetState({{i, WRITE}});
    vector.at(i) = x;
}

// Conducts a compare operation, updating the iterators.
// NOTE: Cannot be used directly. Comparisons such as Less(x, y) are used instead.
void Vector::Compare(uint i, uint j) {
    compare++;
    SetState({{i, COMPARE},
              {j, COMPARE}});
}

// Calls Compare(i, j) and returns `true` if i < j.
bool Vector::Less(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) < vector.at(j);
}

// Calls Compare(i, j) and returns `true` if i > j.
bool Vector::More(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) > vector.at(j);
}

// Calls Compare(i, j) and returns `true` if i == j.
bool Vector::Equal(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) == vector.at(j);
}

// Calls Compare(i, j) and returns `true` if i <= j.
bool Vector::LessOrEqual(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) <= vector.at(j);
}

// Calls Compare(i, j) and returns `true` if i >= j.
bool Vector::MoreOrEqual(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) >= vector.at(j);
}

// Conducts a swap operation, updating the iterators.
void Vector::Swap(uint i, uint j) {
    if (i != j) {
        int buf = vector[i];
        vector[i] = vector[j];
        vector[j] = buf;
        swap++;
        SetState({{i, WRITE},
                  {j, WRITE}});
    }
}

// Returns the amount of read operations.
uint Vector::GetRead() const {
    return read;
}

// Returns the amount of write operations.
uint Vector::GetWrite() const {
    return write;
}

// Returns the amount of compare operations.
uint Vector::GetCompare() const {
    return compare;
}

// Returns the amount of swap operations.
uint Vector::GetSwap() const {
    return swap;
}

// Activates the simulation. The execution will pause when calling the Pause() function only if the simulation is active.
void Vector::Activate() {
    active = true;
}

// Deactivates the simulation. The execution will not pause until the simulation is active again.
void Vector::Deactivate() {
    active = false;
}

// Unpauses the execution.
void Vector::Resume() {
    paused = false;
}

// Returns `true` if the simulation is active.
bool Vector::IsActive() const {
    return active;
}

//Sets the frameskip.
void Vector::SetFrameskip(int i) {
    frameskip = i;
}

//Reloads the vector in order to restart the simulation.
void Vector::Reload() {
    Clear();
    vector = backup;
}

//Returns `true` if the simulation is currently displaying "blank frames". Used to skip them when doing step-by-step simulation.
bool Vector::StateHasNotChanged() const {
    return frameskipCount < 0;
}
