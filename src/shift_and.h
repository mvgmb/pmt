//
// Created by mario on 10/12/19.
//

#ifndef SHIFT_AND_H
#define SHIFT_AND_H

#include <unordered_map>
#include <vector>
#include <list>

using namespace std;

class shift_and {
    list <array<uint64_t, 256>> char_masks;
    list <string> patterns;

public:
    explicit shift_and(list <string> &patterns);

    size_t count(string &text);

    bool exists(string &text);
};

#endif //SHIFT_AND_H
