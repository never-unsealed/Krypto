#ifndef _AESCIPHER_H
#define _AESCIPHER_H

#include <string>
#include <vector>
#include "AESMath.h"
#include "AESState.h"
#include "AESKeySchedule.h"
#include "BlockCipher.h"

using namespace std;

class AESCipher : public BlockCipher {
private:
    bool debug_mode;
    AESMath aesmath;
    AESKeySchedule key_schedule;
    AESState state;

    void debugMessage(size_t round, string msg);

public:

    static const bool Encryption=false;
    static const bool Decryption=true;

    AESCipher(bool debug_mode=false);

    bool setKey(const vector<byte>& key);

    void encryptBlock(const byte *plain_text, byte *cipher_text);

    void decryptBlock(const byte *cipher_text, byte *plain_text);

    bool process(const vector<byte>& in, vector<byte>& out, bool mode);

    bool encrypt(const vector<byte>& plain_text, vector<byte>& cipher_text);

    bool decrypt(const vector<byte>& cipher_text, vector<byte>& plain_text);

    static vector<byte> toVector(const string& msg, size_t block_len=16);

    static string toString(const vector<byte>& vec);
};


#endif
