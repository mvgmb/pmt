#include <iostream>
#include <bitset>

#include "shift_or.h"

shift_or::shift_or(list<string> &patterns) : patterns(patterns) {
    for (auto &p : patterns) {
        array<uint64_t, 256> char_mask;

        char_mask.fill(-1); // 111..111
        uint64_t stamp = -2; // 111..110
        uint64_t one = 1; // 000..001

        for (auto &c : p) {
            char_mask[static_cast<unsigned char>(c)] &= stamp;
            stamp <<= 1;
            stamp |= one;
        }
        char_masks.emplace_back(char_mask);
    }
}

size_t shift_or::count(string &text) {
    size_t no_occ = 0;
    auto patterns_it = patterns.begin();
    auto char_masks_it = char_masks.begin();

    while (patterns_it != patterns.end() && char_masks_it != char_masks.end()) {
        auto &pattern = *patterns_it++;
        auto &char_mask = *char_masks_it++;

        uint64_t cur = -1; // 111..111
        uint64_t comparator = 1ul << (pattern.size() - 1);

        for (auto &c : text) {
            cur <<= 1;
            cur |= char_mask[static_cast<unsigned char>(c)];
            if (!(cur & comparator))
                ++no_occ;
        }
    }

    return no_occ;
}

bool shift_or::exists(string &text) {
    auto patterns_it = patterns.begin();
    auto char_masks_it = char_masks.begin();

    while (patterns_it != patterns.end() && char_masks_it != char_masks.end()) {
        auto &pattern = *patterns_it++;
        auto &char_mask = *char_masks_it++;

        uint64_t cur = -1; // 111..111
        uint64_t comparator = 1ul << (pattern.size() - 1);

        for (auto &c : text) {
            cur <<= 1;
            cur |= char_mask[static_cast<unsigned char>(c)];
            if (!(cur & comparator))
                return true;
        }
    }
    return false;
}

