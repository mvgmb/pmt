#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include <unordered_map>
#include <vector>
#include <list>

using namespace std;

namespace std {

    template<typename T1, typename T2>
    struct hash<pair<T1, T2>> {
        size_t operator()(const pair<T1, T2> &p) const {
            return hash<T1>()(p.first) ^ hash<T2>()(p.second);
        }
    };
}

class aho_corasick {
private:
    unordered_map<pair<size_t, char>, size_t> _goto;
    unordered_map<size_t, vector<size_t>> occ;
    vector<size_t> fail;

public:
    explicit aho_corasick(list <string> &patterns);

    size_t count(string &text);

    bool exists(string &text);

    void print_goto(unordered_map<pair<size_t, char>, size_t> &_goto);

    void print_fail(vector<size_t> &fail);

    void print_occ(unordered_map<size_t, vector<size_t>> &occ);
};
#endif //AHO_CORASICK_H
