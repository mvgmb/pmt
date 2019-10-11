#include <vector>
#include <iostream>
#include <bitset>
#include "wu_manber.h"

wu_manber::wu_manber(list<string> &patterns, size_t err) : err(err), patterns(patterns) {
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

size_t wu_manber::count(string &text) {
    size_t no_occ = 0;
    auto patterns_it = patterns.begin();
    auto char_masks_it = char_masks.begin();

    while (patterns_it != patterns.end() && char_masks_it != char_masks.end()) {
        auto &pattern = *patterns_it++;
        auto &char_mask = *char_masks_it++;

        uint64_t comparator = 1 << (pattern.size() - 1);

        vector<uint64_t> sprev;
        sprev.emplace_back(-1); // 111..111

        for (size_t e = 0; e < err; ++e)
            sprev.emplace_back(sprev[e] << 1);

        for (auto &c : text) {
            vector<uint64_t> s;
            s.emplace_back((sprev[0] << 1) | char_mask[static_cast<unsigned char>(c)]);

            for (size_t i = 1; i <= err; ++i)
                s.emplace_back(((sprev[i] << 1) | char_mask[c]) & (sprev[i-1] << 1) & (s[i-1] << 1) & sprev[i-1]);

            if (!(s[err] & comparator))
                ++no_occ;

            sprev = s;
        }
    }
    return no_occ;
}

bool wu_manber::exists(string &text) {
    size_t no_occ = 0;
    auto patterns_it = patterns.begin();
    auto char_masks_it = char_masks.begin();


    while (patterns_it != patterns.end() && char_masks_it != char_masks.end()) {
        auto &pattern = *patterns_it++;
        auto &char_mask = *char_masks_it++;

        vector<uint64_t> s(err + 1, -1); // 111..111
        uint64_t comparator = 1 << (pattern.size() - 1);

        for (auto &c : text) {
            s[0] <<= 1;
            s[0] |= char_mask[c];

            uint64_t prev = s[0];
            for (size_t i = 1; i <= err; ++i) {
                uint64_t si = s[i];
                s[i] = ((s[i] << 1) | char_mask[c]) & (s[i - 1] << 1) & (prev << 1) & prev;
                prev = si;
            }

            if (!(s[err] & comparator))
                return true;
        }
    }
    return false;
}
