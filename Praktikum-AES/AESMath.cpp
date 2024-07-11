/*!
 * \file
 */

#include <sstream>
#include <iostream>
#include <iomanip>
#include "AESMath.h"

using namespace std;

AESMath::AESMath() : exp_table(), log_table(256, 0), sbox(), inv_sbox(256, 0)
{
    // Generator of GF(256)
    byte g = 3;

    /*************************************************************************
     * Aufgabe 5a
     *************************************************************************/
    byte startwert = 1; // Initialwert für die Exponentialtabelle

    for (int i = 0; i < 255; i++) // Iteriere 255 mal
    {
        exp_table.push_back(startwert);  // Füge den aktuellen Startwert zur Exponentialtabelle hinzu
        startwert = rpmul(startwert, g); // Multipliziere den Startwert mit dem Generator und weise das Ergebnis startwert zu
    }

    /*************************************************************************
     * Aufgabe 5b
     *************************************************************************/

    for (int i = 0; i < 255; i++) // Iteriere 255 mal
    {
        log_table[exp_table[i]] = i; // Fülle die log_table andhand der exp_table auf
    }

    /*************************************************************************
     * Aufgabe 9a
     *************************************************************************/
    for (int i = 0; i < 256; i++)
    {                                    // Iteriere 256 mal
        byte inverse = inv(i);           // Berechne das Inverse des aktuellen Werts
        sbox.push_back(atrans(inverse)); // Wende die affine Transformation auf das Inverse an und füge es zur S-Box hinzu
    }
    /*************************************************************************
     * Aufgabe 9b
     *************************************************************************/
    for (int i = 0; i < 256; i++)
    {                          // Iteriere 256 mal
        inv_sbox[sbox[i]] = i; // Fülle die inverse S-Box anhand der S-Box auf
    }
}

/**
 * @brief Führt Polynomaddition durch.
 *
 * Die Addition zweier Elemente in einem endlichen Feld erfolgt durch
 * „Addition“ der Koeffizienten für die entsprechenden Potenzen in den Polynomen
 * für die beiden Elemente. Die Addition wird durchgeführt mit der XOR-Operation.
 *
 * @param a Erster Wert für Polynomaddition.
 * @param b Zweiter Wert für Polynomaddition.
 * @return Das Ergebnis der Polynomaddition.
 */
byte AESMath::add(byte a, byte b)
{
    /*************************************************************************
     * Aufgabe 2
     *************************************************************************/
    byte r = 0;

    r = a ^ b; // Führe bitweise XOR (exklusives-OR) Operation durch

    return r;
}

/**
 * @brief Führt die Affine Transformation über GF(2) durch.
 *
 * Diese Funktion führt eine Byte-Transformation auf dem Eingabewert x durch,
 * indem sie eine Reihe von XOR-Operationen gemäß den Spezifikationen des
 * Advanced Encryption Standard (AES) durchführt.
 *
 * @param x Das Eingangsbyte, auf das die Transformation angewendet wird.
 * @return Das transformierte Byte, das das Ergebnis der Transformation darstellt.
 */
byte AESMath::atrans(byte x)
{
    /*************************************************************************
     * Aufgabe 8
     *************************************************************************/

    byte ret_val = 0;

    for (int i = 0; i < 8; i++)
    {
        // Berechne anhang der Formel
        // von Kapitel 5.1.1 in FIPS 197 (AES)

        byte b = ((x >> i) & 1) ^
                 ((x >> ((i + 4) % 8)) & 1) ^
                 ((x >> ((i + 5) % 8)) & 1) ^
                 ((x >> ((i + 6) % 8)) & 1) ^
                 ((x >> ((i + 7) % 8)) & 1) ^
                 ((0x63 >> i) & 1);

        // Berechne Parität und schreibe entsprechendes
        // Bit an die passende Position in ret_val
        ret_val |= parity(b) << i;
    }

    return ret_val;
}

/**
 * @brief Berechnet die Funktion Funktion exp(i) = 3^i über GF(256).
 *
 * Liest den gegebenen Wert i aus der exp_table-Klassenvariable aus und
 * gibt diesen zurück.
 *
 * @param i Index, der aus der exp_table gelesen werden soll.
 * @return Wert des gegeben Index.
 */
byte AESMath::exp(byte i) const
{
    /*************************************************************************
     * Aufgabe 5c
     *************************************************************************/
    return exp_table[i];
}

