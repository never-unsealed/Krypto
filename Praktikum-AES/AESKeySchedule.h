//
// Created by Christoph Karg on 14.11.18.
//

#ifndef AES_AESKEYSCHEDULE_H
#define AES_AESKEYSCHEDULE_H

#include <string>
#include <vector>
#include "datatypes.h"
#include "AESMath.h"
#include "AESState.h"

using namespace std;

class AESKeySchedule {
private:
    bool debug_mode;
    const AESMath *math;
    //word *key_schedule;
    vector<word> key_schedule;
    size_t nk;
    size_t nr;
    size_t nb;
    vector<word> r_con;

public:
    static word rotWord(word w);
    word subWord(word w) const;

    AESKeySchedule(const AESMath &aesmath, bool debug_mode=false);
    virtual ~AESKeySchedule();

    //void addRoundKey(AESState& state, size_t round);
    bool setKey(const vector<byte>& key);

    const word* getRoundKey(size_t round);

    string formatRoundKey(size_t round);

    size_t getNrOfRounds() const;

};


#endif //AES_AESKEYSCHEDULE_H
