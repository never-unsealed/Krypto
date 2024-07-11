/*!
 * \file
 */
//
// Created by Christoph Karg on 12.11.18.
//

#include <string>
#include "AESState.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

AESState::AESState(const AESMath &aesmath, bool debug_mode)
    : debug_mode(debug_mode), math((const AESMath *)&aesmath)
{
    for (size_t i = 0; i < 16; i++)
    {
        state[i] = 0;
    }
}

/**
 * @brief Wendet den Rundenschlüssel auf den aktuellen Zustand an.
 *
 * Diese Methode XORt den gegebenen Schlüssel mit dem aktuellen Zustand des AES-Algorithmus.
 * Der Schlüssel wird in 32-Bit-Wörter aufgeteilt und jedes Byte wird auf den Zustand angewendet.
 *
 * @param key Der Rundenschlüssel als Array von 32-Bit-Wörtern.
 */
void AESState::addKey(const word *key)
{
    /*************************************************************************
     * Aufgabe 19
     *************************************************************************/

    // Iteriere über die 4 32-Bit-Wörter des Schlüssels
    for (int i = 0; i < 4; i++)
    {
        word currentWord = key[i]; // Hole das aktuelle 32-Bit-Wort aus dem Schlüssel
        // Iteriere über die 4 Bytes des aktuellen 32-Bit-Worts
        for (int j = 0; j < 4; j++)
        {
            // XOR das Byte aus dem Schlüssel mit dem entsprechenden Byte im Zustand
            state[i * 4 + j] ^= (currentWord >> (8 * (3 - j))) & 0xFF;
        }
    }
}

string AESState::format() const
{
    ostringstream os;

    for (size_t i = 0; i < 16; i++)
    {
        os << setw(2) << setfill('0') << hex << (unsigned)state[i] << " ";
    }
    return os.str();
}

/**
 * @brief Kopiert den aktuellen Zustand in das angegebene Ausgabe-Array.
 *
 * Diese Methode kopiert die 16 Bytes des aktuellen Zustands (state) in das angegebene
 * Ausgabe-Array (out).
 *
 * @param out Ein Zeiger auf das Ausgabe-Array, in das der Zustand kopiert werden soll.
 */
void AESState::get(byte *out)
{
    /*************************************************************************
     * Aufgabe 14b
     *************************************************************************/

    memcpy(out, state, 16); // Kopiere 16 bytes von state zu out
}

byte AESState::getCell(size_t row, size_t col)
{
    return state[row + 4 * col];
}

void AESState::setCell(size_t row, size_t col, byte b)
{
    state[row + 4 * col] = b;
}

/**
 * @brief Wendet die S-Box-Substitution auf den aktuellen Zustand an.
 *
 * Diese Methode ersetzt jedes Byte im Zustand durch das entsprechende Byte aus der S-Box.
 * Sie verwendet eine Schleife, um über alle 16 Bytes im Zustand zu iterieren. 
 * Für jedes Byte wird die sBox-Methode des math-Objekts aufgerufen.
 * Das Ergebnis der sBox-Funktion wird verwendet, um das aktuelle Byte im Zustand state zu ersetzen.
 */
void AESState::subBytes()
{
    /*************************************************************************
     * Aufgabe 17a
     *************************************************************************/

    for (int i = 0; i < 16; i++)
        state[i] = math->sBox(state[i]); // Ersetze jedes Byte im Zustand durch das entsprechende Byte aus der S-Box
}

/**
 * @brief Wendet die inverse S-Box-Substitution auf den aktuellen Zustand an.
 *
 * Diese Methode ersetzt jedes Byte im Zustand durch das entsprechende Byte aus der inversen S-Box.
 * Sie verwendet eine Schleife, um über alle 16 Bytes im Zustand zu iterieren. 
 * Für jedes Byte wird die invSBox-Methode des math-Objekts aufgerufen.
 * Das Ergebnis der sBox-Funktion wird verwendet, um das aktuelle Byte im Zustand state zu ersetzen.
 */
void AESState::invSubBytes()
{
    /*************************************************************************
     * Aufgabe 17b
     *************************************************************************/

    for (int i = 0; i < 16; i++)
        state[i] = math->invSBox(state[i]); // Ersetze jedes Byte im Zustand durch das entsprechende Byte aus der inversen S-Box
}

/**
 * @brief Setzt den Zustand auf die angegebenen Werte.
 *
 * Diese Methode kopiert 16 Bytes von dem gegebenen Eingabe-Array in den Zustand.
 *
 * @param in Ein Zeiger auf das Eingabe-Array, das die neuen Zustandswerte enthält.
 */
