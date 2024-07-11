#include <cassert>
#include "VigenereCipher.h"

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

/**
 * @brief Entschluesselt einen Text mit dem Vigenere-Cipher.
 *
 * Die decrypt-Methode entschluesselt einen Text anhand des 
 * angegebenen Schluessels. Buchstaben in den entsprechenden
 * Eingabe- und Ausgabevektoren werden nicht als ASCII Werte 
 * repraesentiert, sondern
 * Zahlen im Bereich 0 bis 26.
 * @param cipher_text Der verschluesselte Ciphertext.
 * @param key Der Schluessel zum entschluesseln des Textes.
 * @param pain_text Erhaelt den entschluesselten Klartext.
 * @return Die Laenge des Klartextes in Bytes.
 */
int VigenereCipher::decrypt
        (
                const vector<byte> &cipher_text,
                const vector<byte> &key,
                vector<byte> &plain_text) {
    /*************************************************************************
     * Aufgabe 2.
     *************************************************************************/

    UINT8 currentDecrypted, currentKey;

    plain_text.clear();

    for (UINT32 i = 0; i < cipher_text.size(); i++)
    {
        currentKey = key[i % key.size()];

        currentDecrypted = ((cipher_text[i] + 26) - currentKey) % 26;

        plain_text.push_back(currentDecrypted);
    }

    return plain_text.size();
} 

/**
 * @brief Verschluesselt einen Text mit dem Vigenere-Cipher.
 *
 * Die encrypt-Methode verschluesselt einen Text anhand des
 * angegebenen Schluessels. Buchstaben in den entsprechenden
 * Eingabe- und Ausgabevektoren werden nicht als ASCII Werte
 * repraesentiert, sondern als Zahlen im Bereich 0 bis 26.
 * @param pain_text Der zu verschluesselnde Klartext.
 * @param key Der Schluessel zum verschluesseln des Textes.
 * @param cipher_text Erhaelt den verschluesselten Ciphertext.
 * @return Die Laenge des Ciphertextes in Bytes.
 */
int VigenereCipher::encrypt
        (
                const vector<byte> &plain_text,
                const vector<byte> &key,
                vector<byte> &cipher_text
        ) {
    /*************************************************************************
     * Aufgabe 2.
     *************************************************************************/

    UINT8 currentKey, currentEncrypted;
    
    cipher_text.clear();

    for (UINT32 i = 0; i < plain_text.size(); i++)
    {
        currentKey = key[i % key.size()];

        currentEncrypted = (plain_text[i] + currentKey) % 26;

        cipher_text.push_back(currentEncrypted);
    }

    return cipher_text.size();
}


