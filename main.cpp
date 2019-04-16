#include <iostream>

#include "auxil.h"

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
