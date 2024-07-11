#ifndef VigenereBreaker_h_
#define VigenereBreaker_h_

#include <vector>
#include "VigenereCipher.h"

class VigenereBreaker : public VigenereCipher {
protected:

  /**
   * Compute greatest common divisor of two integers a and b
   * 
   * @param a first integer
   * @param b second integer
   *
   * @return gcd(a,b)
   */
  int gcd(int a, int b) const;

  /** 
   * Compute the greatest common divisor of a vector of integers.
   *
   * The vector must be of size > 0.
   *
   * @param v vector containing at least 1 integer
   *
   * @return greatest common divisor of all integers in v
   */
  int gcd(const vector<int>& v) const;

public:

  /**
   * Perform Kasiski test to detect key length
   *
   * @param cipher_text cipher text to be analysed
   * @param ngram_len length of the n-grams to be checked
   * @param verbose Print additional information to stdout.
   *
   * @return key length (0 = no length computed)
   */
  int kasiskiTest(const vector<byte>& cipher_text,
                  int ngram_len=3,
                  bool verbose=false);

  /**
   * Compute the conincidence index for a given text
   *
   * @param text text to be analysed
   *
   * @return coincidence index of text
   */
  float coincidenceIndex(const vector<byte>& text);

  /**
   * Compute the coincidence indices of a given number of
   * columns and check whether all values lie above a 
   * given threshold
   *
   * @param cipher_text cipher_text to be analysed
   * @param cols number of columns
   * @param threshold thershold value
   * @param verbose Print additional information to stdout.
   *
   * @return true <=> indices of all columns are >= threshold
   */
  bool coincidenceTest(const vector<byte>& cipher_text,
                       int cols, 
                       float threshold=0.065,
                       bool verbose=false);

  /**
   * Compute the mutual coincidence index table of two columns
   * and return the relative shift g such that key[j] = key[i] - g
   * if the respective index is >= threshold
   *
   * @param cipher_text cipher_text to be analysed
   * @param cols number of columns
   * @param col_i first column
   * @param col_j second column
   * @param threshold thershold value
   * @param verbose Print additional information to stdout.
   *
   * @return shift g between key[i] and key[j], if respective index 
   * is >= threshold. -1, otherwise.
   */
  int mutualCoinIndex(const vector<byte>& cipher_text,
                      int cols, 
                      int i,
                      int j,
                      float threshold=0.065,
                      bool verbose=false);

  /**
   * Analyse a given cipher text.
   * @param cipher_text cipher text to be analysed
   */
  void analyse(const vector<byte>& cipher_text);

};

#endif
