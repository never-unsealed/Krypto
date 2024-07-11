/*!
 * \file
 */
#include <cassert>
#include <iomanip>
#include "DESCipher.h"
#include <cstring>

DESCipher::DESCipher()
{
}

DESCipher::~DESCipher()
{
}

/**
 * @brief Berechnet die 16 Rundenschlüssel für den DES-Algorithmus.
 *
 * Diese Funktion berechnet die 16 Rundenschlüssel, die in den Verschlüsselungs- oder Entschlüsselungsrunden des DES-Algorithmus verwendet werden. 
 * Die Rundenschlüssel werden basierend auf dem gegebenen Schlüssel generiert und je nach Modus in normaler oder umgekehrter Reihenfolge gespeichert.
 * Der DES-Algorithmus verwendet diese Rundenschlüssel, um den Klartext in Geheimtext zu verwandeln oder umgekehrt.
 * 
 * @param key Ein Zeiger auf das Byte-Array, das den Schlüssel enthält. Der Schlüssel muss 64 Bits (8 Bytes) lang sein. 
 *            Der Schlüssel darf nicht null sein und sollte die Paritätsbits enthalten.
 * @param encmode Ein boolescher Wert, der den Modus bestimmt. Wenn `true`, werden die Rundenschlüssel in der normalen Reihenfolge gespeichert, 
 *                was für die Verschlüsselung verwendet wird. Wenn `false`, werden die Rundenschlüssel in umgekehrter Reihenfolge gespeichert,
 *                was für die Entschlüsselung verwendet wird.
 */
void DESCipher::computeKeySchedule(const byte *key, bool encmode)
{
        /******************************************************************************
         * Aufgabe 5
         ******************************************************************************/

        byte current[16][48], jpc2, jls;

        for (unsigned int i = 0; i < 16; i++)   // Schleife über die 16 Runden zur Generierung der Rundenschlüssel
        {
                for (unsigned int j = 0; j < 48; j++)   // Schleife über die 48 Bits des aktuellen Rundenschlüssels

                {
                        // Berechnung nach Formel in Folie 33

                        jpc2 = pc2[j];

                        if (jpc2 <= 28)
                        {
                                jls = (jpc2 + total_rot[i] - 1) % 28;
                        }
                        else
                        {
                                jls = 28 + ((jpc2 - 28 + total_rot[i] - 1) % 28);
                        }

                        current[i][j] = pc1[jls];
                }
                        // Permutieren und Speichern des Rundenschlüssels
                        // Die Funktion 'permutate' führt die Permutation durch und füllt die Rundenschlüssel
                        // 'encmode' bestimmt, ob die Schlüssel für die Verschlüsselung oder Entschlüsselung berechnet werden
                permutate(current[i], 48, key, 64, key_schedule[encmode ? i : 15 - i], 48);
        }
}

/**
 * @brief Gibt ein Byte aus SBox zurück.
 *
 * Diese Funktion berechnet den Wert eines Eintrags in einer Substitutionsbox (S-Box) anhand der gegebenen ID, Zeile und Spalte.
 * S-Boxen sind ein wesentlicher Bestandteil des DES-Algorithmus und dienen zur Substitution während der Verschlüsselung.
 * Jede S-Box ist eine 4x16-Matrix.
 * @param id Die ID der S-Box, die von 1 bis 8 reicht. Es gibt insgesamt 8 S-Boxen im DES-Algorithmus.
 * @param line Die Zeile in der jeweiligen S-Box, die einen Wert von 0 bis 3 haben kann.
 * @param col Die Spalte in der jeweiligen S-Box, die einen Wert von 0 bis 15 haben kann.
 * @return Den Wert des Bytes an der angegebenen Position in der S-Box.
 */
byte DESCipher::computeSBox(byte id, byte line, byte col)
{
        /******************************************************************************
         * Aufgabe 6a
         ******************************************************************************/

        return sbox[id][(line * 16) + col]; //Der Wert aus der jeweiligen SBox
}

/**
 * @brief Entschlüsselt den verschlüsselten Text
 *
 * Diese Funktion entschlüsselt den verschlüsselten Text basierend auf dem gegebenen Schlüssel.
 * Sie verwendet den DES-Algorithmus, um die verschlüsselten Daten zu entschlüsseln.
 * @param cipher_text Der verschlüsselte Text.
 * @param cipher_len Die Länge des verschlüsselten Textes.
 * @param key Der Schlüssel zur Berechnung der Rundenschlüssel.
 * @param key_len Die Länge des Schlüssels.
 * @param plain_text Der Puffer, in den der entschlüsselte Text geschrieben wird.
 * @param plain_len Die Länge des entschlüsselten Textes.
 * @return Die Länge des entschlüsselten Textes.
 */
