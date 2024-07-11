#include "PublicKeyAlgorithmBox.h"
#include "RSADecryptor.h"

#include <iostream>

using namespace std;

/**
 * @brief Konstruktor für den RSA-Entschlüssler.
 *
 * Dieser Konstruktor initialisiert den RSA-Decryptor mit den gegebenen Primzahlen p und q sowie dem privaten Schlüssel d.
 * Er berechnet außerdem das Produkt n = p * q.
 *
 * @param p Eine Primzahl, die zur Berechnung des privaten Schlüssels verwendet wird.
 * @param q Eine Primzahl, die zur Berechnung des privaten Schlüssels verwendet wird.
 * @param d Der private Schlüssel für die RSA-Entschlüsselung.
 */
RSADecryptor::RSADecryptor(const Integer &p, const Integer &q,
                           const Integer &d)
{
  /******************************************************************************
   * Aufgabe 16c
   ******************************************************************************/

  // Initialisiere Klassenvariablen
  this->p = p;
  this->q = q;
  this->d = d;

  // Berechne n
  n = p * q;
}

RSADecryptor::~RSADecryptor()
{
}

// #compute()
/**
 * @brief Entschlüsselt eine gegebene verschlüsselte Ganzzahl mittels RSA mit dem privaten Schlüssel d und dem Modul n.
 * 
 * Diese Funktion entschlüsselt die RSA-verschlüsselte Ganzzahl y unter Verwendung des privaten Schlüssels d und des Moduls n.
 * Sie verwendet modulare Exponentiation zur Berechnung.
 * 
 * @param y Die verschlüsselte Ganzzahl, die entschlüsselt werden soll.
 * @param x Ausgabeparameter, in dem das entschlüsselte Ergebnis gespeichert wird.
 * @return true bei Erfolg, false wenn y >= n.
 */
bool RSADecryptor::compute(const Integer &y, Integer &x) const
{
  /******************************************************************************
   * Aufgabe 16d
   ******************************************************************************/

  if (y >= n)
    return false;

  PublicKeyAlgorithmBox box;

  x = box.modularExponentation(y, d, n);  // Entschlüssle mit modularer Exponentation

  return true;
}

// #crt()
/**
 * @brief Entschlüsselt eine gegebene verschlüsselte Ganzzahl y mittels RSA unter Verwendung des Chinesischen Restsatzes (CRT).
 * 
 * Diese Funktion entschlüsselt die RSA-verschlüsselte Ganzzahl y unter Verwendung des Chinesischen Restsatzes (CRT) und der Primzahlen p und q sowie des privaten Schlüssels d.
 * Sie verwendet modulare Exponentiation für eine effiziente Berechnung.
 * 
 * @param y Die verschlüsselte Ganzzahl, die entschlüsselt werden soll.
 * @param x Ausgabeparameter, in dem das entschlüsselte Ergebnis gespeichert wird.
 * @return true bei Erfolg, false wenn y >= n.
 */
bool RSADecryptor::crt(const Integer &y, Integer &x) const
{
  /******************************************************************************
   * Aufgabe 16e
   ******************************************************************************/

  PublicKeyAlgorithmBox box; // Instanz der Klasse PublicKeyAlgorithmBox erstellen

  if(y >= n) // Überprüfen, ob y größer oder gleich n ist
    return false;

  Integer dp = d % (p - 1); // Berechnung von dp = d mod (p - 1)
  Integer dq = d % (q - 1); // Berechnung von dq = d mod (q - 1)
  Integer qinv; // Deklaration einer Variablen qinv vom Typ Integer
  box.multInverse(q, p, qinv); // Berechnung des multiplikativen Inversen von q modulo p und Speicherung in qinv

  Integer m1 = box.modularExponentation(y, dp, p); // Berechnung von m1 = y^dp mod p
  Integer m2 = box.modularExponentation(y, dq, q); // Berechnung von m2 = y^dq mod q

  x = ((qinv * (m1 - m2)) % p) * q + m2; // Berechnung des entschlüsselten Ergebnisses x mittels CRT

  return true; 
}

// #garner()
/**
 * @brief Entschlüsselt eine gegebene verschlüsselte Ganzzahl y mittels RSA unter Verwendung des Garner-Algorithmus.
 * 
 * Diese Funktion entschlüsselt die RSA-verschlüsselte Ganzzahl y unter Verwendung des Garner-Algorithmus und der Primzahlen p und q sowie des privaten Schlüssels d.
 * Sie verwendet modulare Exponentiation und das multiplikative Inverse für eine effiziente Berechnung.
 * 
 * @param y Die verschlüsselte Ganzzahl, die entschlüsselt werden soll.
 * @param x Ausgabeparameter, in dem das entschlüsselte Ergebnis gespeichert wird.
 * @return true bei Erfolg, false wenn y >= n.
 */
bool RSADecryptor::garner(const Integer &y, Integer &x) const
{
  /******************************************************************************
   * Aufgabe 16f
   ******************************************************************************/

  PublicKeyAlgorithmBox box; // Instanz der Klasse PublicKeyAlgorithmBox erstellen

  if(y >= n) // Überprüfen, ob y größer oder gleich n ist
    return false; 

  Integer a = box.modularExponentation(y, d, p); // Berechnung von a = y^d mod p
  Integer b = box.modularExponentation(y, d, q); // Berechnung von b = y^d mod q
  Integer inv; // Deklaration einer Variablen inv vom Typ Integer

  box.multInverse(q, p, inv); // Berechnung des multiplikativen Inversen von q modulo p und Speicherung in inv

  x = (((a - b) * inv) % p) * q + b; // Berechnung des entschlüsselten Ergebnisses x mittels des Garner-Algorithmus

  return true; 
}
