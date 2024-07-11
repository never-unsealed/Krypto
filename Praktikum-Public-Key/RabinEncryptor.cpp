#include "RabinEncryptor.h"

/**
 * @brief Konstruktor für die Klasse RabinEncryptor.
 *
 * Initialisiert die Instanz mit den gegebenen Parametern und setzt das Padding.
 *
 * @param n Ein Modulus für die Rabin-Verschlüsselung.
 * @param padding Eine Ganzzahl, die für die Rabin-Padding-Anforderung verwendet wird.
 */
RabinEncryptor::RabinEncryptor(const Integer &n, const Integer &padding)
{
    /******************************************************************************
     * Aufgabe 11a  & 12a
     * 11a fertig
     ******************************************************************************/

    // Initialisierung der Klassenvariablen
    this->n = n;
    this->padding = padding;
}

RabinEncryptor::~RabinEncryptor()
{
}

/**
 * @brief Verschlüsselt den Klartext x mit dem Rabin-Verschlüsselungsalgorithmus.
 *
 * Berechnet den Geheimtext y, indem x quadratisch genommen und modulo n berechnet wird.
 * Überprüft zuerst, ob x ein gültiger Klartext ist (kleiner als n).
 *
 * @param x Der zu verschlüsselnde Klartext.
 * @param y Referenz auf eine Variable vom Typ Integer, in der der Geheimtext gespeichert wird.
 * @return true, wenn die Verschlüsselung erfolgreich war (x war ein gültiger Klartext), sonst false.
 */
bool RabinEncryptor::compute(const Integer &x, Integer &y)
{
    /******************************************************************************
     * Aufgabe 11b
     ******************************************************************************/

    // Überprüfen, ob x ein passender Klartext ist
    // Eine einfache Bedingung könnte sein, dass x kleiner als n ist

    if (x >= n || x < 0)
    {
        return false; // x ist kein passender Klartext
    }

    // Rabin-Verschlüsselung: y = x^2 mod n
    y = (x * x) % n;

    return true;
}

/**
 * @brief Verschlüsselt einen Text mit Padding.
 *
 * Diese Methode initialisiert die Variablen j und i auf 1 bzw. 0.
 * Anschließend wird die Anzahl der Ziffern in der Variablen padding berechnet.
 * Dies geschieht durch eine Schleife, die eine Kopie von padding (temp) iterativ durch 10 dividiert, bis temp null ist.
 * Für jede Iteration wird i um eins erhöht, wodurch die Anzahl der Ziffern in padding bestimmt wird.
 * Danach wird die Variable j in einer weiteren Schleife für jede Ziffer um den Faktor 10 multipliziert.
 * Diese Schleife läuft so oft, wie i groß ist. Schließlich wird die Klassenvariable k um den Wert von padding erweitert.
 * Dies wird erreicht, indem k zunächst mit j multipliziert und dann der Wert von padding hinzugefügt wird.
 * Der resultierende Wert von k enthält somit padding am Ende seiner ursprünglichen Ziffern.
 * Abschließend wird die Methode compute aufgerufen.
 *
 * @param x Der zu verschlüsselnde Text.
 * @param y Erhält den verschlüsselten Text.
 * @return true, wenn erfolgreich, ansonsten false.
 */
bool RabinEncryptor::compute2(const Integer &x, Integer &y)
{
    /******************************************************************************
     * Aufgabe 12b
     ******************************************************************************/

    Integer j = 1; // Initialisiere j auf 1
    Integer i = 0; // Initialisiere i auf 0

    // Berechne die Anzahl der Ziffern in padding
    for (Integer temp = padding; temp != 0; temp /= 10)
    {
        i++;
    }

    // Multipliziere j mit 10 für jede Ziffer in padding
    for (Integer w = 0; w < i; w++)
    {
        j *= 10;
    }

    // Hänge padding an x an
    Integer new_x = (x * j) + padding;

    // Rufe die compute Methode auf
    return compute(new_x, y);
}
