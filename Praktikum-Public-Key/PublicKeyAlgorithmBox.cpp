#include <iostream>
#include <sstream>
#include "nbtheory.h"
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace CryptoPP;
using namespace std;

// #EEA()
/**
 * @brief Erweiterter Euklidischer Algorithmus (EEA).
 *
 * Diese Funktion berechnet den größten gemeinsamen Teiler (ggT) zweier Integer a und b,
 * und findet außerdem die Koeffizienten x und y, sodass:
 *   a * x + b * y = ggT(a, b)
 *
 * @param a Das erste Integer.
 * @param b Das zweite Integer.
 * @param d Ausgabeparameter für ggT(a, b).
 * @param x Ausgabeparameter für den Koeffizienten x.
 * @param y Ausgabeparameter für den Koeffizienten y.
 * @return true, falls ggT(a, b) == 1 (zeigt an, dass a und b teilerfremd sind), sonst false.
 */
bool PublicKeyAlgorithmBox::EEA(const Integer &a, const Integer &b,
                                Integer &d, Integer &x, Integer &y)
{
    /******************************************************************************
     * Aufgabe 3
     ******************************************************************************/

    if (b == Integer::Zero()) {  // Wenn b gleich Null ist,
        d = a;                  // ggT(a, 0) = a
        x = Integer::One();     // x = 1
        y = Integer::Zero();    // y = 0
        return (d == Integer::One());  // Gib true zurück, falls ggT(a, 0) == 1, sonst false
    } else {                    // Ansonsten,
        Integer dNew, xNew, yNew;   // Deklariere temporäre Variablen
        EEA(b, a % b, dNew, xNew, yNew);  // Rekursiver Aufruf mit (b, a % b)
        d = dNew;               // Aktualisiere ggT(a, b) mit dem rekursiven Ergebnis
        x = yNew;               // Aktualisiere den Koeffizienten x
        y = xNew - (a / b) * yNew;  // Aktualisiere den Koeffizienten y
        return (d == Integer::One());  // Gib true zurück, falls ggT(a, b) == 1, sonst false
    }
} // EEA()

// #modularExponentation()
/**
 * @brief Modulare Exponentiation.
 *
 * Diese Funktion berechnet (a^b) % n mithilfe des Verfahrens der modularen Exponentiation,
 * welches Potenzen in modularer Arithmetik effizient berechnet.
 *
 * @param a Die Basis-Zahl.
 * @param b Der Exponent.
 * @param n Der Modulus.
 * @return Das Ergebnis von (a^b) % n.
 */
Integer PublicKeyAlgorithmBox::modularExponentation(const Integer &a,
                                                     const Integer &b, const Integer &n)
{
    /******************************************************************************
     * Aufgabe 5
     ******************************************************************************/

    Integer c = 0;              // Initialisiere c mit 0
    Integer d = 1;              // Initialisiere d mit 1
    size_t k = b.BitCount();    // Anzahl der gesetzten Bits in b (Bitlänge von b)

    for (int i = k - 1; i >= 0; i--)   // Iteriere von der höchsten bis zur niedrigsten gesetzten Stelle
    {
        c = 2 * c;              // c = c * 2 (Linksshift von c)
        d = (d * d) % n;        // d = (d^2) % n (Quadriere d und nimm den Rest modulo n)

        if (b.GetBit(i))        // Falls das Bit an Position i in b gesetzt ist (also 1)
        {
            c++;                // Erhöhe c um 1
            d = (d * a) % n;    // d = (d * a) % n (multipliziere d mit a und nimm den Rest modulo n)
        }
    }

    return d;                   // Gib das Ergebnis von (a^b) % n zurück
} // modularExponentiation()

// #multInverse()
/**
 * @brief Berechnet das multiplikative Inverse von a modulo n.
 *
 * Diese Funktion verwendet den erweiterten euklidischen Algorithmus (EEA),
 * um das multiplikative Inverse von a modulo n zu berechnen.
 *
 * @param a Die Zahl, für die das Inverse berechnet werden soll.
 * @param n Das Modul, in dem das Inverse berechnet wird.
 * @param a_inv Hier wird das berechnete Inverse von a modulo n gespeichert.
 * @return true, wenn das Inverse existiert und berechnet werden konnte, ansonsten false.
 */
bool PublicKeyAlgorithmBox::multInverse(const Integer &a, const Integer &n,
                                        Integer &a_inv)
{
    /******************************************************************************
     * Aufgabe 4
     ******************************************************************************/
    Integer d, x, y;

    // Anwendung des EEA, um den ggT und die Koeffizienten x und y zu finden
    if (EEA(a, n, d, x, y))
    {
        a_inv = x % n; // Berechnung des Inversen als x modulo n

        // Falls das Inverse negativ ist, wird es auf den positiven Bereich von n gehoben
        if (a_inv < Integer::Zero())
        {
            a_inv = a_inv + n;
        }
        return true; // Rückgabe, dass das Inverse erfolgreich berechnet wurde
    }
    else
    {
        return false; // Rückgabe, dass kein Inverses existiert (a und n sind nicht teilerfremd)
    }
} // multInverse()

