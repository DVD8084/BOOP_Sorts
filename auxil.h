//
// Created by DVD on 16.04.2019.
//

#ifndef SORTS_AUXIL_H
#define SORTS_AUXIL_H

#include <iostream>
#include <vector>

typedef unsigned int uint;

std::vector <int> Shuffle(const uint &size);
std::vector <int> Shuffle(const uint &size, const std::string &values, bool = true);

#include "auxil.tpp"

#endif //SORTS_AUXIL_H