int DESCipher::decrypt(
    const byte *cipher_text,
    int cipher_len,
    const byte *key,
    int key_len,
    byte *plain_text,
    int plain_len)
{
        /******************************************************************************
         * Aufgabe 8
         ******************************************************************************/
        // Berechnung der Rundenschlüssel für die Entschlüsselung
        // Der zweite Parameter ist `false`, was bedeutet, dass die Rundenschlüssel in umgekehrter Reihenfolge berechnet werden
        computeKeySchedule(key, false);

        for(unsigned int i = 0; i < cipher_len / block_len; i++)                // Schleife über jeden Block im verschlüsselten Text
        {
                // `cipher_text + (i * block_len)` ist der Pointer auf den aktuellen Block im verschlüsselten Text
                // `plain_text + (i * block_len)` ist der Pointer auf die Position im Klartext, wo das Ergebnis geschrieben werden soll
                processBlock(
                        cipher_text + (i * block_len),                          // Eingabe: aktueller Block des verschlüsselten Textes
                        plain_text + (i * block_len)                            // Ausgabe: aktueller Block des entschlüsselten Textes
                );
        }

        return plain_len; // Rückgabe der Länge des entschlüsselten Textes
}

/**
 * @brief Verschlüsselt den Klartext
 *
 * Diese Funktion verschlüsselt den gegebenen Klartext unter Verwendung des DES-Algorithmus. Wir verwenden einen Schlüssel, um die 16 Rundenschlüssel zu berechnen,
 * und führt die Verschlüsselung blockweise durch.
 * @param plain_text Der Klartext.
 * @param plain_len Die Länge des Klartextes.
 * @param key Der Schlüssel zur Berechnung der Rundenschlüssel.
 * @param key_len Die Länge des Schlüssels.
 * @param cipher_text Der Puffer, in den der verschlüsselte Text geschrieben wird.
 * @param cipher_len Die Länge des verschlüsselten Textes.
 * @return Die Länge des verschlüsselten Textes.
 */
int DESCipher::encrypt(
    const byte *plain_text,
    int plain_len,
    const byte *key,
    int key_len,
    byte *cipher_text,
    int cipher_len)
{
        /******************************************************************************
         * Aufgabe 8
         ******************************************************************************/

        computeKeySchedule(key, true); //Generieren Rundeneschlüssel um zu verschlüsseln

        for(unsigned int i = 0; i < plain_len / block_len; i++)  // Eine Iteration pro Block, da wir blockweise verschlüsseln und dementsprechend auch blockweise schreiben.
        {
                processBlock(
                        plain_text + (i * block_len),            // Klartext als Eingabe und verschlüsselter Text als Ausgabe
                        cipher_text + (i * block_len)
                );
        }

        return cipher_len;
}

/**
 * @brief Führt eine Feistel-Runde der 16 Runden des DES-Algorithmus aus.
 *
 * Diese Funktion führt eine Feistel-Runde aus, indem sie die linke und rechte Hälfte des Eingabeblocks sowie den Rundenschlüssel verwendet.
 * Die Funktion wendet die Funktion F auf die rechte Hälfte des Eingabeblocks an, XORt das Ergebnis mit der linken Hälfte und vertauscht die Hälften.
 *
 * @param l_in Linke Hälfte des Eingabeblocks.
 * @param r_in Rechte Hälfte des Eingabeblocks.
 * @param key Rundenschlüssel für diese Runde.
 * @param l_out Linke Hälfte des Ausgabeblocks.
 * @param r_out Rechte Hälfte des Ausgabeblocks.
 * @param rnd Aktuelle Rundenzahl.
 */
void DESCipher::feistel(
    const byte *l_in,
    const byte *r_in,
    const byte *key,
    byte *l_out,
    byte *r_out,
    int rnd)
{
        /******************************************************************************
         * Aufgabe 6c
         ******************************************************************************/

        byte inter[4];

        functionF(r_in, key, inter, rnd); //Aufruf der F-Funktion mit der rechten Hälfte des Eingabeblocks, das Ergebnis wird in inter gespeichert.

        for (unsigned int i = 0; i < 4; i++) 
        {
                l_out[i] = r_in[i]; //Wir tauschen die linke Hälfte des Ausgabeblocks mit der rechten Hälfte des Eingabeblocks.
                r_out[i] = l_in[i] ^ inter[i]; //Die rechte Hälfte des Ausgabeblocks ist das Ergebnis der XOR-Verknüpfung unserer linken Hälfte des Eingabeblocks mit inter.
        }
}