void AESState::set(const byte *in)
{
    /*************************************************************************
     * Aufgabe 14a
     *************************************************************************/
    memcpy(state, in, 16); // Kopiere 16 Bytes von in in das state-Array
}

/**
 * @brief Wendet die MixColumns-Transformation auf den Zustand an.
 *
 * Diese Methode transformiert jede Spalte des Zustands durch Matrixmultiplikation
 * mit einer festen Matrix im Galois-Feld GF(2^8).
 * Zunächst wird eine 4x4-Matrix matrix definiert, die die MixColumns-Matrix repräsentiert.
 * Ein temporäres Array tmp wird erstellt, um die aktuellen Spaltenwerte zu speichern. 
 * Die äußere Schleife iteriert über jede Spalte (col) des Zustands. 
 * In der inneren Schleife werden die Werte der aktuellen Spalte in tmp gespeichert, indem die getCell-Methode verwendet wird.
 * Für jede Spalte wird dann die MixColumns-Transformation angewendet, 
 * indem jede Zeile der aktuellen Spalte mit der MixColumns-Matrix multipliziert wird. 
 * Dies geschieht in einer weiteren Schleife über die Zeilen (row) der Matrix. 
 * In dieser innersten Schleife wird für jedes Element der Zeile die 
 * GF-Multiplikation (math->mul) angewendet und das Ergebnis mit current XOR-verknüpft.
 * Das berechnete Ergebnis current wird dann mit der setCell-Methode zurück in den Zustand geschrieben, 
 * wobei das transformierte Ergebnis die ursprünglichen Werte der Spalte ersetzt.
 */
void AESState::mixColumns()
{
    /*************************************************************************
     * Aufgabe 18a
     *************************************************************************/

    // Definiere die MixColumns-Matrix
    byte matrix[4][4] = {
        {2, 3, 1, 1},
        {1, 2, 3, 1},
        {1, 1, 2, 3},
        {3, 1, 1, 2}};

    byte tmp[4]; // Temporäres Array zur Speicherung der Spaltenwerte

    for (int col = 0; col < 4; col++) // Iteriere über jede Spalte
    {
        for (int row = 0; row < 4; row++)
        {
            tmp[row] = getCell(row, col); // Speichere die aktuelle Spalte in tmp
        }

        // Multipliziere die aktuelle Spalte mit der MixColumns-Matrix
        for (int row = 0; row < 4; row++)
        {
            byte current = 0;
            for (int k = 0; k < 4; k++)
            {
                current ^= math->mul(matrix[row][k], tmp[k]); // Galois-Feld Multiplikation und XOR
            }
            setCell(row, col, current); // Setze das Ergebnis zurück in den Zustand
        }
    }
}

/**
 * @brief Wendet die inverse MixColumns-Transformation auf den Zustand an.
 *
 * Diese Methode transformiert jede Spalte des Zustands durch Matrixmultiplikation
 * mit einer festen inversen Matrix im Galois-Feld GF(2^8).
 * Zunächst wird eine 4x4-Matrix matrix definiert, die die inverse MixColumns-Matrix repräsentiert.
 * Ein temporäres Array tmp wird erstellt, um die aktuellen Spaltenwerte zu speichern. 
 * Die äußere Schleife iteriert über jede Spalte (col) des Zustands. 
 * In der inneren Schleife werden die Werte der aktuellen Spalte in tmp gespeichert, indem die getCell-Methode verwendet wird.
 * Für jede Spalte wird dann die MixColumns-Transformation angewendet, 
 * indem jede Zeile der aktuellen Spalte mit der inversen MixColumns-Matrix multipliziert wird. 
 * Dies geschieht in einer weiteren Schleife über die Zeilen (row) der Matrix. 
 * In dieser innersten Schleife wird für jedes Element der Zeile die 
 * GF-Multiplikation (math->mul) angewendet und das Ergebnis mit current XOR-verknüpft.
 * Das berechnete Ergebnis current wird dann mit der setCell-Methode zurück in den Zustand geschrieben, 
 * wobei das transformierte Ergebnis die ursprünglichen Werte der Spalte ersetzt.
 */
void AESState::invMixColumns()
{
    /*************************************************************************
     * Aufgabe 18b
     *************************************************************************/

    // Definiere die inverse MixColumns-Matrix
    byte matrix[4][4] = {
        {14, 11, 13, 9},
        {9, 14, 11, 13},
        {13, 9, 14, 11},
        {11, 13, 9, 14}};

    byte tmp[4]; // Temporäres Array zur Speicherung der Spaltenwerte

    for (int col = 0; col < 4; col++) // Iteriere über jede Spalte
    {
        for (int row = 0; row < 4; row++)
        {
            tmp[row] = getCell(row, col); // Speichere die aktuelle Spalte in tmp
        }
        // Multipliziere die aktuelle Spalte mit der inversen MixColumns-Matrix
        for (int row = 0; row < 4; row++)
        {
            byte current = 0;
            for (int k = 0; k < 4; k++)
            {
                current ^= math->mul(matrix[row][k], tmp[k]); // Galois-Feld Multiplikation und XOR
            }
            setCell(row, col, current); // Setze das Ergebnis zurück in den Zustand
        }
    }
}