// #witness()
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
bool PublicKeyAlgorithmBox::witness(const Integer &a, const Integer &n)
{
    /******************************************************************************
     * Aufgabe 7a
     ******************************************************************************/
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
} // witness()

// randomInteger()
Integer PublicKeyAlgorithmBox::randomInteger(const Integer &n)
{
    NonblockingRng rng;

    Integer a;
    a.Randomize(rng, Integer::One(), n);
    return a;
}

// #millerRabinTest()
/**
 * @brief Führt den Miller-Rabin-Primzahltest durch, um zu prüfen, ob n wahrscheinlich eine Primzahl ist.
 *
 * Diese Funktion implementiert den Miller-Rabin-Primzahltest, der eine Methode zur
 * Bestimmung der Primalität einer Zahl n ist. Die Wahrscheinlichkeit, dass n prim ist, wird durch die Anzahl
 * der Durchläufe s erhöht.
 *
 * @param n Die zu testende Zahl.
 * @param s Die Anzahl der Durchläufe des Tests.
 * @return true Wenn n wahrscheinlich eine Primzahl ist.
 * @return false Wenn n eine zusammengesetzte Zahl ist.
 */
bool PublicKeyAlgorithmBox::millerRabinTest(Integer &n, unsigned int s)
{
    /******************************************************************************
     * Aufgabe 7b
     ******************************************************************************/
    // Prüfe, ob n kleiner als 2 ist (0 und 1 sind nicht prim)
    if (n < 2)
    {
        return false; // 0 und 1 sind nicht prim
    }

    // Prüfe, ob n gleich 2 oder 3 ist (beide sind prim)
    if (n == 2 || n == 3)
    {
        return true; // 2 und 3 sind prim
    }

    // Prüfe, ob n gerade ist (gerade Zahlen größer als 2 sind nicht prim)
    if (n.IsEven())
    {
        return false; // Gerade Zahlen größer als 2 sind nicht prim
    }

    // Initialisiere einen Zufallszahlengenerator
    NonblockingRng rng;
    for (unsigned int i = 0; i < s; i++)
    {
        // Erzeuge eine Zufallszahl a im Bereich 2 <= a <= n-1
        Integer a;
        a.Randomize(rng, 2, n - 1);

        // Wenn a ein Zeuge dafür ist, dass n zusammengesetzt ist, gib false zurück
        if (witness(a, n))
        {
            return false; // Zusammengesetzt
        }
    }

    // Wenn keiner der Durchläufe n als zusammengesetzt identifiziert hat, ist n wahrscheinlich prim
    return true; // Wahrscheinlich prim

} // millerRabinTest()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomPrime(Integer &p,
                                                unsigned int bitlen, unsigned int s)
{
    /******************************************************************************
     * Aufgabe 14
     ******************************************************************************/
    NonblockingRng rng;
    unsigned int tries = 0;

    /******************************************************************************
     * Aufgabe 8
     ******************************************************************************/
    while (true)
    {
        // Erzeuge eine zufällige Zahl p mit bitlen Bits
        p.Randomize(rng, bitlen);

        // Führe den Miller-Rabin-Test durch, um zu überprüfen, ob p eine Primzahl ist
        if (millerRabinTest(p, s))
        {
            break; // p ist eine Primzahl, breche die Schleife ab
        }

        tries++; // Inkrementiere die Anzahl der Versuche
    }

    return tries; // Gib die Anzahl der Versuche zurück
} // randomPrime()

// #randomPrime()
/**
 * @brief Generiert eine zufällige Primzahl p.
 *
 * Diese Methode generiert eine zufällige Primzahl p, die die Bedingungen p ≡ 3 (mod 4) und eine bestimmte Bitlänge erfüllt.
 * Es wird der Miller-Rabin-Test verwendet, um die Primzahleigenschaft zu überprüfen.
 *
 * @param p Referenz auf den Integer, der die generierte Primzahl speichern wird.
 * @param bitlen Die Bitlänge der zu generierenden Primzahl.
 * @param s Die Anzahl der Runden des Miller-Rabin-Tests zur Überprüfung der Primzahleigenschaft.
 * @return Die Anzahl der Versuche, die benötigt wurden, um eine passende Primzahl zu finden.
 */
