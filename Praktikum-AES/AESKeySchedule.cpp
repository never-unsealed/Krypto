/*!
 * \file
 */
//
// Created by Christoph Karg on 14.11.18.
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include "AESKeySchedule.h"

using namespace std;

/**
 * @brief Konstruktor zur Initialisierung des AESKeySchedule.
 *
 * Dieser Konstruktor initialisiert die notwendigen Variablen und erstellt die Rundkonstanten (RCON),
 * die für den Key-Map des AES-Algorithmus benötigt werden.
 *
 * @param aesmath Referenz auf ein AESMath-Objekt, das die notwendigen mathematischen Operationen bereitstellt.
 * @param debug_mode Boolescher Wert, der angibt, ob der Debug-Modus aktiviert ist.
 */
AESKeySchedule::AESKeySchedule(const AESMath &aesmath, bool debug_mode)
    : debug_mode(debug_mode), math((const AESMath *)&aesmath), key_schedule(), nk(0),
      nb(4), nr(0), r_con(11, 0)
{

    /*************************************************************************
     * Aufgabe 10
     *************************************************************************/

    r_con[0] = 0;  // Setzen des initialen Wertes von r_con[0] auf 0
    byte rcon = 1; // Initialisierung von rcon mit 1

    for (int i = 1; i < 11; i++) // Schleife zum Setzen der ersten 10 RCON-Werte
    {
        r_con[i] = rcon << 24;       // Setzen des RCON-Werts an die höchste Stelle des 32-Bit-Worts
        rcon = aesmath.mul(rcon, 2); // Multiplikation von rcon mit 2 im endlichen Feld GF(2^8)
    }
}

AESKeySchedule::~AESKeySchedule()
{
    //    if (key_schedule != NULL) {
    //        delete key_schedule;
    //    }
}

// AK_STRIP_BEGIN
// void AESKeySchedule::addRoundKey(AESState& state, size_t round) {
//    assert(round<=nr);
//    if (debug_mode) {
//        cout << "round[" << dec << setw(2) << setfill(' ') << round << "].k_sch    ";
//        for (size_t i=0; i<4; i++) {
//            cout << hex << setw(8) << setfill('0') << key_schedule[4*round+i];
//        }
//        cout << endl;
//    }
//
//    state.addKey(&key_schedule[4*round]);
//}

size_t AESKeySchedule::getNrOfRounds() const
{
    return nr;
}

const word *AESKeySchedule::getRoundKey(size_t round)
{
    assert(round <= nr);
    return &key_schedule[4 * round];
}

string AESKeySchedule::formatRoundKey(size_t round)
{
    assert(round <= nr);
    ostringstream os;
    for (size_t i = 0; i < nb; i++)
    {
        os << hex << setw(8) << setfill('0') << key_schedule[4 * round + i];
    }

    return os.str();
    /*
    string s = os.str();
    string d("");
    for (size_t i=0; i<4*nb; i++) {
        d = d + s.substr(2*i,2) + " ";
    }

    return d;
     */
}

/**
 * @brief Führt eine zyklische Linksverschiebung durch.
 *
 * Speichert die oberen 8 Bits in w und führt eine Linksverschiebung um 8 bits durch.
 * Die gespeicherten Bits werden dem Ergebnis als untere 4 Bits angefügt.
 * Genauer extrahiert der Algorithmus die oberen 8 Bits eines 32-Bit-Worts w, 
 * indem es w um 24 Bit nach rechts verschiebt und das Ergebnis in upper_bits speichert. 
 * Danach wird w um 8 Bit nach links verschoben, um Platz für die unteren 8 Bits zu schaffen. 
 * Diese unteren 8 Bits werden dann mit den zuvor extrahierten oberen 8 Bits kombiniert und zurückgegeben. 
 * Der Algorithmus ermöglicht somit eine zirkuläre Verschiebung der 32-Bit-Werte, indem die oberen Bits an die unteren Bits angehängt werden.
 *
 * @param w 32-bit breite Zahl, für die die Linksverschiebung durchgeführt werden soll.
 * @return 32-bit breite Zahl, die das Ergebnis der zyklischen Linksverschiebung speichert.
 */
