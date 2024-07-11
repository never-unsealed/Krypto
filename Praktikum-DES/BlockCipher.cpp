/*!
 * \file
 */
#include "BlockCipher.h"
#include <cassert>
#include <iomanip>
#include <vector>


// #BlockCipher()
BlockCipher::BlockCipher(unsigned int in_block_len) 
: block_len(in_block_len)
{
  assert(block_len>0);
} // BlockCipher()


// #hexToByte()
byte BlockCipher::hexToByte(byte xdigit) {
  assert(isxdigit(xdigit)>0);

  if (isxdigit(xdigit)>0) {
    if (isdigit(xdigit)>0) {
      return (xdigit-'0');
    } // if
    else {
      return (tolower(xdigit)-'a')+10;
    } // else
  } // if
  else {
    return 0;
  } // else

} // hexToByte()


// #readStream()
int BlockCipher::readStream
(
 istream& strm, 
 byte*& in_text, 
 bool hex_mode,
 unsigned char blender
) 
{
  char c;
  byte b, low_n, high_n;
  int len, i;
  vector<byte> input_vec;
  vector<byte>::iterator v_it;

  /*
   * Read data from input stream.
   */
  strm.get(c);
  while (strm.eof()==false) {
    if (hex_mode==true) {
      if (isxdigit(c)>0) {
        input_vec.push_back(c);
      } // if
    } // if
    else {
      input_vec.push_back(c);
    } // else
    strm.get(c);
  } // while

  // Hex encoding must be of even size!
  if ((hex_mode==true) && (input_vec.size() % 2 ==1)) {
    input_vec.push_back('0');
  } // if

  /*
   * Allocate memory on heap.
   */
  i = input_vec.size();
  if (hex_mode==true) {
    i = i / 2;
  } // if
  len = (i / block_len) * block_len;
  if (len<i) {
    len = len + block_len;
  } // if
  in_text=new byte[len];

  /*
   * Copy data into allocated array and convert it if necessary.
   */
  i=0;
  v_it=input_vec.begin();
  while (v_it!=input_vec.end()) {
    if (hex_mode==true) {
      b = hexToByte(*v_it);
      v_it++;
      b <<= 4;
      b += hexToByte(*v_it);
      v_it++;
    } // if
    else {
      b = *v_it;
      v_it++;
    } // else
    in_text[i]=b;
    i++;
  } // while

  while (i<len) {
    in_text[i]=blender;
    i++;
  } // while

  input_vec.clear();

  return len;
  
} // readStream()


// #writeStream()
int BlockCipher::writeStream
(
 ostream& strm, 
 byte* out_text, 
 int out_len, 
 bool hex_mode,
 int columns
) 
{
  int i, j, sum;

  j=0; sum=0;
  for (i=0; i<out_len; i++) {
    if (hex_mode==true) {
      strm << hex << setw(2) << setfill('0') << (int)(out_text[i]);
      j++; sum+=2;
      if (j >= columns) {
        strm << endl;
        j=0;
      } // if
    } // if
    else {
      strm << (unsigned char)(out_text[i]);
      sum++;
    } // else

  } // for


  return sum;
  
} // writeStream()
