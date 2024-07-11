#include <cctype>
#include <sstream>
#include <fstream>
#include <random>
#include <ctime>
#include "PolybiusCipher.h"

PolybiusCipher::PolybiusCipher() : decrypt_map(), encrypt_map() {
  /****************************************************************************
   * Aufgabe 9.
   ****************************************************************************/
}

PolybiusCipher::~PolybiusCipher() {
}


bool PolybiusCipher::setKey(string key) {
  /****************************************************************************
   * Aufgabe 8.
   ****************************************************************************/

  return false;
}

void PolybiusCipher::printKey() const {
  /****************************************************************************
   * Aufgabe 10.
   ****************************************************************************/

}


string PolybiusCipher::randomKey() {
    string key = "0123456789abcdefghijklmnopqrstuvwxyz";
    default_random_engine generator(time(NULL));
    uniform_int_distribution<int> distribution(0,key.size()-1);

    for (size_t i=0; i<key.size(); i++) {
        swap(key[i],key[distribution(generator)]);
    }

    return key;
}


string PolybiusCipher::encrypt(string plain_text) const {
  /****************************************************************************
   * Aufgabe 12a.
   ****************************************************************************/
    string cipher_text;

    return cipher_text;
}

string PolybiusCipher::decrypt(string cipher_text) const {
  /****************************************************************************
   * Aufgabe 12b.
   ****************************************************************************/
    string plain_text;

    return plain_text;
}