word AESKeySchedule::rotWord(word w)
{
    /*************************************************************************
     * Aufgabe 11
     *************************************************************************/
    byte upper_bits;

    upper_bits = w >> 24; // Oberen 8 bits abspeichern

    w <<= 8;         // Linksverschiebung um 8 bits
    w |= upper_bits; // Hinzufügen der vorher oberen 8 bits nun als untere 8 bits

    return w;
}

/**
 * @brief Setzt den Schlüssel für den AES-Algorithmus und generiert den Key-Map.
 *
 * Diese Methode initialisiert den Key-Map basierend auf dem gegebenen Schlüssel.
 * Der Schlüssel wird in 32-Bit-Wörter aufgeteilt und durchläuft den Schlüsselerweiterungsprozess.
 * Zunächst wird die Anzahl der 32-Bit-Wörter nk im Schlüssel berechnet, basierend auf der Schlüsselgröße in Bytes. 
 * Anhand von nk wird die Anzahl der Runden nr festgelegt: 10 Runden für 128-Bit-Schlüssel, 12 Runden für 
 * 192-Bit-Schlüssel und 14 Runden für 256-Bit-Schlüssel.
 * Der Key-Map wird mit den initialen Schlüsselwörtern initialisiert, indem die Bytes des Schlüssels in 32-Bit-Wörter 
 * konvertiert und zum Key-Map hinzugefügt werden. Anschließend wird der Key-Map durch Transformationen erweitert, 
 * bis er die erforderliche Größe für alle Runden erreicht.
 * Während dieser Erweiterungsschritte werden verschiedene Transformationen auf die vorherigen Schlüsselwörter angewendet, 
 * darunter Rotationen (rotWord), Substitutionen (subWord) und XOR-Operationen mit Rundkonstanten (r_con). 
 * Je nach Position im Key-Map und Debug-Modus werden die Zwischenergebnisse detailliert ausgegeben.
 * Die Methode liefert true zurück, wenn die Schlüsselerstellung 
 * erfolgreich war, andernfalls false bei ungültiger Schlüsselgröße.
 *
 * @param key Der zu setzende Schlüssel als Vektor von Bytes.
 * @return true Wenn der Schlüssel erfolgreich gesetzt wurde.
 * @return false Wenn die Schlüsselgröße ungültig ist.
 */
bool AESKeySchedule::setKey(const vector<byte> &key)
{
    /*************************************************************************
     * Aufgabe 13
     *************************************************************************/
    word current;               // Temporäre Variable zum Speichern der aktuellen Wortoperation
    nk = (key.size() * 8) / 32; // Anzahl der 32-Bit-Wörter im Schlüssel basierend auf der Schlüsselgröße

    // Bestimmen der Anzahl der Runden (nr) basierend auf der Anzahl der Schlüsselwörter (nk)
    switch (nk)
    {
    case 4:      // 128-Bit-Schlüssel (4 * 32 Bit)
        nr = 10; // 10 Runden
        break;

    case 6:      // 192-Bit-Schlüssel (6 * 32 Bit)
        nr = 12; // 12 Runden
        break;

    case 8:      // 256-Bit-Schlüssel (8 * 32 Bit)
        nr = 14; // 14 Runden
        break;

    default: // Ungültige Schlüsselgröße
        return false;
    }

    if (debug_mode)
    {
        // Tabellenspalten "erstellen"
        cout << string(127, '-') << endl;
        cout << setw(6) << left << "| i(dec)" << " | "
             << setw(8) << left << "temp" << " | "
             << setw(15) << left << "After RotWord()" << " | "
             << setw(15) << left << "After SubWord()" << " | "
             << setw(10) << left << "Rcon[i/Nk]" << " | "
             << setw(19) << left << "After XOR with Rcon" << " | "
             << setw(8) << left << "w[i-Nk]" << " | "
             << setw(20) << left << "w[i]=temp XOR w[i-Nk] |" << endl;
    }

    // Initialisieren des Key-Maps mit den Originalschlüssel-Words (1. Schlüssel)
    for (int i = 0; i < nk; i++)
    {
        current = 0; // Zurücksetzen der tmp Variable

        // Zusammensetzen der 32-Bit-Wörter aus dem Byte-Array
        for (int k = 0; k < 4; k++)
            current |= key[4 * i + k] << ((3 - k) * 8);
        key_schedule.push_back(current); // Hinzufügen des aktuellen Wortes zum Key-Map
    }

    // Erweitern des Key-Maps bis zur erforderlichen Größe für alle Runden
    for (int i = nk; i < nb * (nr + 1); i++)
    {
        current = key_schedule[i - 1]; // Holen des letzten Worts aus dem Key-Map

        if (debug_mode)
        {
            cout << string(127, '-') << endl;
            cout << "| " << setw(6) << left << i << " | ";
            cout << right << std::setfill('0') << hex << setw(8) << current << left;
        }

        // Anwendung der Key-Map-Transformationen
        if (!(i % nk)) // Wenn i ein Vielfaches von nk ist
        {
            if (debug_mode)
            {
                cout << right << std::setfill('0') << " | " << setw(15) << setfill(' ') << rotWord(current)
                     << " | " << setw(15) << setfill(' ') << subWord(rotWord(current))
                     << " | " << setw(10) << setfill(' ') << r_con[i / nk]
                     << " | " << setw(19) << setfill(' ') << (subWord(rotWord(current)) ^ r_con[i / nk]) << left;
            }

            current = subWord(rotWord(current)) ^ r_con[i / nk]; // Rotations- und Substitutionsoperationen plus Rundkonstante
        }
        else if (nk > 6 && (i % nk) == 4) // Wenn nk > 6 und i modulo nk gleich 4 ist
        {
            current = subWord(current); // Nur Substitutionsoperation

            if (debug_mode)
            {
                cout << " | " << setw(15) << setfill(' ') << " "
                     << " | " << setw(15) << setfill(' ') << current;
            }
        }
        else if (debug_mode)
        {
            cout << " | " << setw(15) << setfill(' ') << " "
                 << " | " << setw(15) << setfill(' ') << " " // Keine Ausgabe...
                 << " | " << setw(10) << setfill(' ') << " "
                 << " | " << setw(19) << setfill(' ') << " ";
        }

        // Berechnen und Hinzufügen des neuen Worts zum Key-Map
        key_schedule.push_back(key_schedule[i - nk] ^ current);

        if (debug_mode)
        {
            cout << right << std::setfill('0') << " | " << setw(8) << setfill(' ') << key_schedule[i - nk]
                 << " | " << setw(21) << setfill(' ') << (key_schedule[i - nk] ^ current) << left << " |" << endl;
        }
    }

    if (debug_mode)
        cout << string(127, '-') << endl;

    return true;
}