unsigned int PublicKeyAlgorithmBox::randomRabinPrime(Integer &p,
                                                     unsigned int bitlen, unsigned int s)
{
    NonblockingRng rng;
    unsigned int tries = 0;

    /******************************************************************************
     * Aufgabe 8
     ******************************************************************************/
    while (true)
    {
        // Erzeuge eine zufällige Zahl p mit bitlen Bits
        p.Randomize(rng, bitlen);

        // Überprüfe die Bedingung p ≡ 3 (mod 4)
        if (p % 4 != 3)
        {
            continue; // Springe zur nächsten Iteration, wenn Bedingung nicht erfüllt
        }

        // Führe den Miller-Rabin-Test durch, um zu überprüfen, ob p eine Primzahl ist
        if (millerRabinTest(p, s))
        {
            break; // p ist eine Primzahl, breche die Schleife ab
        }

        tries++; // Inkrementiere die Anzahl der Versuche
    }

    return tries; // Gib die Anzahl der Versuche zurück
} // randomRabinPrime()

/*
 * Algorithmus aus Yan Seite 337
 */
bool PublicKeyAlgorithmBox::modPrimeSqrt(const Integer &y, const Integer &p,
                                         vector<Integer> &v)
{
    /******************************************************************************
     * Aufgabe 10
     ******************************************************************************/
    // Überprüfen, ob p ≡ 3 (mod 4)
    if (p % 4 != 3)
    {
        return false; // p erfüllt nicht die Bedingung
    }

    // Berechne z = y^((p+1)/4) % p mittels modulare Exponentiation
    Integer z = modularExponentation(y, (p + 1) / 4, p);

    // Speichere die beiden Quadratwurzeln in v
    v.push_back(z);
    v.push_back(p - z);

    return true;
}

/**
 * @brief Berechnet den größten gemeinsamen Teiler von zwei ganzen Zahlen unter Verwendung des Euklidischen Algorithmus.
 *
 * Diese Funktion berechnet den ggT von a und b und speichert die Quotienten 
 * in dem Vektor q. Der Algorithmus verwendet den Euklidischen Algorithmus zur
 * Iteration über die Reste, bis der ggT gefunden ist.
 *
 * @param a Die erste ganze Zahl.
 * @param b Die zweite ganze Zahl.
 * @param q Ein Vektor zur Speicherung der Quotienten.
 * @return Der größte gemeinsame Teiler von a und b.
 */
Integer PublicKeyAlgorithmBox::euklid(const Integer &a, const Integer &b, vector<Integer> &q)
{
    /******************************************************************************
     * Aufgabe 19a
     ******************************************************************************/

    Integer r0 = a;
    Integer r1 = b;

    // Solange r1 nicht gleich Null ist, führe den Algorithmus aus
    while (r1 != 0)
    {
        // Berechne den Quotienten qm aus der Division von r0 durch r1
        Integer qm = (r0 / r1);

        // Füge den Quotienten zum Vektor q hinzu
        q.push_back(qm);

        // Berechne den neuen Rest rm1
        Integer rm1 = r0 - qm * r1;

        // Aktualisiere r0 auf den Wert von r1
        r0 = r1;
        // Aktualisiere r1 auf den neuen Rest rm1
        r1 = rm1;
    }

    // Gib den größten gemeinsamen Teiler zurück
    return r0;
}

/**
 * @brief Berechnet die Kettenbruchkonvergenzen von zwei ganzen Zahlen a und b.
 *
 * Diese Funktion berechnet die Kettenbruchkonvergenzen der beiden Zahlen a und b 
 * und speichert die Ergebnisse in den Vektoren c und d. Die Konvergenzen werden
 * durch die Erweiterung der Quotienten berechnet, die durch den erweiterten 
 * Euklidischen Algorithmus erhalten wurden.
 *
 * @param a Die erste ganze Zahl.
 * @param b Die zweite ganze Zahl.
 * @param c Vektor zur Speicherung der Zähler der Konvergenzen.
 * @param d Vektor zur Speicherung der Nenner der Konvergenzen.
 * @return Die Anzahl der berechneten Konvergenzen.
 */
unsigned int PublicKeyAlgorithmBox::computeConvergents(const Integer &a, const Integer &b, vector<Integer> &c, vector<Integer> &d)
{
    /******************************************************************************
     * Aufgabe 20a
     ******************************************************************************/
    
    vector<Integer> q;

    // Berechnet den Kettenbruch von a und b
    euklid(a, b, q);

    // Initialisiere die ersten beiden Werte der Zähler-Konvergenzen c
    c.push_back(1);       // Für j=0 ist der Wert 1
    c.push_back(q[0]);    // Für j=1 ist der Wert q[0]
    
    // Initialisiere die ersten beiden Werte der Nenner-Konvergenzen d
    d.push_back(0);
    d.push_back(1);
    
    // Berechne die restlichen Konvergenzen für j=2 bis zur Größe von q
    for (int j = 2; j <= q.size(); j++)
    {
        // Berechne den neuen Zähler cj der Konvergenz
        Integer cj = q[j - 1] * c[j - 1] + c[j - 2];
        // Berechne den neuen Nenner dj der Konvergenz
        Integer dj = q[j - 1] * d[j - 1] + d[j - 2];

        // Füge den neuen Zähler und Nenner zu den entsprechenden Vektoren hinzu
        c.push_back(cj);
        d.push_back(dj);
    }

    // Gib die Anzahl der berechneten Konvergenzen zurück
    return c.size();
}