/**
 * @brief Führt die F-Funktion des DES-Algorithmus aus.
 *
 * Diese Funktion wendet die F-Funktion des DES-Algorithmus an. Dies beinhaltet das Erweitern der 32-Bit-Eingabe zu einer 48-Bit-Eingabe, das XOR mit dem Rundenschlüssel,
 * die Anwendung der S-Boxen mit jeweils 6 Bit, um jeweils 4 Bit zu erhalten, und die endgültige Permutation.
 *
 * @param r_in Die rechte Hälfte des Eingabeblocks.
 * @param key Der Rundenschlüssel für diese Runde.
 * @param r_out Die Ausgabe der F-Funktion.
 * @param rnd Die aktuelle Rundenzahl.
 */
void DESCipher::functionF(
    const byte *r_in,
    const byte *key,
    byte *r_out,
    int rnd)
{
        /******************************************************************************
         * Aufgabe 6b
         ******************************************************************************/

        byte arr[6], line, col, sboxArr[4], sboxVal;
        int pos = 0;
        bool lineFirstBit, lineSecondBit;
        bool colFirst, colSec, colThird, colFourth;

        permutate(ev, 48, r_in, 4, arr, 6); //Rechte Eingabehälfte (4 Byte) mittels des EV-Arrays um 6 Byte erweitern.

        for (unsigned int i = 0; i < 6; i++)    //Für jeden Byte, der grund wieso 6 mal weil wir mit 48 bit arbeiten
        {
                arr[i] ^= key[i]; //XOR der erweiterten Eingabehälfte mit dem Rundenschlüssel und Speicherung des Ergebnisses in arr
        }

        for (unsigned int i = 0; i < 8; i++, pos += 6)
        {
                // Extrahiere Zeile (erstes und füntes Bit)
                lineFirstBit = getBit(arr, 6, pos);
                lineSecondBit = getBit(arr, 6, pos + 5);

                // Extrahiere Spalte (zweites bis viertes Bit)
                colFirst = getBit(arr, 6, pos + 1);
                colSec = getBit(arr, 6, pos + 2);
                colThird = getBit(arr, 6, pos + 3);
                colFourth = getBit(arr, 6, pos + 4);

                // Setze die extrahierten Bits zusammen, um Reihe bzw. Spalte zu erhalten
                line = lineFirstBit << 1;
                line |= lineSecondBit;

                col = colFirst << 3;
                col |= colSec << 2;
                col |= colThird << 1;
                col |= colFourth;

                sboxVal = computeSBox(i, line, col);

                for (unsigned int x = 0; x < 4; x++)
                {
                        //Setze ermitteltes Bit in sboxArr
                        setBit(sboxArr, 4, (i * 4) + x, getBit(&sboxVal, 1, x + 4));
                }
        }

        permutate(pp, 32, sboxArr, 4, r_out, 4); //Finale Permutation mit dem ermittelten sboxArr
}

bool DESCipher::getBit(const byte *array, int array_len, int pos) const
{
        int bytepos, bitpos;
        byte b;

        assert(array_len > 0);
        assert(pos >= 0);

        bytepos = pos / 8;
        bitpos = 7 - (pos % 8);
        if (bytepos < array_len)
        {
                b = 0x01;
                b = b << bitpos;
                // out << "(" << dec << bytepos << "," << bitpos << "," << hex << (short)b << ") ";
                return ((array[bytepos] & b) == b);
        }
        else
        {
                return false;
        }
}

/**
 * @brief Permutiert die Bits in einem Array.
 *
 * Die permututate Methode nutzt die über p übergebene Tabelle um eine bitweise Permutation
 * eines Eingabearrays zu erstellen.
 * @param p Array, mit dem die Permutation des Arrays in_array bestimmt wird.
 * @param p_len Länge des p-Arrays in Bytes.
 * @param in_array Bitfolge, die permutiert werden soll.
 * @param in_len Länge des Eingabearrays in Bytes.
 * @param out_array Zeiger auf Array, das die neue Bitfolge erhält.
 * @param out_len Länge des Ausgabearrays in bytes.
 */
void DESCipher::permutate(
    const byte *p,
    int p_len,
    const byte *in_array,
    int in_len,
    byte *out_array,
    int out_len) const
{
        /******************************************************************************
         * Aufgabe 4
         ******************************************************************************/
        byte current;
        bool newBit;

        for (unsigned int i = 0; i < p_len; i++) // Iteriere für jedes Bit
        {
                current = p[i] - 1; //Position des nächsten Bits anhand von p bestimmen
                newBit = getBit(in_array, in_len, current); // Das Bit aus dem Eingabearray an der Stelle von current
                setBit(out_array, out_len, i, newBit); // Setze das ermittelte Bit in out_array
        }
}

