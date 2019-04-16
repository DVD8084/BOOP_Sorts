//
// Created by DVD on 16.04.2019.
//

#include "auxil.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

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