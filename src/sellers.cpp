#include "sellers.h"
#include <string.h>
#include<vector>
#include <algorithm>


sellers::sellers(list<string> &patterns, size_t err) : patterns(patterns), err(err) {}

size_t sellers::count(string &text) {
    size_t count = 0;

    for (auto &p : patterns) {
        size_t m = p.size();
        vector<vector<size_t>> d(2, vector<uint64_t>(m + 1));
        bool b = false;

        for (size_t i = 0; i <= m; ++i)
            d[!b][i] = i;

        for (size_t j = 1; j <= text.size(); ++j) {
            for (size_t i = 1; i <= m; ++i) {
                d[b][i] = min({
                                      d[!b][i - 1] + (p[i - 1] != text[j - 1]),
                                      d[!b][i] + 1,
                                      d[b][i - 1] + 1,
                                      err + 1
                              });
            }
            if (d[b][m] <= err)
                ++count;

            b = !b;
        }
    }

    return count;
}

bool sellers::exists(string &text) {
    for (auto &p : patterns) {
        size_t m = p.size();
        vector<vector<size_t>> d(2, vector<uint64_t>(m + 1));
        bool b = false;

        for (size_t i = 0; i <= m; ++i)
            d[!b][i] = i;

        for (size_t j = 1; j <= text.size(); ++j) {
            for (size_t i = 1; i <= m; ++i)
                d[b][i] = min({
                                      d[!b][i - 1] + (p[i - 1] != text[j - 1]),
                                      d[!b][i] + 1,
                                      d[b][i - 1] + 1
                              });

            if (d[b][m] <= err)
                return true;

            b = !b;
        }
    }
    return false;
}
