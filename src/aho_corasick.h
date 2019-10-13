#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include <unordered_map>
#include <vector>
#include <list>

using namespace std;

class aho_corasick {
private:
    vector<vector<size_t>> _goto;
    vector<vector<size_t>> occ;
    vector<size_t> fail;

public:
    explicit aho_corasick(list <string> &patterns);

    size_t count(string &text);

    bool exists(string &text);
};
#endif //AHO_CORASICK_H
