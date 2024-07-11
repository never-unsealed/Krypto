#ifndef AES_AESMATH_H
#define AES_AESMATH_H

#include <string>
#include <vector>
#include "datatypes.h"

using namespace std;

class AESMath {
private:
    vector<byte> log_table;
    vector<byte> exp_table;
    vector<byte> sbox;
    vector<byte> inv_sbox;

public:
    AESMath();
    static byte add(byte a, byte b);
    byte exp(byte i) const;
    byte inv(byte b) const;
    byte log(byte i) const;
    byte mul(byte a, byte b) const;
    byte sBox(byte b) const;
    byte invSBox(byte b) const;
    static byte atrans(byte x);
    static byte rpmul(byte a, byte b);
    static byte parity(byte b);
    static void printTable(const vector<byte>& table);
    static byte xtime(byte b);

    static string format(byte b);
};


#endif //AES_AESMATH_H
