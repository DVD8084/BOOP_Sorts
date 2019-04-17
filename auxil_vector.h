//
// Created by DVD on 16.04.2019.
//

#ifndef SORTS_AUXIL_VECTOR_H
#define SORTS_AUXIL_VECTOR_H

#include "imgui\imgui.h"

#include <iostream>
#include <vector>

typedef unsigned int uint;

enum PTR_STATE {IDLE, READ, NOTE, SET, STATE_AMOUNT};
ImVec4 StateColor(PTR_STATE color);

std::vector <int> Shuffle(const uint &size);
std::vector <int> Shuffle(const uint &size, const std::string &values, bool = true);

void Pause(bool &paused);
void SetState(uint &pointer, int newPointer, bool &paused);
void SetState(PTR_STATE &state, PTR_STATE newState, bool &paused);
void SetState(uint &pointer, int newPointer, PTR_STATE &state, PTR_STATE newState, bool &paused);

#include "auxil_vector.tpp"

#endif //SORTS_AUXIL_VECTOR_H
