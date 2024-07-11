#ifndef _BlockCipher_h_
#define _BlockCipher_h_

#include <iostream>
#include <string>
#include <vector>
#include "datatypes.h"

using namespace std;

class BlockCipher {
protected:
    unsigned int block_len;

public:
    BlockCipher(unsigned int in_block_len = 8);

    virtual bool decrypt(const vector<byte> &plain_text,
                         vector<byte> &cipher_text) = 0;

    virtual bool encrypt(const vector<byte> &plain_text,
                         vector<byte> &cipher_text) = 0;

    virtual bool setKey(const vector<byte> &key) = 0;

    static byte hexToByte(byte xdigit);

    static bool readStream(istream &strm, vector<byte> &data,
                           bool hex_mode);

    static void writeStream(ostream &strm, const vector<byte> &data,
                            bool hex_mode, int columns = 30);

    static bool hexStringToVector(string s, vector<byte> &data);

    static string toHexString(const vector<byte> &data);
};

#endif
