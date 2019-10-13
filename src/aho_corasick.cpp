#include <iostream>
#include <queue>
#include <climits>

#include "aho_corasick.h"

using namespace std;

aho_corasick::aho_corasick(list<string> &patterns) {
    occ.emplace_back();

    // build _goto
    size_t i = 0;
    for (auto &p : patterns) {
        size_t cur = 0;
        for (auto &c : p) {
            if (_goto.size() > cur && _goto[cur][static_cast<unsigned char>(c)] != -1)
                cur = _goto[cur][static_cast<unsigned char>(c)];
            else {
                occ.emplace_back();

                _goto.emplace_back(256, -1);
                _goto[cur][static_cast<unsigned char>(c)] = _goto.size();
                cur = _goto.size();
            }
        }

        occ[cur].emplace_back(i++);
    }
    // done _goto and initial occ

    // build fail
    fail.resize(_goto.size() + 1);

    queue<size_t> q;
    // initialize _goto for every char
    for (size_t c = 0; c < 256; c++) {
        if (!_goto.empty() && _goto[0][static_cast<unsigned char>(c)] != -1 && _goto[0][static_cast<unsigned char>(c)] != 0)
            q.push(_goto[0][static_cast<unsigned char>(c)]);
        else
            _goto[0][static_cast<unsigned char>(c)] = 0;
    }

    while (!q.empty()) {
        size_t state = q.front();
        q.pop();

        for (size_t c = 0; c < 256; c++)
            if (_goto.size() > state && _goto[state][static_cast<unsigned char>(c)] != -1) {// _goto.find({state, c}) != _goto.end()) {
                size_t next = _goto[state][static_cast<unsigned char>(c)];
                q.push(next);

                size_t cur = fail[state];
                while (_goto.size() <= cur || _goto[cur][static_cast<unsigned char>(c)] == -1) //(_goto.find({cur, c}) == _goto.end())
                    cur = fail[cur];

                fail[next] = _goto[cur][static_cast<unsigned char>(c)];
                occ[next].insert(occ[next].end(), occ[fail[next]].begin(), occ[fail[next]].end());
            }
    }
    // done fail and complete
}

size_t aho_corasick::count(string &text) {
    size_t no_occ = 0;
    size_t cur = 0;
    size_t i = 0;
    for (auto &c : text) {
        while (_goto.size() <= cur || _goto[cur][static_cast<unsigned char>(c)] == -1) //(_goto.find({cur, c}) == _goto.end())
            cur = fail[cur];
        cur = _goto[cur][static_cast<unsigned char>(c)];//  [{cur, c}];
        no_occ += occ[cur].size();
        ++i;
    }

    return no_occ;
}

bool aho_corasick::exists(string &text) {
    size_t cur = 0;
    size_t i = 0;
    for (auto &c : text) {
        while (_goto.size() <= cur || _goto[cur][static_cast<unsigned char>(c)] == -1) //(_goto.find({cur, c}) == _goto.end())
            cur = fail[cur];
        cur = _goto[cur][static_cast<unsigned char>(c)];//  [{cur, c}];

        if (!occ[cur].empty())
            return true;
        ++i;
    }
    return false;
}
