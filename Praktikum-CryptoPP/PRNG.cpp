#include "PRNG.h"

/**
 * @brief Gibt ein pseudozufaelliges Byte zurueck.
 *
 * Nutzt den BlumBlumShubGenerator, um ein Byte mit zufaelligen Bits zu erstellen.
 * @return 8-bit breite Zufallszahl.
 */
byte PRNG::getByte()
{
    /*************************************************************************
     * Aufgabe 6d.
     *************************************************************************/
    byte b = 0;

    // 8 Schleifendurchlaeufe, um 8 Zufallsbits zu generieren.
    for (int i = 7; i >= 0; i--)
        b |= this->getBit() << i; // Das generierte Bit an die jeweilige Position shiften.

    return b;
}

/**
 * @brief Fuellt einen Puffer mit beliebiger Anzahl an Pseudozufallsbytes.
 *
 * Nutzt den BlumBlumShubGenerator, um einen Puffer der groesse n mit Pseudozufallsbytes zu fuellen.
 * @param buffer Der Puffer, der die Pseudozufallsbytes erhaelt.
 * @param n Die goesse des Puffers buffer.
 */
void PRNG::getBlock(byte *buffer, unsigned int n)
{
    /*************************************************************************
     * Aufgabe 6e.
     *************************************************************************/

    // n Schleifendurchlaeufe, um n Zufallsbytes zu generieren.
    for (unsigned int i = 0; i < n; i++)
        buffer[i] = this->getByte(); // Schreibe das Byte in das Ausgabearray.
}

/**
 * @brief Erstellt eine Pseudozufallszahl vom Typ Integer.
 *
 * Nutzt den BlumBlumShubGenerator, eine Pseudozufallszahl vom Typ Integer mit beliebiger Laenge zu erstellen.
 * @param size Die groesse der gewuenschten Zufallszahl in Bytes.
 * @return Die Zufallszahl als Integer.
 */
Integer PRNG::getInteger(unsigned int size)
{
    /*************************************************************************
     * Aufgabe 6g.
     *************************************************************************/
    byte *block = new byte[size]; // Neues Byte Array mit size Bytes.

    this->getBlock(block, size); // Fuelle das Array mit Zufallsbits.
    Integer result(block, size); // Initialisiere den Integer mit dem Array.

    return result;
}

/**
 * @brief Erstellt eine Pseudozufallszahl vom Typ Integer, die kleiner als m sein muss.
 *
 * Nutzt den BlumBlumShubGenerator, eine Pseudozufallszahl vom Typ Integer zu erstellen.
 * Die zurueckgegebene Zahl ist hierbei immer Teil der Menge {0, 1, . . . , m−1}
 * @param m Der vorgegebene Wertebereich fuer die Ausgabe.
 * @return Die Zufallszahl als Integer.
 */
Integer PRNG::getInteger(const Integer &m)
{
    /*************************************************************************
     * Aufgabe 6i.
     *************************************************************************/
    Integer result;

    do
    {

        result = this->getInteger(m.ByteCount()); // Generiere Integer mit dem Byte Count von m.

    } while (result >= m); // Da die Zahl gleichverteilt sein muss, erneuter Versuch, wenn diese nicht in der Menge {0, 1, . . . , m−1} ist.

    return result;
}