/**
 * @brief Berechnet das multiplikative Inverse von GF(256).
 *
 * Die Methode berechnet das Inverse eines Bytes durch eine logarithmische Transformation
 * in den Exponentenraum des Galois-Feldes, die Berechnung des negativen Exponenten
 * und anschließende Rücktransformation in den ursprünglichen Feldraum.
 *
 * @param b Wert, für den das multiplikative Inverse berechnet werden soll.
 * @return Das multiplikative Inverse.
 */
byte AESMath::inv(byte b) const
{
    /*************************************************************************
     * Aufgabe 6
     *************************************************************************/
    int fin;

    if (b == 0)
    {
        return 0; // Das Inverse von 0 ist 0
    }

    fin = (255 - log(b)) % 255; // Berechnung des Inversen im Logarithmus
    return exp(fin);            // Zurückkonvertierung in das Feld
}

/**
 * @brief Berechnet die Funktion Funktion log3(b) über GF(256).
 *
 * Liest den gegebenen Wert b aus der log_table-Klassenvariable aus und
 * gibt diesen zurück.
 *
 * @param b Index, der aus der log_table gelesen werden soll.
 * @return Wert des gegeben Index.
 */
byte AESMath::log(byte b) const
{
    /*************************************************************************
     * Aufgabe 5d
     *************************************************************************/
    return log_table[b];
}

/**
 * @brief Führt die Russische Bauernmultiplikation durch.
 *
 * Multipliziert zwei natürliche Zahlen mittels der Russischen Bauernmultiplikation
 * Nutzt hierfür die add und xtime Implementierung der AESMath Klasse.
 * Wenn a und b 0 sind, wird null zurückgegeben.
 * Solange a größer als 0 ist, wird geprüft, ob a ungerade ist. 
 * Wenn dies der Fall ist, wird b zum Zwischenergebnis p hinzugefügt. Anschließend wird a halbiert und b mit der Funktion xtime multipliziert. 
 * Dieser Prozess wird wiederholt, bis a gleich 0 ist. Am Ende wird das Ergebnis p zurückgegeben, das das Produkt von a und b darstellt, 
 * berechnet durch die Addition von b zu p, wenn a ungerade ist, und fortlaufende Halbierung von a sowie Verdoppelung von b.
 *
 * @param a Erster Wert für Bauernmultiplikation.
 * @param b Zweiter Wert für Bauernmultiplikation.
 * @return Das Ergebnis der Bauernmultiplikation.
 */
byte AESMath::rpmul(byte a, byte b)
{
    /*************************************************************************
     * Aufgabe 4
     *************************************************************************/
    byte p = 0;

    // Implementierung nach Folie 21 (AES)

    if (!a || !b) // Wenn a und b 0 sind, wird entsprechend 0 zurückgegeben
        return p;

    while (a > 0)
    {
        if ((a % 2)) // Prüfung, ob a gerade ist
            p = add(p, b);

        a = (a / 2);
        b = xtime(b);
    }

    return p;
}

/**
 * @brief Multipliziert 2 Werte aus GF(256) miteinander.
 *
 *  Die Methode berechnet das Produkt zweier Bytes durch eine logarithmische Transformation
 *  in den Exponentenraum des Galois-Feldes, die Addition der Exponenten, und anschließende
 *  Rücktransformation in den ursprünglichen Feldraum.
 *  Das Ergebnis jeweils von log(a) + log(b) wird in den Feldraum von GF(256) moduliert
 *  und das Ergebnis der Operation für den exp Aufruf verwendet, dessen Ergebnis
 *  von der Funktion zurückgegeben werden soll.
 *
 * @param a Erster Wert für die Multiplikation.
 * @param b Zweiter Wert für Multiplikation.
 * @return Das Ergebnis der Multiplikation.
 */
byte AESMath::mul(byte a, byte b) const
{
    /*************************************************************************
     * Aufgabe 5e
     *************************************************************************/
    int fin;

    if (a == 0 || b == 0) // Wenn a und b 0 sind, wird entsprechend 0 zurückgegeben
    {
        return 0;
    }

    fin = (log(a) + log(b)) % 255; // logarithmische Transformation über GF(256)

    return exp(fin);
}

