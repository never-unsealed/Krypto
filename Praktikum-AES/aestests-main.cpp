#include <iostream>
#include <iomanip>
#include "AESMath.h"
#include "AESState.h"
#include "AESKeySchedule.h"
#include "AESCipher.h"

using namespace std;

void aesMathTest() {
    AESMath aesmath;

    byte x = 0x63;

    cout << "parity(" << AESMath::format(x) << "): " << dec << (unsigned) AESMath::parity(x) << endl;

    cout << endl;
}

void mulTestold() {
    cout << "mult() Test" << endl;
    cout << "===========" << endl << endl;

    AESMath aesmath;
    byte a = 0x57;
    byte b = 0x13;

    cout << AESMath::format(a) << " * " << AESMath::format(b) << " = " << AESMath::format(aesmath.mul(a, b)) << endl;

    a = 0x13;
    b = 0x57;
    cout << AESMath::format(a) << " * " << AESMath::format(b) << " = " << AESMath::format(aesmath.mul(a, b)) << endl;

    a = 0x83;
    b = 0xac;
    cout << AESMath::format(a) << " * " << AESMath::format(b) << " = " << AESMath::format(aesmath.mul(a, b)) << endl;
}


void invTest() {
    cout << "inv() Test" << endl;
    cout << "==========" << endl << endl;

    AESMath aesmath;

    vector<byte> v = { 0x57, 0xf9, 0xab, 0x8e, 0x65, 0x01, 0x91, 0x4b, 0x02, 0xff, 0x09, 0xed, 0xba };

    for (size_t i=0; i<v.size(); i++) {
        byte a = v[i], a_inv = aesmath.inv(a), c = aesmath.mul(a, a_inv);
        cout << AESMath::format(a) << " & " << AESMath::format(a_inv);
        cout << " \\\\ \\hline %  " << (int) c << endl;
    }

}

void parityTest() {
    cout << "parity() Test" << endl;
    cout << "=============" << endl << endl;

    AESMath aesmath;

    vector<byte> v = { 0x00, 0x01, 0x05, 0x1f, 0x43, 0xea, 0xff, 0xf1 };

    for (size_t i=0; i<v.size(); i++) {
        byte a = v[i], a_par = aesmath.parity(a);
        cout << AESMath::format(a) << " & " << AESMath::format(a_par);
        cout << " \\\\ \\hline %  " << endl;
    }

}


void atransTest() {
    cout << "atrans() Test" << endl;
    cout << "=============" << endl << endl;

    AESMath aesmath;

    vector<byte> v = { 0x00, 0x81, 0x75, 0xab, 0x4e, 0xea, 0xc6, 0xff };

    for (size_t i=0; i<v.size(); i++) {
        byte a = v[i], a_trans = aesmath.atrans(a);
        cout << AESMath::format(a) << " & " << AESMath::format(a_trans);
        cout << " \\\\ \\hline %  " << endl;
    }

}


void mulTest() {
    cout << "mult() Test" << endl;
    cout << "===========" << endl << endl;

    AESMath aesmath;

    byte a, b, m, rpm;
    unsigned int cntr = 0;

    a=0;
    for (size_t i=0; i<256; i++) {
        b=0;
        for (size_t j=0; j<256; j++) {
            m = aesmath.mul(a,b);
            rpm = aesmath.rpmul(a,b);
            if (m!=rpm) {
                cout << "Problem: a = " << AESMath::format(a) << ", b = " << AESMath::format(b);
                cout << ": mul = " << AESMath::format(m) << ", rpmul = " << AESMath::format(rpm) << endl;
                cntr++;
            }
            b++;
        }
        a++;
    }
    cout << endl << cntr << " wrong values" << endl;
}

void mulTest2() {
    cout << "rpmul() Test:" << endl;
    cout << "=============" << endl << endl;

    vector<byte> a = { 0x47, 0x00, 0x67, 0xfa, 0x81 };
    vector<byte> b = { 0x59, 0x91, 0x00, 0xae, 0x42 };

    for (size_t i=0; i<a.size(); i++) {
        byte m = AESMath::rpmul(a[i], b[i]);
        cout << AESMath::format(a[i]) << " & " << AESMath::format(b[i]) << " & "
        << AESMath::format(m) << "\\\\ %" << endl;
    }
}

