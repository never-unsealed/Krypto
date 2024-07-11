#ifndef _BlockCipher_h_
#define _BlockCipher_h_

#include <iostream>
#include <string>
#include "byte.h"

using namespace std;

/**
 * @class A class for classic ciphers.
 * @author Christoph Karg
 *
 * This class is a base class for block ciphers.
 */
class BlockCipher {
protected:
  /** Block length of the cipher in bytes. */
  unsigned int block_len;

public: 
  /**
   * Constructor.
   */
  BlockCipher(unsigned int in_block_len=8);

  /**
   * Decrypt a cipher text.
   *
   * @param cipher_text array to store cipher text
   * @param cipher_len length of the cipher text array
   * @param key key
   * @param key_len key length
   * @param plain_text plain text to be decrypted
   * @param plain_len length of the plain text
   *
   * @return number of decrypted bytes
   */
  virtual int decrypt(const byte* cipher_text, int cipher_len, 
                      const byte* key, int key_len,
                      byte* plain_text, int plain_len)=0;

   /**
   * Encrypt a plain text.
   *
   * @param plain_text plein text to be encrypted
   * @param plain_len length of the plain text
   * @param key key
   * @param key_len key length
   * @param cipher_text array to store cipher text
   * @param cipher_len length of the cipher text array
   *
   * @return number of encrypted bytes
   */
  virtual int encrypt(const byte* plain_text, int plain_len, 
                      const byte* key, int key_len,
                      byte* cipher_text, int cipher_len)=0;


  /**
   * Convert an ascii character containg an hex digit to a byte.
   *
   * @param xdigit digit to convert
   *
   * @return result of convert operation
   */
  byte hexToByte(byte xdigit);

  /**
   * Read a stream and store the characters in an array.
   *
   * @param strm stream to read
   * @param in_text pointer to the 
   * @param hex_mode stream is interpreted as hex encoding
   * @param blender character to use for block padding
   *
   * @return number of read bytes
   */
  int readStream(istream& strm, byte*& in_text,
                 bool hex_mode, unsigned char blender='\0');

  /**
   * Write a byte array to a stream.
   *
   * @param strm stream to write
   * @param out_text pointer to the storage area
   * @param out_len length of the text array
   * @param hex_mode output is hex encoded 
   * @param columns number of columns per line (for hex encoding)
   *
   * @return number of  bytes written on stream
   */
  int writeStream(ostream& strm, byte* out_text, int out_len, 
                  bool hex_mode, int columns=30);

}; // BlockCipher

#endif
