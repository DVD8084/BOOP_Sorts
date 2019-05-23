//
// Created by DVD on 16.04.2019.
//

#ifndef SORTS_VECTOR_H
#define SORTS_VECTOR_H

#include <imgui.h>

#include <iostream>
#include <map>
#include <vector>

typedef unsigned int uint;

enum ITER_STATE {
    IDLE, READ, WRITE, COMPARE, STATE_AMOUNT
};

typedef std::pair<const uint, ITER_STATE> ExecIterator;

ImVec4 StateColor(ITER_STATE color);

class Vector {

private:
    std::vector<int> vector;
    std::vector<int> backup;

    void Pause();

    void SetState(const std::initializer_list<ExecIterator> &newIterators);

    void Compare(uint i, uint j);

protected:
    std::map<uint, ITER_STATE> iterators;
    bool paused = false;
    int frameskip = 0;
    int frameskipCount = 0;
    bool active = false;
    uint read = 0;
    uint write = 0;
    uint compare = 0;
    uint swap = 0;

    int Read(uint i);

    void Write(uint i, int x);

    bool Less(uint i, uint j);

    bool More(uint i, uint j);

    bool Equal(uint i, uint j);

    bool LessOrEqual(uint i, uint j);

    bool MoreOrEqual(uint i, uint j);

    void Swap(uint i, uint j);

public:

    void Activate();

    void Deactivate();

    void SetFrameskip(int i);

    void Resume();

    void Reload();

    bool IsActive() const;

    void Shuffle(const uint &size);

    void Shuffle(const uint &size, const std::string &values, bool shuffle = true);

    void Clear();

    void ResetIterators();

    uint GetSize() const;

    int GetElement(uint i) const;

    uint GetMaxLength() const;

    bool IsEmpty() const;

    std::map<uint, ITER_STATE> GetIterators() const;

    uint GetRead() const;

    uint GetWrite() const;

    uint GetCompare() const;

    uint GetSwap() const;

    bool StateHasNotChanged() const;

};

#endif //SORTS_VECTOR_H
