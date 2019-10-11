#include <iostream>
#include <queue>
#include <climits>

#include "aho_corasick.h"

using namespace std;

aho_corasick::aho_corasick(list<string> &patterns) {
    // build _goto
    size_t i = 0;
    for (auto &p : patterns) {
        size_t cur = 0;
        for (auto &c : p) {
            pair<char, size_t> key = {cur, c};
            if (_goto.find(key) != _goto.end())
                cur = _goto[key];
            else
                _goto[key] = cur = _goto.size();
        }

        occ[cur].emplace_back(i++);
    }
    // done _goto and initial occ

    // build fail
    fail.resize(_goto.size() + 1);

    queue<size_t> q;
    // initialize _goto for every char
    for (size_t c = 0; c <= 256; c++) {
        if (_goto[{0, c}] != 0)
            q.push(_goto[{0, c}]);
    }

    while (!q.empty()) {
        size_t state = q.front();
        q.pop();

        for (size_t c = 0; c <= 256; c++)
            if (_goto.find({state, c}) != _goto.end()) {
                size_t next = _goto[{state, c}];
                q.push(next);

                size_t cur = fail[state];
                while (_goto.find({cur, c}) == _goto.end())
                    cur = fail[cur];

                fail[next] = _goto[{cur, c}];
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
        while (_goto.find({cur, c}) == _goto.end())
            cur = fail[cur];
        cur = _goto[{cur, c}];
        no_occ += occ[cur].size();
        ++i;
    }

    return no_occ;
}

bool aho_corasick::exists(string &text) {
    size_t cur = 0;
    size_t i = 0;
    for (auto &c : text) {
        while (_goto.find({cur, c}) == _goto.end())
            cur = fail[cur];
        cur = _goto[{cur, c}];

        if (!occ[cur].empty())
            return true;
        ++i;
    }
    return false;
}

void aho_corasick::print_goto(unordered_map<pair<size_t, char>, size_t> &_goto) {
    for (unordered_map<pair<size_t, char>, size_t>::const_iterator it = _goto.begin(); it != _goto.end(); ++it)
        if (it->first.first != 0 || it->second != 0)
            cout << "{" << it->first.first << ", " << it->first.second << "} -> " << it->second << "\n";
}

void aho_corasick::print_fail(vector<size_t> &fail) {
    for (auto &f : fail)
        cout << f << " ";
    cout << endl;
}

void aho_corasick::print_occ(unordered_map<size_t, vector<size_t>> &occ) {
    for (auto &oc : occ) {
        cout << "{" << oc.first << "} -> ";
        for (auto &o : oc.second)
            cout << " " << o;
        cout << endl;
    }
}