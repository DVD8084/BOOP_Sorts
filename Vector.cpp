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

ImVec4 StateColor(PTR_STATE color) {
    static const ImVec4 colors[STATE_AMOUNT] = {
            ImVec4(0.25f, 0.50f, 0.50f, 1.00f), //IDLE
            ImVec4(0.25f, 0.75f, 0.25f, 1.00f), //READ
            ImVec4(0.75f, 0.25f, 0.25f, 1.00f), //WRITE
            ImVec4(0.75f, 0.75f, 0.25f, 1.00f)  //COMPARE
    };
    return colors[color];
}

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


void Vector::SetState(const std::initializer_list<ExecPointer> &newPointers) {
    pointers = std::map<uint, PTR_STATE>(newPointers);
    Pause();
}

void Vector::Shuffle(const uint &size) {
    vector = std::vector<int>(size, -1);
    for (uint i = 0; i < size; i++) {
        vector[i] = i;
    }
    std::default_random_engine rnd(static_cast<uint>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::shuffle(vector.begin(), vector.end(), rnd);
    backup = vector;
}

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

uint Vector::GetMaxLength() const {
    if (vector.empty())
        return 0;
    uint length_max = static_cast<uint>(std::to_string(*max_element(vector.begin(), vector.end())).length());
    uint length_min = static_cast<uint>(std::to_string(*min_element(vector.begin(), vector.end())).length());
    return length_max > length_min ? length_max : length_min;
}

uint Vector::GetSize() const {
    return static_cast<uint>(vector.size());
}

int Vector::GetElement(uint i) const {
    return vector.at(i);
}

void Vector::Clear() {
    vector.clear();
    pointers.clear();
    read = 0;
    write = 0;
    compare = 0;
    swap = 0;
    frameskip = 0;
    frameskipCount = 0;
    paused = false;
    active = false;
}

void Vector::ResetPointers() {
    frameskip = 0;
    frameskipCount = 0;
    SetState({});
}

bool Vector::IsEmpty() const {
    return vector.empty();
}

std::map<uint, PTR_STATE> Vector::GetPointers() const {
    return pointers;
}


int Vector::Read(uint i) {
    read++;
    SetState({{i, READ}});
    return vector.at(i);
}

void Vector::Write(uint i, int x) {
    write++;
    SetState({{i, WRITE}});
    vector.at(i) = x;
}

void Vector::Compare(uint i, uint j) {
    compare++;
    SetState({{i, COMPARE},
              {j, COMPARE}});
}

bool Vector::Less(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) < vector.at(j);
}

bool Vector::More(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) > vector.at(j);
}

bool Vector::Equal(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) == vector.at(j);
}

bool Vector::LessOrEqual(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) <= vector.at(j);
}

bool Vector::MoreOrEqual(uint i, uint j) {
    Compare(i, j);
    return vector.at(i) >= vector.at(j);
}

uint Vector::GetRead() const {
    return read;
}

uint Vector::GetWrite() const {
    return write;
}

uint Vector::GetCompare() const {
    return compare;
}

uint Vector::GetSwap() const {
    return swap;
}

void Vector::Activate() {
    active = true;
}

void Vector::Deactivate() {
    active = false;
}

void Vector::Resume() {
    paused = false;
}

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

bool Vector::IsActive() const {
    return active;
}

void Vector::SetFrameskip(int i) {
    frameskip = i;
}

void Vector::Reload() {
    Clear();
    vector = backup;
}

bool Vector::StateHasNotChanged() const {
    return frameskipCount < 0;
}
