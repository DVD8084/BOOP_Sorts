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
            ImVec4(1.00f, 0.25f, 0.25f, 1.00f), //WRITE
            ImVec4(1.00f, 1.00f, 0.25f, 1.00f)  //COMPARE
    };
    return colors[color];
}

void Vector::Pause() {
    paused = true;
    while (paused && active) {}
}

void Vector::SetState(uint newPosition) {
    position = newPosition;
    Pause();
}

void Vector::SetState(PTR_STATE newState) {
    state = newState;
    Pause();
}

void Vector::SetState(uint newPosition, PTR_STATE newState) {
    position = newPosition;
    state = newState;
    Pause();
}

void Vector::Shuffle(const uint &size) {
    vector = std::vector<int>(size, -1);
    for (uint i = 0; i < size; i++) {
        vector[i] = i;
    }
    std::default_random_engine rnd(static_cast<uint>(std::chrono::system_clock::now().time_since_epoch().count()));
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
            vector = std::vector<int>(0);
        }
        pos = values.find(',', pos);
    }
    if (shuffle) {
        std::default_random_engine rnd(static_cast<uint>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::shuffle(vector.begin(), vector.end(), rnd);
    }
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
    vector = std::vector<int>(0);
    position = 0;
    state = IDLE;
    read = 0;
    write = 0;
    compare = 0;
    paused = false;
    active = false;
}

void Vector::ResetPointer() {
    SetState(0, IDLE);
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

int Vector::Read(uint i) {
    read++;
    SetState(i, READ);
    return vector.at(i);
}

void Vector::Write(uint i, int x) {
    write++;
    SetState(i, WRITE);
    vector.at(i) = x;
}

void Vector::Compare(uint i, uint j) {
    compare++;
    SetState(i, COMPARE);
    SetState(j, COMPARE);
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
    int buf = Read(i);
    Write(i, Read(j));
    Write(j, buf);
}

bool Vector::IsActive() const {
    return active;
}

void Vector::RegisterRead() {
    read++;
}

void Vector::RegisterWrite() {
    write++;
}

void Vector::RegisterCompare() {
    compare++;
}
