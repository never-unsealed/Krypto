/*
 * MillerRabinTest.cpp
 */
#include <iostream>
#include <cassert>
#include <integer.h>
#include "MillerRabinTest.h"

using namespace std;
using namespace CryptoPP;

/**
 * @brief Implementiert den modexp Algorithmus aus dem Vorkurs der Vorlesung.
 *
 * Führt modulare Exponentation anhand einer Basis, einem Exponenten und dem entsprechenden
 * Modul durch.
 *
 * @param a Basis für die modulare Exponentation
 * @param b Der Exponent
 * @param n Der Modul für die modulare Exponentation
 * @return Das Ergebnis der modularen Exponentation als Integer
 */
Integer MillerRabinTest::modularExponentation(
    const Integer &a,
    const Integer &b,
    const Integer &n)
{

    /*************************************************************************
     * Aufgabe 3b.
     *************************************************************************/
    Integer c = 0;
    Integer d = 1;
    size_t k = b.BitCount(); // Zahl der gesetzten Bits

    for (int i = k - 1; i >= 0; i--)
    {
        c = 2 * c;
        d = (d * d) % n;

        if (b.GetBit(i)) // Ist das Bit an Stelle i gesetzt (also 1)?
        {
            c++;
            d = (d * a) % n;
        }
    }

    return d;
}

/**
 * @brief Implementiert die Witness Funktion.
 *
 * Führt den Witness-Test durch, welche eine abgwandelte Form von modularer Exponentation
 * darstellt. Der Algorithmus berechnet a^(n-1) mod n und sucht während der Berechnung nach
 * Lösungen für die Gleichung x^2 kongurent 1 (mod n), die verschieden zu 1 und n - 1 sind.
 *
 * @param a Solider Beweis für n
 * @param n Die zu testende natürliche Zahl
 * @return true, falls n keine Primzahl
 */
bool MillerRabinTest::witness(const Integer &a, const Integer &n)
{
    /*************************************************************************
     * Aufgabe 3c.
     *************************************************************************/

    Integer r, u, d, x;

    u = ((n - 1) / 2); // Teile n -1 (garantiert gerade) zum ersten Mal
    r = 1;             // Setze r auf eins

    while (!(u % 2)) // Wiederhole so lange, bis u ungerade ist.
    {
        r++;    // Erhöhe r um 1
        u /= 2; // Teile u durch 2
    }

    // Implementierung des Pseudocodes auf Folie 35 für Primzahlen

    d = modularExponentation(a, u, n);

    for (int i = 0; i < r; i++)
    {
        x = d;
        d = (d * d) % n;

        if (d == 1 && x != 1 && x != (n - 1))
            return true;
    }

    if (d != 1)
        return true;

    return false;
}

/**
 * @brief Testet, ob es sich bei einer Zahl um eine Primzahl handelt.
 *
 * Führt verschiedene Tests durch, um zu ermitteln, ob es sich bei n um eine
 * Primzahl handelt. Es wird die Primazahl-Methode sowie der GCD zur Ermittlung genutzt.
 *
 * @param rng Referenz auf ein PRNG-Objekt, das zur Generierung von Zufallsbytes genutzt wird.
 * @param n Die zu testende Zahl.
 * @param s Qualitätsparameter
 * @return true, falls n eine Primzahl ist. Andernfalls false.
 */
bool MillerRabinTest::isPrime(PRNG *rng, const Integer &n, unsigned int s)
{
    /*************************************************************************
     * Aufgabe 4a.
     *************************************************************************/

    // Implementierung des Pseudocodes auf Folie 37 & 38 (Primzahlen)

    bool ret_val = true; // Initialisere Rückgabewert auf true
    Integer a;

    if (!(n % 2)) // Wenn n gerade ist wird false zurückgegeben
        return false;

    Integer b;
    unsigned int e;

    for (int i = 0; i < s; i++) // Iteriere s-mal
    {
        a = rng->getInteger(n - 1); // Initialisiere neuen Integer mit n - 1 Zufallsbytes

        if (Integer::Gcd(a, n) > 1) // Berechne GCD aus der Zufallszahl und n
        {
            return false; // Falls GCD > 1, handelt es sich um keine Primzahl.
        }

        if (witness(a, n) == true) // Witness-Methode
        {
            return false; // Falls true zurückgegeben wird, handelt es sich um keine Primzahl
        }
    }

    if (MillerRabinTest::isIntegerPower(n, b, e))
        return false;

    return ret_val;
}

/**
 * @brief Ermittelt die Quadratwurzel einer gegebenen Zahl n und gibt an, ob diese ganzzahlig ist.
 *
 * Nutzt den in der Vorlesung kennengelernten Algorithmus, um die Quadratwurzel einer
 * Zahl n zu berechnen. Es wird außerdem unterschieden, ob diese Wurzel ganzzahlig ist oder nicht
 * und ein entsprechender Idikator (true, false) zurückgegeben.
 *
 * @param n Zahl, für die die Quadratwurzel berechnet werden soll.
 * @param r Referenz, die die Quadratwurzel von n erhält.
 * @return true, falls die Wurzel von n ganzzahlig ist. Andernfalls false.
 */
