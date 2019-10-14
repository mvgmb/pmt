#include "sellers.h"
#include <string.h>
#include <algorithm>

sellers::sellers(list<string> &patterns, size_t err) : patterns(patterns), err(err) {}

size_t sellers::count(string &txt) {
    size_t count = 0;

    for (auto pat : patterns) {
        const int n = txt.length(), m = pat.length();
        int cost, last = 1, prev = 0;
        vector<vector<int>> d = vector<vector<int>>(2, vector<int>(m + 1, 0));
        for (int j = 0; j <= n; j++) {
            d[last][0] = 0;
            for (int i = 1; i <= m; i++) {
                cost = pat[i - 1] == txt[j] ? 0 : 1;
                d[last][i] = min({(d[prev][i] + 1),//deletion
                                  (d[last][i - 1] + 1),//insertion
                                  (d[prev][i - 1] + cost)});//substitution
            }
            if (d[last][m] <= err)
                ++count;

            last = (last + 1) % 2;
            prev = (prev + 1) % 2;
        }
    }
    return count;
}


bool sellers::exists(string &txt) {

    for (auto pat : patterns) {
        const int len_txt = txt.length(), len_pat = pat.length();
        int cost, last = 1, prev = 0;
        int d[2][len_pat + 1];
        memset(d, 0, sizeof d);
        for (int j = 0; j <= len_txt; j++) {
            d[last][0] = 0;
            for (int i = 1; i <= len_pat; i++) {
                cost = pat[i - 1] == txt[j] ? 0 : 1;
                d[last][i] = min({(d[prev][i] + 1),//deletion
                                  (d[last][i - 1] + 1),//insertion
                                  (d[prev][i - 1] + cost)});//substitution
            }
            if (d[last][len_pat] <= err)
                return true;
            last = (last + 1) % 2;
            prev = (prev + 1) % 2;
        }
    }
    return false;
}
