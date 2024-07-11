#include <iostream>
#include <vector>
#include "PublicKeyAlgorithmBox.h"
#include "RSAAttack.h"

using namespace std;

RSAAttack::RSAAttack()
{
}

RSAAttack::~RSAAttack()
{
}

// #factorizeN()
/**
 * @brief Faktorisierung des gegebenen RSA-Modulus n unter Verwendung von phi_n.
 *
 * Diese Funktion versucht, n durch Lösen der quadratischen Gleichung, die aus den
 * Eigenschaften von n und phi_n abgeleitet ist, zu faktorisieren. Sie berechnet die
 * Wurzeln der Gleichung und überprüft, ob das Produkt der Wurzeln gleich n ist.
 *
 * @param n Der zu faktorisierende RSA-Modulus.
 * @param phi_n Die Euler'sche Totientfunktion von n.
 * @param p Einer der Primfaktoren von n, falls erfolgreich.
 * @param q Der andere Primfaktor von n, falls erfolgreich.
 * @return true, wenn die Faktorisierung erfolgreich war, andernfalls false.
 */
bool RSAAttack::factorizeN(const Integer &n, const Integer &phi_n, Integer &p, Integer &q) const
{
    // Deklaration der benötigten Variablen
    Integer a, b, c, sqrt_res;
    PublicKeyAlgorithmBox box;

    // Setze a auf 1
    a = 1;
    // Berechne b als phi_n - n - 1
    b = phi_n - n - 1;
    // Setze c auf n
    c = n;

    // Berechne die Diskriminante und überprüfe, ob die Quadratwurzel existiert
    if (!box.sqrt((b * b) - 4 * a * c, sqrt_res))
    {
        // Falls die Quadratwurzel nicht existiert, return false
        return false;
    }

    // Berechne die beiden Wurzeln der quadratischen Gleichung
    p = ((-b) + sqrt_res) / 2;
    q = ((-b) - sqrt_res) / 2;

    // Überprüfe, ob das Produkt der Wurzeln gleich n ist
    if (p * q != n)
        return false;

    // Wenn die Überprüfung bestanden ist, return true
    return true;
}

// #wienerAttack()
/**
 * @brief Führt den Wiener-Angriff auf den RSA-Modulus n und den öffentlichen Schlüssel e durch.
 *
 * Diese Funktion versucht, den Wiener-Angriff anzuwenden, um die Primfaktoren p und q des RSA-Modulus n
 * zu finden, indem die Kettenbruchkonvergenzen von e/n berechnet werden. Der Angriff basiert auf der Annahme,
 * dass der geheime Schlüssel d klein genug ist, sodass die Konvergenzen genutzt werden können, um p und q
 * zu berechnen.
 *
 * @param n Der RSA-Modulus.
 * @param e Der öffentliche Schlüssel.
 * @param p Einer der Primfaktoren von n, falls erfolgreich.
 * @param q Der andere Primfaktor von n, falls erfolgreich.
 * @return true, wenn der Angriff erfolgreich war und die Primfaktoren gefunden wurden, sonst false.
 */
bool RSAAttack::wienerAttack(const Integer &n, const Integer &e, Integer &p, Integer &q) const
{
    /******************************************************************************
     * Aufgabe 21a
     ******************************************************************************/

    PublicKeyAlgorithmBox box;

    // Vektoren zur Speicherung der Zähler und Nenner der Kettenbruchkonvergenzen
    vector<Integer> c, d;

    // Berechne die Kettenbruchkonvergenzen von e/n und speichere die Ergebnisse in c und d
    box.computeConvergents(e, n, c, d);

    // Iteriere über die berechneten Konvergenzen, beginnend ab der dritten Konvergenz (Index 2)
    for (int i = 2; i <= c.size(); i++)
    {
        Integer sqrt_res;

        // Überspringe Konvergenzen, deren Zähler oder Nenner gleich Null sind
        if (c[i - 1] == 0 || d[i - 1] == 0)
            continue;

        // Berechne phi_n basierend auf der aktuellen Konvergenz
        Integer phi_n = (e * d[i - 1] - 1) / c[i - 1];

        // Überprüfe, ob phi_n ganzzahlig ist und versuche die Faktorisierung von n
        if (((e * d[i - 1] - 1) % c[i - 1]) == 0 && factorizeN(n, phi_n, p, q))
        {
            // Wenn die Faktorisierung erfolgreich war, gib true zurück
            return true;
        }
    }

    return false;
}

// #halfAttack()
/**
 * @brief Führt einen Half-Attack auf die RSA-Verschlüsselung durch, um den Klartext wiederherzustellen.
 * 
 * @param n Das Modul, das in RSA verwendet wird.
 * @param e Der öffentliche Exponent, der in RSA verwendet wird.
 * @param y Der Geheimtext, der entschlüsselt werden soll.
 * @param rsa_oracle Ein Oracle-Object für die RSA-Entschlüsselung.
 * @return Der Integer, der den aus dem Geheimtext wiederhergestellten Klartext repräsentiert.
 */
Integer RSAAttack::halfAttack(const Integer &n, const Integer &e, const Integer &y,
                              RSAOracle &rsa_oracle) const
{
    // Initialisiere Variablen und Datenstrukturen
    PublicKeyAlgorithmBox box;
    vector<bool> h;  // Vektor zur Speicherung der Ergebnisse von Oracle-Abfragen
    
    // Bestimme die Bitlänge des Moduls
    Integer l = n.BitCount();
    
    // Initialisiere y_new mit dem Geheimtext
    Integer y_new = y;
    Integer lo, hi, mid;  // Variablen für die binäre Suche
    
    // Führe die Half-Attack-Schleife aus
    for (int i = 0; i < l; i++)
    {
        // Frage das Oracle mit y_new ab und speichere das Ergebnis
        h.push_back(rsa_oracle.half(y_new));
        
        // Aktualisiere y_new gemäß dem RSA-Entschlüsselungsprozess
        y_new = y_new * box.modularExponentation(2, e, n);
    }
    
    // Initialisiere die Grenzen für die binäre Suche
    lo = 0;
    hi = n;
    
    // Erweitere hi auf eine ausreichend große Zahl für die Präzision der binären Suche
    hi *= 100000;
    
    // Führe eine binäre Suche durch, um den Klartext wiederherzustellen
    for (int i = 0; i < l; i++)
    {
        mid = (hi + lo) / 2;  // Berechne den Mittelpunkt
        
        // Passe die Grenzen der binären Suche basierend auf den Oracle-Ergebnissen an
        if (h[i] == 1)
        {
            lo = mid;
        }
        else
        {
            hi = mid;
        }
    }
    
    // Korrigiere das Ergebnis, um den Klartext zurückzugeben
    hi /= 100000;
    
    return hi;
}
