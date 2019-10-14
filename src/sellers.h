#ifndef PMT_SELLERS_H
#define PMT_SELLERS_H

#include <list>
#include <string>

using namespace std;

class sellers {
private:
    size_t err;
    list<string> patterns;

public:
    explicit sellers(list<string> &patterns, size_t err);

    size_t count(string &txt);

    bool exists(string &txt);
};


#endif //PMT_SELLERS_H
