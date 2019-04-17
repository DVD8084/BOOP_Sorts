//
// Created by DVD on 16.04.2019.
//

#include "auxil_vector.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

ImVec4 StateColor(PTR_STATE color) {
    static const ImVec4 colors[STATE_AMOUNT] = {
            ImVec4(0.25f, 0.50f, 0.50f, 1.00f), //IDLE
            ImVec4(0.25f, 1.00f, 0.25f, 1.00f), //READ
            ImVec4(1.00f, 1.00f, 0.25f, 1.00f), //NOTE
            ImVec4(1.00f, 0.25f, 0.25f, 1.00f)  //SET
    };
    return colors[color];
}

void Pause(bool &paused) {
    paused = true;
    while (paused) {}
}

void SetState(uint &pointer, int newPointer, bool &paused) {
    pointer = newPointer;
    Pause(paused);
}

void SetState(PTR_STATE &state, PTR_STATE newState, bool &paused) {
    state = newState;
    Pause(paused);
}

void SetState(uint &pointer, int newPointer, PTR_STATE &state, PTR_STATE newState, bool &paused) {
    pointer = newPointer;
    state = newState;
    Pause(paused);
}

std::vector<int> Shuffle(const uint &size) {
    std::vector<int> values(size, -1);
    for (uint i = 0; i < size; i++)
    {
        values[i] = i;
    }
    std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(values.begin(), values.end(), rnd);
    return values;
}

std::vector<int> Shuffle(const uint &size, const std::string &values, const bool shuffle) {
    unsigned long long pos = 0;
    std::vector<int> v(size, 0);
    for (int &i : v) {
        if (pos == std::string::npos)
            continue;
        if (pos)
            ++pos;
        std::string value = values.substr(pos, values.find(',', pos));
        try {
            i = stoi(value);
        }
        catch(const std::invalid_argument&) {
            return std::vector<int>();
        }
        pos = values.find(',', pos);
    }
    if (shuffle) {
        std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
        std::shuffle(v.begin(), v.end(), rnd);
    }
    return v;
}