/**
 * @brief Führt Byte-Substitution für die Bytes eines 32-bit breiten WORDs durch.
 *
 * Die Methode iteriert über jedes Byte in w und ruft mit diesem Wert die SBOX
 * Methode auf. Anschließend wird der zurückgegebene Wert an der passenden
 * Position im Rückgabewert gespeichert.
 * 
 * In diesem Algorithmus wird ein 32-Bit-Wort w in seine einzelnen Bytes aufgeteilt.
 * Zunächst wird ein Zeiger byte_arr auf das erste Byte von w gesetzt. 
 * Ein neues 32-Bit-Wort ret_val wird initialisiert, das am Ende zurückgegeben wird.
 * Der Algorithmus durchläuft eine Schleife viermal, da ein 32-Bit-Wort aus vier Bytes besteht. 
 * In jeder Iteration wird die sBox-Funktion auf das aktuelle Byte von w angewendet, das durch den Zeiger byte_arr[i] zugänglich gemacht wird. 
 * Das Ergebnis der sBox-Funktion wird an der entsprechenden Position im ret_val gespeichert, das schließlich zurückgegeben wird.
 * Dieser Algorithmus ermöglicht eine Byte-weise Transformation eines 32-Bit-Wortes unter 
 * Verwendung einer Substitutionsbox (sBox), wobei das Ergebnis in einem neuen 32-Bit-Wort gesammelt und zurückgegeben wird.
 *
 * @param w 32-bit breite Zahl, für die die Byte-Substitution durchgeführt werden soll.
 * @return 32-bit breite Zahl, die das Ergebnis der Substitution speichert.
 */
word AESKeySchedule::subWord(word w) const
{
    /*************************************************************************
     * Aufgabe 12
     *************************************************************************/
    char *byte_arr = (char *)&w;
    word ret_val = 0;

    for (int i = 0; i < 4; i++)                          // 4 Iterationen für 4 Bytes
        ((char *)&ret_val)[i] = math->sBox(byte_arr[i]); // Rufe sBox für das aktuelle Byte auf und speichere das Ergebnis an der jeweiligen Position in ret_val

    return ret_val;
}
