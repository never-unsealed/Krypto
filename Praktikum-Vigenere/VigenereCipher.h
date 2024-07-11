#ifndef _VigenereCipher_h_
#define _VigenereCipher_h_

#include "ClassicCipher.h"

class VigenereCipher : public ClassicCipher {
public:
    virtual int decrypt(const vector<byte>& cipher_text,
                      const vector<byte>&  key,
                      vector<byte>& plain_text);

  virtual int encrypt( const vector<byte>& plain_text,
                       const vector<byte>& key, 
                       vector<byte>& cipher_text);

}; // VigenereCipher

#endif
