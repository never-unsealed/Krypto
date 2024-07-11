#include "PublicKeyAlgorithmBox.h"
#include "RSAEncryptor.h"

/**
 * @brief Konstruktor für den RSA-Encryptor.
 *
 * Dieser Konstruktor initialisiert einen RSA-Encryptor mit den gegebenen Parametern n und e.
 * Diese Parameter werden gesetzt, um später die Verschlüsselung mit dem RSA-Algorithmus durchzuführen.
 *
 * @param n Der öffentliche Modulus für die Verschlüsselung.
 * @param e Der öffentliche Exponent für die Verschlüsselung.
 */
RSAEncryptor::RSAEncryptor(const Integer &n, const Integer &e)
{
  /******************************************************************************
   * Aufgabe 16a
   ******************************************************************************/

  // Setzen der Klassenvariablen
  this->n = n;
  this->e = e;
}

RSAEncryptor::~RSAEncryptor() {}

/**
 * @brief Verschlüsselt eine gegebene Ganzzahl x mittels RSA-Verschlüsselung mit dem öffentlichen Schlüssel (e, n).
 * 
 * Diese Funktion berechnet die RSA-Verschlüsselung der Ganzzahl x unter Verwendung des öffentlichen Schlüssels (e, n).
 * Sie verwendet modulare Exponentiation für eine effiziente Berechnung.
 * 
 * @param x Die Eingabeganzahl, die verschlüsselt werden soll.
 * @param y Ausgabeparameter, in dem das verschlüsselte Ergebnis gespeichert wird.
 * @return True, wenn x < n, ansonsten false.
 */
bool RSAEncryptor::compute(const Integer &x, Integer &y) const
{
  /******************************************************************************
   * Aufgabe 16b
   ******************************************************************************/
  
  PublicKeyAlgorithmBox box;

  if(x >= n)
    return false;

  // Führt modulare Exponentiation zur Berechnung der RSA-Verschlüsselung aus
  y = box.modularExponentation(x, e, n);

  return true;
}
