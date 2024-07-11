//
// Created by Christoph Karg on 12.11.18.
//

#ifndef AES_AESSTATE_H
#define AES_AESSTATE_H

#include <cassert>
#include <string>
#include "datatypes.h"
#include "AESMath.h"

using namespace std;

class AESState {
private:
    bool debug_mode;
    const AESMath *math;

protected:
    byte state[16];

    byte getCell(size_t row, size_t col);
    void setCell(size_t row, size_t col, byte b);
    void shiftRow(size_t row, size_t shift);

public:
    AESState(const AESMath& aesmath, bool debug_mode=false);

    void set(const byte* in);
    void get(byte* out);

    void shiftRows();
    void subBytes();
    void mixColumns();
    void invShiftRows();
    void invSubBytes();
    void invMixColumns();
    void addKey(const word* key);

    string format() const;
};


#endif //AES_AESSTATE_H
