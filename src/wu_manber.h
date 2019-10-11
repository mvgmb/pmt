#ifndef WU_MANBER_H
#define WU_MANBER_H


#include <cstddef>
#include <string>
#include <list>
#include <array>

using namespace std;

class wu_manber {
private:
    size_t err;
    list<array<uint64_t, 256>> char_masks;
    list<string> patterns;

public:
    wu_manber(list<string> &patterns, size_t err);

    size_t count(string &text);

    bool exists(string &text);
};


#endif //WU_MANBER_H