/**
 * @brief Gibt ein Bitfeld aus.
 *
 * Gibt ein Bitfeld beliebiger Größe auf der Konsole aus. Der Aufrufe kann das Bitfeld in Blöcke beliebiger
 * Größe unterteilen lassen.
 * @param bytefield Bitfeld, das ausgegeben werden soll.
 * @param len Länge des Bitfeldes in Bytes.
 * @param block_len Größe eines Ausgabeblocks in Bits.
 */
void DESCipher::printBitField(const byte *bytefield, int len, int block_len) const
{
        /******************************************************************************
         * Aufgabe 2
         ******************************************************************************/

        for (unsigned int i = 0; i < len * 8; i++) // Iteriere über alle Bits des Arrays
        {
                if (i && !(i % block_len))
                        cout << " "; // Alle block_len Bits ein Leerzeichen

                cout << getBit(bytefield, len, i); // Gebe das aktuelle Bit des Array aus
        }
}

/**
 * @brief Verarbeitet einen Block des DES-Algorithmus.
 *
 * Diese Funktion verarbeitet einen 64-Bit-Block des DES-Algorithmus. Dies beinhaltet die anfängliche Permutation,
 * die 16 Feistel-Runden und die abschließende Permutation.
 *
 * @param in_block Der Eingabeblock (64 Bit).
 * @param out_block Der Ausgabeblock (64 Bit).
 */
void DESCipher::processBlock(const byte *in_block, byte *out_block)
{
        /******************************************************************************
         * Aufgabe 7
         ******************************************************************************/

        byte permutated[8], l[4], r[4], l_out[4], r_out[4], combined[8];

        permutate(ip, 64, in_block, 8, permutated, 8); //Eingabeblock mittels des IP (Initial Permutation)-Arrays permutieren und das Ergebnis (8-Byte) in permuted speichern.

        for (int i = 0; i < 4; i++) //Eingabeblock in eine linke und eine rechte Seite teilen, jeweils 4-Byte.
        {
                l[i] = permutated[i];
                r[i] = permutated[i + 4];
        }

        for (unsigned int i = 0; i < 16; i++) //16 Verschlüsselungsrunden
        {
                feistel(l, r, key_schedule[i], l_out, r_out, i);

                for (int i = 0; i < 4; i++)    //Nach jeder Runde werden die neuen linken und rechten Seiten überschrieben, sodass sie in der nächsten Runde als Eingabe verwendet werden können.
                {
                        l[i] = l_out[i];
                        r[i] = r_out[i];
                }
        }

        for (int i = 0; i < 4; i++) //Nach der 16. Runde werden die linken und rechten Seiten vertauscht ("32-Bit-Swap") und in combined gespeichert.
        {
                combined[i] = r[i];
                combined[i + 4] = l[i];
        }

        permutate(fp, 64, combined, 8, out_block, 8); //combined mittels des fp (Final Permutation)-Arrays permutieren und das Ergebnis (8 Byte) in out_block speichern.
}

void DESCipher::setBit(
    byte *array,
    int array_len,
    int pos,
    bool value) const
{
        int bytepos, bitpos;
        byte b;

        assert(array_len > 0);
        assert(pos >= 0);
        assert(pos < 8 * array_len);

        bytepos = pos / 8;
        bitpos = 7 - (pos % 8);
        b = 0x01;
        b = b << bitpos;
        // cout << "(" << dec << bytepos << "," << bitpos << "," << hex << (short)b << ") ";
        if (value == true)
        {
                array[bytepos] |= b;
        }
        else
        {
                b = b ^ 0xff;
                array[bytepos] &= b;
        }
}

void DESCipher::getKeySchedule(byte *key_schedule2)
{
        for (size_t i = 0; i < 16; i++)
        {
                for (size_t j = 0; j < 6; j++)
                {
                        key_schedule2[i * 6 + j] = key_schedule[i][j];
                }
        }
}

// #ip
byte DESCipher::ip[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};

// #fp
byte DESCipher::fp[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25};

// #ev
byte DESCipher::ev[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};

// #pc1
byte DESCipher::pc1[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,

    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};

// #pc2
byte DESCipher::pc2[] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

// #sbox
byte DESCipher::sbox[8][64] = {
    /* S1 */
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,

    /* S2 */
    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

    /* S3 */
    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

    /* S4 */
    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,

    /* S5 */
    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

    /* S6 */
    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

    /* S7 */
    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

    /* S8 */
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};

// #pp
byte DESCipher::pp[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25};

// #total_rot
byte DESCipher::total_rot[16] = {
    1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28};
