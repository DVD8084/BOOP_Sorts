#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

typedef unsigned int uint;

template <class T>
void Print(const std::vector<T> &v, const std::string &sep = " ") {
    for (int i : v) {
        std::cout << i << sep;
    }
    std::cout << '\n';
}

std::vector <int> Shuffle(const uint &size);
std::vector <int> Shuffle(const uint &size, const std::string &values, bool = true);

int main() {
    std::vector<int> v = Shuffle(10);
    Print(v);
    v = Shuffle(10, "10,11,12,13,14");
    Print(v);
    v = Shuffle(10, "10,11,12,13,14,15,16,17,18,19");
    Print(v);
    v = Shuffle(10, "10,11,12,13,14,15,16,17,18,19,20,21");
    Print(v);
    v = Shuffle(10, "1,1,1,1,1,2,2,2,2,2");
    Print(v);
    return 0;
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
    uint pos = 0;
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
