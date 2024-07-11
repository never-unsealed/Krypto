/*
 * Toolbox.cpp
 */

#include <cassert>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include "Toolbox.h"

Toolbox::Toolbox() {
}

Toolbox::~Toolbox() {
}

string Toolbox::prune(const string &s) {
    string r;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i]) || isdigit(s[i])) {
            r.push_back(s[i]);
        }
    }
    return r;
}

string Toolbox::format(const string &s, unsigned short columns,
                       unsigned short blocklen, bool uppercase) {
    string sp = prune(s);
    string t;

    for (int i = 0; i < sp.length(); i++) {
        if (uppercase == true) {
            t.push_back(toupper(sp[i]));
        } else {
            t.push_back(tolower(sp[i]));
        }
    }
    ostringstream out;

    if (columns == 0) {
        columns = 8;
    }
    if (blocklen == 0) {
        blocklen = 5;
    }

    vector<string> blocks;

    for (int i = 0; i < t.length(); i += blocklen) {
        blocks.push_back(t.substr(i, blocklen));
    }

    unsigned short cntr = 0, col_cntr = 0;

    while (cntr < blocks.size()) {

        out << blocks[cntr];
        cntr++;
        col_cntr++;

        if (col_cntr == columns) {
            out << endl;
            col_cntr = 0;
        }
        else {
            out << " ";
        }
    }

    return out.str();
}

string Toolbox::readFromStream(istream &strm) {
    string line;
    string data;
    char c;

    while (strm.eof() == false) {
        //strm >> line;
        strm.get(c);
        //if (isalpha(c) || isdigit(c)) {
            data.push_back((char) tolower(c));
        //}
    }

    return data;
}

string Toolbox::toLower(const string &s) {
    string r("");

    for (int i = 0; i < s.length(); i++) {
        r = r + (char) tolower(s[i]);
    }

    return r;
}

string Toolbox::toUpper(const string &s) {
    string r("");

    for (int i = 0; i < s.length(); i++) {
        r = r + (char) toupper(s[i]);
    }

    return r;
}

bool Toolbox::readFromFile(const string &filename, string &output) {
    ifstream strm;

    strm.open(filename);
    if (strm.is_open()) {
        output = Toolbox::readFromStream(strm);
        strm.close();
        return true;
    } else {
        return false;
    }
}

bool Toolbox::writeToFile(const string &filename, const string &s) {
    ofstream strm;

    strm.open(filename);
    if (strm.is_open()) {
        strm << s;
        strm.close();
        return true;
    } else {
        return false;
    }
}