/**
 * @brief Testet, ob die Anzahl der Bits mit Wert 1 in einem Byte gerade ist.
 *
 *  Die Methode zählt alle Bits mit dem Wert 1 im Byte b zusammen und gibt an,
 *  ob die Anzahl dieser Bits gerade oder ungerade ist. Das tut sie, indem
 *  sie cntr dann erhöht, wenn das LSB in b 1 ist und anschließend
 *  für den nächsten Test das second LSB zum LSB verschiebt.
 *
 * @param b Die 8-bit breite Zahl, deren Bits untersucht werden sollen.
 * @return 1, wenn die Anzahl der Bits mit dem Wert 0 ungerade ist, ansonsten 0.
 */
byte AESMath::parity(byte b)
{
    /*************************************************************************
     * Aufgabe 7
     *************************************************************************/
    byte cntr = 0;

    for (int i = 0; i < 8; i++) // Eine Iteration pro Bit im Byte
    {
        cntr += b & 1; // Wenn das least significant bit 1 ist, erhöhe cntr um 1 (b & 1 kann nur 1 oder 0 ergeben)
        b >>= 1;       // Verschiebe das vorletzte bit auf die letzte position, dieses ist "als nächstes in der reihe"
    }

    return (cntr % 2); // Ist CNTR gerade?
}

void AESMath::printTable(const vector<byte> &table)
{
    cout << "         +----------------------------------------";
    cout << "---------------------------------------+" << endl;
    cout << "         |                                       y";
    cout << "                                       |" << endl;
    cout << "         +----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    cout << "        ";
    for (size_t i = 0; i < 16; i++)
    {
        cout << " | " << setw(2) << setfill(' ') << hex << i;
    }
    cout << " |" << endl;
    cout << "+---+----+----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    for (size_t i = 0; i < 16; i++)
    {
        cout << "|   | " << setw(2) << setfill(' ') << hex << i;
        for (size_t j = 0; j < 16; j++)
        {
            cout << " | " << setw(2) << setfill('0') << (unsigned)table[(i << 4) + j];
        }
        cout << " |" << endl;
        if (i < 15)
        {
            if (i == 7)
            {
                cout << "| x ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;
            }
            else
            {
                cout << "|   ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;
            }
        }
        else
        {
            cout << "|---";
            cout << "+----+----+----+----+----+----+----+----+----+";
            cout << "----+----+----+----+----+----+----+----+" << endl;
        }
    }
}

/**
 * @brief Berechnet die s-Box anhand der s-Box-Tabelle.
 *
 * Nutzt die Klassenvariable sbox, um den Wert an Position b auszulesen
 * und gibt diesen zurück.
 *
 * @param b Index, der aus der sbox-Tabelle gelesen werden soll.
 * @return Wert des gegeben Index.
 */
byte AESMath::sBox(byte b) const
{
    /*************************************************************************
     * Aufgabe 9c
     *************************************************************************/
    return sbox[b];
}

/**
 * @brief Berechnet die inverse S-Box anhand der inversen S-Box-Tabelle.
 *
 * Nutzt die Klassenvariable inv_sbox, um den Wert an Position b auszulesen
 * und gibt diesen zurück.
 *
 * @param b Index, der aus der inversen SBOX-Tabelle gelesen werden soll.
 * @return Wert des gegeben Index.
 */
byte AESMath::invSBox(byte b) const
{
    /*************************************************************************
     * Aufgabe 9d
     *************************************************************************/
    return inv_sbox[b];
}

/**
 * @brief Multipliziert ein Element b (aus GF(256)) mit dem Wert 2.
 *
 * Die Operation wird auf Byte-Ebene durch Linksverschiebung auf b und
 * anschließende bedingtes bitweises XOR Operation mit {1b} implementiert,
 * wenn b AND 0x80 den Wert 0x80 ergibt.
 *
 * @param b Element, welches multipiziert werden soll.
 * @return Das Ergebnis der Multiplikation.
 */
byte AESMath::xtime(byte b)
{
    /*************************************************************************
     * Aufgabe 3
     *************************************************************************/

    // Implementierung nach Folie 24 (AES)
    byte r = (b << 1);

    if ((b & 0x80) == 0x80)
        r = r ^ 0x1B;

    return r;
}

string AESMath::format(byte b)
{
    ostringstream os;
    //    os << "{" << hex << setfill('0') << setw(2) << (unsigned)b <<"}";
    os << "\\mathtt{" << hex << setfill('0') << setw(2) << (unsigned)b << "}";
    return os.str();
}
