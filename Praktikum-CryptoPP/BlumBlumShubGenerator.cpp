#include "BlumBlumShubGenerator.h"

/**
 * @brief Initialisiert den BlumBlumShub Zufallszahlengenerator.
 *
 * Initialisiert ein Objekt vom Typ BlumBlumShubGenerator mit einem Modul und Seed.
 * @param n Modul fuer den Zufallszahlengenerator.
 * @param seed Ausgangswert zur Generierung der Zufallszahlen.
 */
BlumBlumShubGenerator::BlumBlumShubGenerator(const Integer &n, const Integer &seed) {
    this->n = n;
    this->s = (seed * seed) % n;
}

/**
 * @brief Gibt ein pseudozufaelliges Bit zurueck.
 *
 * Nutzt den BlumBlumShub-Algorithmus, um ein pseudozufaelliges Bit zu erzeugen.
 * @return Variable, die entweder den Wert 1 oder 0 enthalten kann.
 */
bool BlumBlumShubGenerator::getBit() {
    /*************************************************************************
     * Aufgabe 6b.
     *************************************************************************/

    // Berechnung eines Zufallsbits anhand des Seeds mit der Formel der Vorlesung.
    // Abspeichern des neuen Wertes von s.
    s = (s*s) % n;     

    return s % 2; // Ein Bit kann nur entweder 1 oder 0 sein.
}

