#ifndef _ClassicCipher_h_
#define _ClassicCipher_h_

#include <iostream>
#include <string>
#include <vector>
#include "byte.h"

using namespace std;

/**
 * @class A class for classic ciphers.
 * @author Christoph Karg
 *
 * This class is a base class for classic ciphers such as affine or vigenere
 * cipher.
 */
class ClassicCipher {
public: 
  /**
   * Decrypt a cipher text.
   *
   * @param cipher_text to be decrypted
   * @param key key
   * @param plain_text vector to store the resulting plain text
   *
   * @return number of decrypted bytes
   */
  virtual int decrypt(const vector<byte>& cipher_text, 
                      const vector<byte>&  key,
                      vector<byte>& plain_text)=0;

   /**
   * Encrypt a plain text.
   *
   * @param plain_text plain text to be encrypted
   * @param plain_len length of the plain text
   * @param key key
   * @param key_len key length
   * @param cipher_text array to store cipher text
   * @param cipher_len length of the cipher text array
   *
   * @return number of encrypted bytes
   */
  virtual int encrypt(const vector<byte>& plain_text,
                      const vector<byte>& key, 
                      vector<byte>& cipher_text)=0;


  /**
   * Read a stream and store the characters in an STL vector.
   *
   * @param strm stream to read
   * @param out_text storage area
   * @param block_len block length
   * @param blender character to use for block padding
   *
   * @return number of read bytes
   */
  static int readStream(istream& strm, vector<byte>& out_text,
                 int block_len=1, unsigned char blender='x');

  /**
   * Write a byte array to a stream.
   *
   * @param strm stream to write
   * @param in_text text to be written
   * @param c_shift character to shift output
   * @param columns number of columns per line
   *
   * @return number of written bytes
   */
  static int writeStream(ostream& strm, const vector<byte>& in_text,
                  unsigned char c_shift='a', int columns=50);

};

#endif