bool MillerRabinTest::searchSqrt(const Integer &n, Integer &r)
{
    /*************************************************************************
     * Aufgabe 5a.
     *************************************************************************/
    bool result = true;

    // Implementierung des Pseudocodes auf Folie 31 (Primzahlen)

    int l = n.BitCount() + 1;       // Anzahl der Bits in n + 1
    Integer y = Integer::Power2(l); // y = 2^l
    Integer x;

    do
    {

        x = y;
        y = ((x + (n / x)) / 2);

    } while (y < x);

    if (x.Squared() == n) // ist x eine Ganzzahl? / x^2
    {
        r = x;
        return true;
    }
    else if (y.Squared() == n) // ist x eine Ganzzahl? / y^2
    {
        r = y;
        return true;
    }
    else // Die Wuzel ist keine Ganzzahl
    {
        r = x;
        return false;
    }
}

/**
 * @brief Ermittelt die k-te Wurzel einer gegebenen Zahl n und gibt an, ob diese ganzzahlig ist.
 *
 * Nutzt den in der Vorlesung kennengelernten (modifizierten) Algorithmus, um die k-te Wurzel einer
 * Zahl n zu berechnen. Es wird außerdem unterschieden, ob diese Wurzel ganzzahlig ist oder nicht
 * und ein entsprechender Idikator (true, false) zurückgegeben.
 *
 * @param n Zahl, für die die Wurzel berechnet werden soll.
 * @param k Gibt an, welche Wuzel berechnet werden soll.
 * @param r Referenz, die die k-te Wurzel von n erhält.
 * @return true, falls die Wurzel von n ganzzahlig ist. Andernfalls false.
 */
bool MillerRabinTest::searchRoot(const Integer &n, unsigned int k, Integer &r)
{
    /*************************************************************************
     * Aufgabe 7a.
     *************************************************************************/
    bool result = true;

    // Implementierung des Pseudocodes auf Folie 31 + 32 (Primzahlen)

    int l = (n.BitCount() / k) + 1; // Anzahl der Bits in n geteilt durch k + 1
    Integer y = Integer::Power2(l); // y = 2^l
    Integer x;

    do
    {

        x = y;
        y = ((k - 1) * x + (n / exp(x, k - 1))) / k;

    } while (y < x);

    if (exp(x, k) == n) // ist x eine Ganzzahl? / x^2
    {
        r = x;
        return true;
    }
    else if (exp(y, k) == n) // ist x eine Ganzzahl? / y^2
    {
        r = y;
        return true;
    }
    else // Die Wuzel ist keine Ganzzahl
    {
        r = x;
        return false;
    }
}

/**
 * @brief Methode zur Berechnung von Ganzzahlpotenzen.
 *
 * Nutzt den Algorithmus für modulare Exponentation, aber in einer abgewandelten Form,
 * um keine Modulo-Rechnungen einzubauen.
 *
 * @param b Basis für die Exponentation
 * @param e Der Exponent
 * @return Das Ergebnis der Exponentation als Integer
 */
Integer MillerRabinTest::exp(const Integer &b, const Integer &e)
{
    /*************************************************************************
     * Aufgabe 6a.
     *************************************************************************/
    Integer c = 0;
    Integer d = 1;
    size_t k = e.BitCount(); // Zahl der gesetzten Bits

    for (int i = k - 1; i >= 0; i--)
    {
        c = 2 * c;
        d = (d * d); // Kein Modulo an dieser Stelle !

        if (e.GetBit(i)) // Ist das Bit an Stelle i gesetzt (also 1)?
        {
            c++;
            d = (d * b); // Kein Modulo an dieser Stelle !
        }
    }

    return d;
}

/**
 * @brief Testet, ob die Zahl n eine Ganzzahlpotenz ist.
 *
 * Testet, ob die Zahl n eine Ganzzahlpotenz ist, indem es die searchRoot-Methode nutzt.
 *
 * @param n Die Zahl, die überprüft werden soll.
 * @param b Erhält die Basis, falls n eine Ganzzahlpotenz ist.
 * @param e Erhält den Exponenten, falls n eine Ganzzahlpotenz ist.
 * @return true, falls n eine Ganzzahlpotenz ist, ansonsten false.
 */
bool MillerRabinTest::isIntegerPower(const Integer &n, Integer &b, unsigned int &e)
{
    /*************************************************************************
     * Aufgabe 8a.
     *************************************************************************/
    bool found = false;
    int k = 2;
    Integer x;
    bool ist_ganzzahl;

    // Implementiert den Pseudocode von Folie 34 (Primzahlen)

    while (k <= n.BitCount() && found == false)
    {
        ist_ganzzahl = searchRoot(n, k, x);

        if (ist_ganzzahl) // Ganzzahlpotenz gefunden
            found = true;
        else
            k++; // k muss erhöht werden und es muss weitergesucht werden
    }

    if (found == true)
    {
        b = x;
        e = k;
        return true;
    }

    return false;
}
