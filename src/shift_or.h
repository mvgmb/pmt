#ifndef SHIFT_OR_H
#define SHIFT_OR_H

#include <unordered_map>
#include <vector>
#include <list>

using namespace std;

class shift_or {
private:
    list <array<uint64_t, 256>> char_masks;
    list <string> patterns;

public:
    explicit shift_or(list <string> &patterns);

    size_t count(string &text);

    bool exists(string &text);
};

#endif //SHIFT_OR_H