/**
 * @brief Verschiebt die Elemente einer bestimmten Zeile im Zustand um eine gegebene Anzahl von Positionen.
 *
 * Diese Methode verschiebt die Elemente einer bestimmten Zeile (row) im Zustand um eine gegebene Anzahl
 * von Positionen (shift). Die Verschiebung erfolgt zyklisch.
 * Ein temporäres Array row_content wird erstellt, um die verschobenen Werte der Zeile zu speichern.
 * Zuerst wird überprüft, ob shift gleich 0 ist. 
 * Wenn dies der Fall ist, wird die Methode sofort verlassen, da keine Verschiebung erforderlich ist.
 * Ansonsten werden die Werte der Zeile zyklisch verschoben, 
 * indem jedes Element der Zeile von seiner aktuellen Position um shift nach links verschoben wird. 
 * Das Ergebnis der Verschiebung wird im temporären Array row_content gespeichert. Die Berechnung (i + shift) % 4 stellt sicher, 
 * dass die Werte zyklisch verschoben werden, sodass das letzte Element auf die erste Position verschoben 
 * wird und alle anderen entsprechend nach rechts wandern.
 * Schließlich werden die verschobenen Werte aus dem 
 * temporären Array row_content zurück in die entsprechenden Positionen der 
 * Zeile im Zustand geschrieben, indem die setCell-Methode verwendet wird.
 *
 * @param row Die Zeile im Zustand, die verschoben werden soll.
 * @param shift Die Anzahl der Positionen, um die die Zeile verschoben werden soll.
 */
void AESState::shiftRow(size_t row, size_t shift)
{
    /*************************************************************************
     * Aufgabe 15
     *************************************************************************/
    byte row_content[4]; // Temporäres Array zur Speicherung des Inhalts der Zeile

    if (!shift)
        return; // Wenn shift 0 ist, keine Verschiebung notwendig

    // Kopiere die verschobenen Werte in das temporäre Array
    for (int i = 0; i < 4; i++)
        row_content[i] = getCell(row, (i + shift) % 4);

    // Setze die verschobenen Werte zurück in den Zustand
    for (int i = 0; i < 4; i++)
        setCell(row, i, row_content[i]);
}

/**
 * @brief Wendet die ShiftRows-Transformation auf den Zustand an.
 *
 * Diese Methode verschiebt die Elemente jeder Zeile im Zustand.
 * Die Methode nutzt eine Schleife:
 * Für jede Iteration von i (von 0 bis 3) wird die Methode shiftRow aufgerufen, 
 * wobei sowohl der Index der Zeile als auch die Anzahl der Positionen, um die sie verschoben werden soll, 
 * gleich sind. Die Verschiebung erfolgt zyklisch, was bedeutet, dass die letzten Elemente der Zeile auf die
 * ersten Positionen verschoben werden und alle anderen entsprechend nach rechts wandern.
 */
void AESState::shiftRows()
{
    /*************************************************************************
     * Aufgabe 16a
     *************************************************************************/

    for (int i = 0; i < 4; i++)
        shiftRow(i, i); // Verschiebt die i-te Zeile um i Positionen
}

/**
 * @brief Wendet die inverse ShiftRows-Transformation auf den Zustand an.
 *
 * Diese Methode verschiebt die Elemente jeder Zeile im Zustand in die entgegengesetzte Richtung
 * entsprechend der Zeilennummer.
 * Die Methode nutzt eine Schleife:
 * Für jede Iteration von i (von 0 bis 3) wird die Methode shiftRow aufgerufen, 
 * wobei sowohl der Index der Zeile als auch die Anzahl der Positionen, um die sie verschoben werden soll (jeweils um 4 - i Positionen), 
 * gleich sind. Die Verschiebung erfolgt zyklisch, was bedeutet, dass die letzten Elemente der Zeile auf die
 * ersten Positionen verschoben werden und alle anderen entsprechend nach rechts wandern.
 */
void AESState::invShiftRows()
{
    /*************************************************************************
     * Aufgabe 16b
     *************************************************************************/

    for (int i = 1; i < 4; i++)
        shiftRow(i, 4 - i); // Verschiebt die i-te Zeile um 4 - i Positionen
}
