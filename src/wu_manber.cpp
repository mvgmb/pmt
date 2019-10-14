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

        uint64_t comparator = 1ul << (pattern.size() - 1);

        bool b = true;
        vector<vector<uint64_t>> s(2, vector<uint64_t>(err + 1));

        size_t index = 0;
        s[b][index++] = -1; // 111..111

        for (size_t e = 0; e < err; ++e)
            s[b][index++] = s[b][e] << 1;

        for (auto &c : text) {
            index = 0;
            s[!b][index++] = ((s[b][0] << 1) | char_mask[static_cast<unsigned char>(c)]);

            for (size_t i = 1; i <= err; ++i)
                s[!b][index++] = (((s[b][i] << 1) | char_mask[c]) & (s[b][i - 1] << 1) & (s[!b][i - 1] << 1) & s[b][i - 1]);

            if (!(s[!b][err] & comparator))
                ++no_occ;

            b = !b;
        }
    }
    return no_occ;
}

bool wu_manber::exists(string &text) {
    auto patterns_it = patterns.begin();
    auto char_masks_it = char_masks.begin();

    while (patterns_it != patterns.end() && char_masks_it != char_masks.end()) {
        auto &pattern = *patterns_it++;
        auto &char_mask = *char_masks_it++;

        uint64_t comparator = 1ul << (pattern.size() - 1);

        bool b = true;
        vector<vector<uint64_t>> s(2, vector<uint64_t>(err + 1));

        size_t index = 0;
        s[b][index++] = -1; // 111..111

        for (size_t e = 0; e < err; ++e)
            s[b][index++] = s[b][e] << 1;

        for (auto &c : text) {
            index = 0;
            s[!b][index++] = ((s[b][0] << 1) | char_mask[static_cast<unsigned char>(c)]);

            for (size_t i = 1; i <= err; ++i)
                s[!b][index++] = (((s[b][i] << 1) | char_mask[c]) & (s[b][i - 1] << 1) & (s[!b][i - 1] << 1) & s[b][i - 1]);

            if (!(s[!b][err] & comparator))
                return true;

            b = !b;
        }
    }
    return false;
}
