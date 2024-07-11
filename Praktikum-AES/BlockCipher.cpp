/*!
 * \file
 */

#include "BlockCipher.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <vector>


// #BlockCipher()
BlockCipher::BlockCipher(unsigned int in_block_len)
        : block_len(in_block_len) {
    assert(block_len > 0);
} // BlockCipher()


// #hexToByte()
byte BlockCipher::hexToByte(byte xdigit) {
    assert(isxdigit(xdigit) > 0);

    if (isxdigit(xdigit) > 0) {
        if (isdigit(xdigit) > 0) {
            return (xdigit - '0');
        } // if
        else {
            return (tolower(xdigit) - 'a') + 10;
        } // else
    } // if
    else {
        return 0;
    } // else

} // hexToByte()


bool BlockCipher::readStream(
        istream &strm, vector<byte> &data,
        bool hex_mode
) {
    char c;
    byte b;
    int len, i;
    vector<byte> input_vec;

    /*
     * Read data from input stream.
     */
    strm.get(c);
    while (strm.eof() == false) {
        if (hex_mode == true) {
            if (isxdigit(c) > 0) {
                input_vec.push_back(c);
            }
        } else {
            input_vec.push_back(c);
        }
        strm.get(c);
    }

    if ((hex_mode == true) && (input_vec.size() % 2 == 1)) {
        return false;
    }

    i = input_vec.size();
    data.clear();
    i = 0;
    while (i < input_vec.size()) {
        if (hex_mode == true) {
            b = hexToByte(input_vec[i]);
            i++;
            b <<= 4;
            b += hexToByte(input_vec[i]);
            i++;
        } else {
            b = input_vec[i];
            i++;
        }
        data.push_back(b);
    }

    return true;

}

void BlockCipher::writeStream(ostream &strm, const vector<byte> &data,
                              bool hex_mode, int columns) {
    int i, j;

    j = 0;
    for (i = 0; i < data.size(); i++) {
        if (hex_mode == true) {
            strm << hex << setw(2) << setfill('0') << (int) (data[i]);
            j++;
            if (j >= columns) {
                strm << endl;
                j = 0;
            }
        } else {
            strm << (unsigned char) (data[i]);
        }
    }
}

bool BlockCipher::hexStringToVector(string s, vector<byte> &data) {
    size_t i;
    byte b;

    if (s.size() % 2 == 1) {
        return false;
    } else {
        data.clear();
        i = 0;
        while (i < s.size()) {
            b = hexToByte(s[i]);
            i++;
            b <<= 4;
            b += hexToByte(s[i]);
            i++;

            data.push_back(b);
        }
        return true;
    }
}

string BlockCipher::toHexString(const vector<byte>& data) {
    ostringstream os;

    for (size_t i=0; i<data.size(); i++) {
        os << setw(2) << setfill('0') << hex << (unsigned int)data[i];
    }
    return os.str();
}

