//
// Created by Christoph Karg on 2019-08-18.
//

#ifndef ANGEWANDTEKRYPTOGRAPHIE_POLYBIUSCIPHER_H
#define ANGEWANDTEKRYPTOGRAPHIE_POLYBIUSCIPHER_H

#include <map>
#include <iostream>
#include <string>

using namespace std;

class PolybiusCipher {
protected:
    map<string,string> encrypt_map;
    map<string,string> decrypt_map;

public:
    PolybiusCipher();
    virtual ~PolybiusCipher();

    bool setKey(string key);
    void printKey() const;
    static string randomKey();

    string decrypt(string cipher_text) const;
    string encrypt(string plain_text) const;
};


#endif //ANGEWANDTEKRYPTOGRAPHIE_POLYBIUSCIPHER_H