void xtimeTest() {
    cout << "xtime() Test" << endl;
    cout << "============" << endl << endl;

    byte x = 0x57;
    byte y = AESMath::xtime(x);
    cout << "xtime(" << AESMath::format(x) << ") = " << AESMath::format(y) << endl;
    x = y;
    y = AESMath::xtime(x);
    cout << "xtime(" << AESMath::format(x) << ") = " << AESMath::format(y) << endl;
    x = y;
    y = AESMath::xtime(x);
    cout << "xtime(" << AESMath::format(x) << ") = " << AESMath::format(y) << endl;
    x = y;
    y = AESMath::xtime(x);
    cout << "xtime(" << AESMath::format(x) << ") = " << AESMath::format(y) << endl;

    cout << endl;

    vector<byte> v = { 0x57, 0xf9, 0xab, 0x8e, 0x65, 0x01, 0x91, 0x4b };

    for (size_t i=0; i<v.size(); i++) {
        cout << AESMath::format(v[i]) << " & " <<  AESMath::format(AESMath::xtime(v[i]))
        << " \\\\ %" << endl;
    }

}

void generator() {
    cout << "generator()" << endl;
    cout << "===========" << endl << endl;

    AESMath aesmath;
    byte x = 0x03;
    int counter = 1;
    do {
        cout << AESMath::format(x) << " (" << AESMath::format(aesmath.exp(counter)) << ")" << endl;
        x = AESMath::rpmul(x, 0x03);
        counter++;
    } while (x != 1);
    cout << AESMath::format(x) << " (" << AESMath::format(aesmath.exp(counter)) << ")" << endl;
    cout << "Anzahl: " << counter << " Elemente" << endl;

    cout << endl;
}

void inverseElements() {
    cout << "inverseElements():" << endl;
    cout << "==================" << endl << endl;

    AESMath aesmath;
    byte b = 1;

    for (size_t i = 1; i <= 255; i++) {
        cout << "inv(" << AESMath::format(b) << ") = " << AESMath::format(aesmath.inv(b));
        cout << " Check: " << AESMath::format(aesmath.mul(b, aesmath.inv(b))) << endl;
        b++;
    }
}

void sboxTest() {
    cout << endl;
    cout << "SBox Test():" << endl;
    cout << "============" << endl << endl;

    AESMath aesmath;
    vector<byte> sbox;
    vector<byte> inv_sbox(256, 0);

    byte x = 0, x_inv, y;
    for (size_t i = 0; i < 256; i++) {
        if (x != 0) {
            x_inv = aesmath.inv(x);
        } else {
            x_inv = 0;
        }
        cout << "x = " << AESMath::format(x) << ", x_inv = " << AESMath::format(x_inv)
             << " Check: " << AESMath::format(aesmath.rpmul(x, x_inv)) << endl;
        y = AESMath::atrans(x_inv);

        sbox.push_back(y);
        inv_sbox[y] = x;
        x++;
    }

    cout << endl;
    cout << "AESCipher SBox" << endl;
    cout << "========" << endl << endl;
    AESMath::printTable(sbox);

    cout << endl;
    cout << "Inverse AESCipher SBox" << endl;
    cout << "================" << endl << endl;
    AESMath::printTable(inv_sbox);
}

void stateTest() {
    cout << endl;
    cout << "State Test():" << endl;
    cout << "=============" << endl << endl;

    //byte pt[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    //             0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    byte pt[] = {0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11,
                 0x22, 0x22, 0x22, 0x22, 0x33, 0x33, 0x33, 0x33};

    vector<byte> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    byte ct[16];
    bool debug_mode=true;
    AESMath aesmath;
    AESState state(aesmath, debug_mode);
    AESKeySchedule ks(aesmath, debug_mode);

    ks.setKey(key);
    state.set(pt);

    cout << "State: " << state.format() << endl;
    state.shiftRows();
    cout << "State: " << state.format() << endl;
    state.invShiftRows();
    cout << "State: " << state.format() << endl;
    cout << endl;

    cout << "State: " << state.format() << endl;
    state.subBytes();
    cout << "State: " << state.format() << endl;
    state.invSubBytes();
    cout << "State: " << state.format() << endl;
    cout << endl;

    cout << "State: " << state.format() << endl;
    state.mixColumns();
    cout << "State: " << state.format() << endl;
    state.invMixColumns();
    cout << "State: " << state.format() << endl;
    cout << endl;

    cout << "State: " << state.format() << endl;
    state.addKey(ks.getRoundKey(0));
    cout << "State: " << state.format() << endl;
    state.addKey(ks.getRoundKey(0));
    cout << "State: " << state.format() << endl;

    state.get(ct);

}

void keyScheduleTest128() {
    cout << endl;
    cout << "Key Schedule Test (128 Bit Key):" << endl;
    cout << "================================" << endl << endl;

    vector<byte> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                  0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    AESMath aesmath;
    AESKeySchedule key_schedule(aesmath, true);

    key_schedule.setKey(key);

}

