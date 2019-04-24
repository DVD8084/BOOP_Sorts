//
// Created by DVD on 16.04.2019.
//

#include "Vector.h"

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

void Vector::Pause() {
    paused = true;
    while (paused) {}
}

void Vector::Pause(bool &active) {
    paused = true;
    while (paused && active) {}
}

void Vector::SetState(uint newPosition, bool &active) {
    position = newPosition;
    Pause(active);
}

void Vector::SetState(PTR_STATE newState, bool &active) {
    state = newState;
    Pause(active);
}

void Vector::SetState(uint newPosition, PTR_STATE newState, bool &active) {
    position = newPosition;
    state = newState;
    Pause(active);
}

void Vector::Shuffle(const uint &size) {
    vector = std::vector<int>(size, -1);
    for (uint i = 0; i < size; i++) {
        vector[i] = i;
    }
    std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(vector.begin(), vector.end(), rnd);
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
        std::default_random_engine rnd(std::chrono::system_clock::now().time_since_epoch().count());
        std::shuffle(vector.begin(), vector.end(), rnd);
    }
}

uint Vector::GetMaxLength() const {
    uint length = 0;
    for (int i : vector) {
        if (std::to_string(i).length() > length) {
            length = std::to_string(i).length();
        }
    }
    return length;
}

uint Vector::GetSize() const {
    return vector.size();
}

int Vector::Get(uint i) const {
    return vector.at(i);
}

void Vector::Clear() {
    vector = std::vector<int>(0);
}

bool Vector::IsEmpty() const {
    return vector.empty();
}

uint Vector::GetPosition() const {
    return position;
}

PTR_STATE Vector::GetState() const {
    return state;
}
