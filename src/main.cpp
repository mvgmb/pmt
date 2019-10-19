#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <sstream>

#include <getopt.h>

#include "aho_corasick.h"
#include "shift_or.h"
#include "shift_and.h"
#include "wu_manber.h"
#include "sellers.h"

using namespace std;

enum algorithm {
    AHO_CORASICK = 'a',
    OR_SHIFT = 'o',
    WU_MANBER = 'w',
    SELLERS = 's',
};

enum options {
    DEFAULT = 0x00,
    COUNT = 0x01,
    LINE_COUNT = 0x02,
};

void help(char *s) {
    cerr
            << "Usage: " << s << " [options] PATTERN [FILE...]" << endl
            << endl
            << "Search for PATTERN in each FILE" << endl
            << endl
            << "Options:" << endl
            << "  -a, --algorithm NAME  set matching algorithm ('a'/'aho-corasick' 'o'/'or-shift' 'w'/'wu-manber' 's'/`sellers`) (default 'aho-corasick')"
            << endl
            << "  -e, --edit UINT       set max edit distance (default 0)" << endl
            << "  -p, --pattern FILE    obtain patterns from FILE" << endl
            << endl
            << "  -c, --count           print only the total count of occurrences" << endl
            << "  -l, --line_count      print only the total count of lines that has occurrences" << endl
            << "  -h, --help            display this information" << endl
            << endl
            << "Example: " << s << " pattern123 file1.txt file2.cpp" << endl
            << endl;
}

int main(int argc, char *argv[]) {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    const char *short_options = "a:e:p:clh";
    const option long_options[] = {
            {"algorithm",  required_argument, nullptr, 'a'},
            {"edit",       required_argument, nullptr, 'e'},
            {"pattern",    required_argument, nullptr, 'p'},
            {"count",      no_argument,       nullptr, 'c'},
            {"line_count", no_argument,       nullptr, 'l'},
            {"help",       no_argument,       nullptr, 'h'},
            {nullptr,      no_argument,       nullptr, '\0'},
    };
    int option_index = -1;

    algorithm a = AHO_CORASICK;
    size_t e = 0;
    size_t m = options::DEFAULT;

    list<string> patterns;
    queue<unique_ptr<istream, function<void(istream *)>>> t;

    int c;
    while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1)
        switch (c) {
            case 'a': {
                a = static_cast<algorithm>(optarg[0]);
                break;
            }

            case 'e': {
                stringstream(optarg) >> e;
                break;
            }

            case 'p': {
                ifstream ifs(optarg);
                string s;
                while (ifs >> s)
                    patterns.emplace_back(s);
                break;
            }

            case 'c': {
                m |= options::COUNT;
                break;
            }

            case 'l': {
                m |= options::LINE_COUNT;
                break;
            }

            case 'h':
            case '?':
            default: {
                help(argv[0]);
                return EXIT_FAILURE;
            }
        }

    for (auto i = static_cast<size_t>(optind); i < argc; ++i) {
        if (patterns.empty())
            patterns.emplace_back(argv[i]);
        else
            t.emplace(new ifstream(argv[i]), [](istream *v) { delete v; });
    }
    if (t.empty())
        t.emplace(&cin, [](istream *) {});

    if (patterns.empty()) {
        help(argv[0]);
        return EXIT_FAILURE;
    }

    function<size_t(string &)> count;
    function<bool(string &)> exists;

    switch (a) {
        case algorithm::WU_MANBER: {
            auto algorithm = new wu_manber(patterns, e);
            count = [&](string &s) -> size_t { return algorithm->count(s); };
            exists = [&](string &s) -> bool { return algorithm->exists(s); };
            break;
        }
        case algorithm::SELLERS: {
            auto algorithm = new sellers(patterns, e);
            count = [&](string &s) -> size_t { return algorithm->count(s); };
            exists = [&](string &s) -> bool { return algorithm->exists(s); };
            break;
        }
        case algorithm::OR_SHIFT: {
            auto algorithm = new shift_or(patterns);
            count = [&](string &s) -> size_t { return algorithm->count(s); };
            exists = [&](string &s) -> bool { return algorithm->exists(s); };
            break;
        }
        case algorithm::AHO_CORASICK:
        default:
            auto algorithm = new aho_corasick(patterns);
            count = [&](string &s) -> size_t { return algorithm->count(s); };
            exists = [&](string &s) -> bool { return algorithm->exists(s); };
            break;
    }

    while (!t.empty()) {
        istream &is = *t.front();

        size_t sum = 0;

        string text;
        while (getline(is, text)) {
            if (!is.eof())
                text += '\n';

            if (m & options::COUNT)
                sum += count(text);
            else if ((m & options::LINE_COUNT) && exists(text))
                ++sum;
            else if (exists(text)) {
                cout << text;

                if (is.eof())
                    cout << endl;
            }
        }

        if (m & options::COUNT || m & options::LINE_COUNT)
            cout << sum << endl;

        t.pop();
    }

    return EXIT_SUCCESS;
}