void keyScheduleTest192() {
    cout << endl;
    cout << "Key Schedule Test (192 Bit Key):" << endl;
    cout << "================================" << endl << endl;

    vector<byte> key = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52,
                  0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
                  0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};

    AESMath aesmath;
    AESKeySchedule key_schedule(aesmath, true);

    key_schedule.setKey(key);

}

void keyScheduleTest256() {
    cout << endl;
    cout << "Key Schedule Test (256 Bit Key):" << endl;
    cout << "================================" << endl << endl;

    vector<byte> key = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                  0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                  0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                  0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};

    AESMath aesmath;
    AESKeySchedule key_schedule(aesmath, true);

    key_schedule.setKey(key);

}

void mixColumnsTest() {
    cout << endl;
    cout << "MixColumns Test:" << endl;
    cout << "================" << endl << endl;

    AESMath aesmath;
    AESState state(aesmath, true);

    byte s[] = {0x63, 0x53, 0xe0, 0x8c,
                0x09, 0x60, 0xe1, 0x04,
                0xcd, 0x70, 0xb7, 0x51,
                0xba, 0xca, 0xd0, 0xe7};

    state.set(s);
    cout << state.format() << endl;
    state.mixColumns();
    cout << state.format() << endl;

}

void shiftRowsTest() {
    cout << endl;
    cout << "ShiftRows Test:" << endl;
    cout << "===============" << endl << endl;

    AESMath aesmath;
    AESState state(aesmath, true);

    byte s[] = {0x63, 0x53, 0xe0, 0x8c,
                0x09, 0x60, 0xe1, 0x04,
                0xcd, 0x70, 0xb7, 0x51,
                0xba, 0xca, 0xd0, 0xe7};

    state.set(s);
    cout << state.format() << endl;
    state.shiftRows();
    cout << state.format() << endl;

}

void subBytesTest() {
    cout << endl;
    cout << "SubBytes Test:" << endl;
    cout << "==============" << endl << endl;

    AESMath aesmath;
    AESState state(aesmath, true);

    byte s[] = {0x63, 0x53, 0xe0, 0x8c,
                0x09, 0x60, 0xe1, 0x04,
                0xcd, 0x70, 0xb7, 0x51,
                0xba, 0xca, 0xd0, 0xe7};

    state.set(s);
    cout << state.format() << endl;
    state.subBytes();
    cout << state.format() << endl;

}

void aesTest() {
    cout << endl;
    cout << "AESCipher Test:" << endl;
    cout << "===============" << endl << endl;

    byte plain_text[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    vector<byte> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    byte cipher_text[16];

    bool debug_mode=true;
    AESCipher aes(debug_mode);

    aes.setKey(key);
    cout << endl;
    cout << "Encryption" << endl;
    cout << "==========" << endl << endl;

    aes.encryptBlock(plain_text, cipher_text);

    cout << endl;
    cout << "Decryption" << endl;
    cout << "==========" << endl << endl;
    aes.decryptBlock(cipher_text, plain_text);

}

void aesTest2() {
    cout << endl;
    cout << "AESCipher Encryption Test:" << endl;
    cout << "==========================" << endl << endl;

    vector<byte> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    vector<byte> plain_text;
    vector<byte> cipher_text;
    vector<byte> result;

    string msg("Dies ist ein Test mit einem langen Satz!!!!");
    plain_text = AESCipher::toVector(msg);

    bool debug_mode=true;
    AESCipher aes(debug_mode);

    aes.setKey(key);
    aes.process(plain_text, cipher_text, false);
    aes.process(cipher_text, plain_text, true);

    string msg2 = AESCipher::toString(plain_text);

    cout << msg2 << endl;
}

void hexEncoding() {
    cout << endl;
    cout << "Hex Encoding Test:" << endl;
    cout << "==================" << endl << endl;

    vector<byte> v = { 0x01, 0x34, 0x11, 0x89, 0xfa, 0xa5, 0x7b };
    vector<byte> v2;
    string s;

    s = BlockCipher::toHexString(v);

    cout << "Hex-String: " << s << endl;


}


int main() {
    aesMathTest();
    xtimeTest();
    mulTest();
    mulTest2();
    invTest();
    parityTest();
    atransTest();
    generator();
    inverseElements();
    sboxTest();

    stateTest();

    keyScheduleTest128();
    keyScheduleTest192();
    keyScheduleTest256();

    hexEncoding();
    mixColumnsTest();
    subBytesTest();
    shiftRowsTest();
    aesTest();
    aesTest2();

    return 0;
}