// #sqrt()
/**
 * @brief Berechnet die kleinste nicht-negative ganzzahlige Quadratwurzel s von x, falls eine existiert.
 *
 * Diese Methode verwendet die Technik der binären Suche, um die Quadratwurzel s von x zu finden.
 * Sie liefert true zurück, falls eine solche s gefunden wurde, andernfalls false.
 *
 * @param x Die Eingabeganzahl, von der die Quadratwurzel berechnet werden soll.
 * @param s Ausgabeparameter, in dem die berechnete Quadratwurzel gespeichert wird.
 * @return true, wenn eine Quadratwurzel gefunden wurde, ansonsten false.
 */
bool PublicKeyAlgorithmBox::sqrt(const Integer &x, Integer &s) const
{
    /******************************************************************************
     * Aufgabe 17a
     ******************************************************************************/

    if (x < 0) // Keine Quadratwurzel für negative Zahlen
        return false;

    if (x == 0) // Quadratwurzel von 0 ist 0
    {
        s = 0;
        return true;
    }

    // Initialisierung für binäre Suche
    Integer left = 1;
    Integer right = x;

    while (left <= right)
    {
        Integer mid = (left + right) / 2; // Mittelpunkt der aktuellen Spanne

        // Überprüfen, ob mid*mid == x
        if (mid == x / mid && x % mid == 0)
        {
            s = mid; // mid ist die Quadratwurzel von x
            return true;
        }
        else if (mid < x / mid)
        {
            left = mid + 1; // Suchbereich auf rechte Hälfte einschränken
        }
        else
        {
            right = mid - 1; // Suchbereich auf linke Hälfte einschränken
        }
    }

    return false; // Keine Quadratwurzel gefunden
}

/**
 * @brief Generiert RSA-Parameter (p, q, e, d).
 *
 * Diese Methode generiert die notwendigen RSA-Parameter p, q, e und d.
 * Es werden zwei große Primzahlen p und q generiert, der öffentliche Schlüssel e wird so gewählt,
 * dass er teilerfremd zu phi(n) ist, und der private Schlüssel d wird als multiplikatives Inverses von e modulo phi(n) berechnet.
 *
 * @param p Referenz auf den Integer, der die generierte Primzahl p speichern wird.
 * @param q Referenz auf den Integer, der die generierte Primzahl q speichern wird.
 * @param e Referenz auf den Integer, der den generierten öffentlichen Schlüssel e speichern wird.
 * @param d Referenz auf den Integer, der den generierten privaten Schlüssel d speichern wird.
 * @param bitlen Die Bitlänge der zu generierenden Primzahlen p und q.
 * @param s Die Anzahl der Runden des Miller-Rabin-Tests zur Überprüfung der Primzahleigenschaft.
 */
void PublicKeyAlgorithmBox::generateRSAParams(Integer &p, Integer &q,
                                              Integer &e, Integer &d, unsigned int bitlen, unsigned int s)
{
    /******************************************************************************
     * Aufgabe 15
     ******************************************************************************/

    randomPrime(p, bitlen, s); // Generiere die Primzahl p mit der gegebenen Bitlänge und Rundenanzahl s für den Miller-Rabin-Test
    randomPrime(q, bitlen, s); // Generiere die Primzahl q mit der gegebenen Bitlänge und Rundenanzahl s für den Miller-Rabin-Test

    Integer phi = (p - 1) * (q - 1); // Berechne phi(n) = (p - 1) * (q - 1)

    do
    {
        e = randomInteger(phi - 1); // Generiere eine zufällige Ganzzahl e im Bereich [1, phi - 1]
    } while (Integer::Gcd(e, phi) != 1); // Wiederhole die Generierung von e, bis GCD(e, phi) == 1

    multInverse(e, phi, d); // Berechne das multiplikative Inverse von e modulo phi und speichere es in d
}

string PublicKeyAlgorithmBox::vectorToString(const vector<CryptoPP::Integer> &v)
{
    ostringstream buffer;

    buffer << "[";
    for (size_t i = 0; i < v.size(); i++)
    {
        buffer << v[i];
        if (i < v.size() - 1)
        {
            buffer << ", ";
        }
    }
    buffer << "]";

    return buffer.str();
}