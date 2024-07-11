#include <cassert>
#include <iostream>
#include "RSAOracle.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

/**
 * @brief Konstruktor der Klasse RSAOracle, der die RSA-Parameter initialisiert.
 *
 * Dieser Konstruktor initialisiert die Klassenvariablen n und d mit den gegebenen
 * Primfaktoren p und q sowie dem geheimen Exponenten d.
 *
 * @param p Einer der Primfaktoren des RSA-Modulus.
 * @param q Der andere Primfaktor des RSA-Modulus.
 * @param d Der geheime Exponent des privaten Schlüssels.
 */
RSAOracle::RSAOracle(Integer &p, Integer &q, Integer &d)
{
    /*********************************************************************
     * Aufgabe 22a
     *********************************************************************/

    // Initialisiere Klassenvariablen
    this->n = p * q;
    this->d = d;
}

RSAOracle::~RSAOracle()
{
}

/**
 * @brief Berechnung des höchstwertigsten Bits der Zahl y^d mod n.
 *
 *  Benutzt die modulare Exponentation aus der PublicKeyAlgorithmBox Klasse
 *  von y^d mod n und überprüft anschließend, ob x < n / 2 ist.
 *
 * @param y Der verschlüsselte Text.
 * @return true, wenn die entschlüsselte Nachricht größer als die Hälfte von n ist, andernfalls false.
 */
bool RSAOracle::half(const Integer &y) const
{
    /*********************************************************************
     * Aufgabe 22b
     *********************************************************************/

    PublicKeyAlgorithmBox pkab;

    // Entschlüssele den verschlüsselten Text y unter Verwendung des privaten Schlüssels d und des Modulus n
    Integer x = pkab.modularExponentation(y, d, n);

    // Überprüfe, ob die entschlüsselte Nachricht x kleiner ist als die Hälfte des Modulus n
    if (x < n / 2)
    {
        // Wenn ja, gib false zurück
        return false;
    }
    else
    {
        // Andernfalls gib true zurück
        return true;
    }
}

bool RSAOracle::parity(const Integer &y) const
{
    PublicKeyAlgorithmBox pkab;
    Integer x = pkab.modularExponentation(y, d, n);

    return x.IsOdd();
}
