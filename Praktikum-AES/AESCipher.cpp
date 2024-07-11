/*!
 * \file
 */
//
// Created by Christoph Karg on 16.11.18.
//

#include <cassert>
#include <iostream>
#include <iomanip>
#include "AESCipher.h"

using namespace std;

AESCipher::AESCipher(bool debug_mode) : debug_mode(debug_mode), aesmath(),
                                        key_schedule(aesmath, debug_mode), state(aesmath, debug_mode)
{
}

/**
 * @brief Verschlüsselt den gegebenen Klartext und speichert den resultierenden Geheimtext.
 *
 * Diese Methode verschlüsselt den gegebenen Klartext (plain_text) und speichert das Ergebnis im
 * Geheimtext-Vektor (cipher_text). Sie ruft intern die Methode process mit mode=false auf.
 *
 * @param plain_text Der zu verschlüsselnde Klartext als Vektor von Bytes.
 * @param cipher_text Der Vektor, in dem der verschlüsselte Geheimtext gespeichert wird.
 * @return true Wenn die Verschlüsselung erfolgreich war.
 * @return false Wenn die Verschlüsselung fehlgeschlagen ist.
 */
bool AESCipher::encrypt(const vector<byte> &plain_text, vector<byte> &cipher_text)
{
    /*************************************************************************
     * Aufgabe 24b
     *************************************************************************/

    // Verschlüssele den Klartext und speichere das Ergebnis im Geheimtext-Vektor
    return process(plain_text, cipher_text, false);
}

/**
 * @brief Entschlüsselt den gegebenen Geheimtext und speichert den resultierenden Klartext.
 *
 * Diese Methode entschlüsselt den gegebenen Geheimtext (cipher_text) und speichert das Ergebnis im
 * Klartext-Vektor (plain_text). Sie ruft intern die Methode process mit mode=true auf.
 *
 * @param cipher_text Der zu entschlüsselnde Geheimtext als Vektor von Bytes.
 * @param plain_text Der Vektor, in dem der entschlüsselte Klartext gespeichert wird.
 * @return true Wenn die Entschlüsselung erfolgreich war.
 * @return false Wenn die Entschlüsselung fehlgeschlagen ist.
 */
bool AESCipher::decrypt(const vector<byte> &cipher_text, vector<byte> &plain_text)
{
    /*************************************************************************
     * Aufgabe 24c
     *************************************************************************/

    // Entschlüssele den Geheimtext und speichere das Ergebnis im Klartext-Vektor
    return process(cipher_text, plain_text, true);
}

/**
 * @brief Führt die AES-Verschlüsselung oder -Entschlüsselung durch.
 *
 * Diese Methode führt die eigentliche Verarbeitung durch, die sowohl für die
 * Verschlüsselung als auch für die Entschlüsselung verwendet wird.
 * Zunächst wird die Größe des Ausgabevektors angepasst, um Platz für die verschlüsselten oder entschlüsselten Daten zu schaffen, 
 * die dieselbe Länge wie die Eingabedaten haben.
 * Es wird überprüft, ob die Eingabelänge ein Vielfaches von 16 Bytes ist, da AES in Blöcken von 16 Bytes arbeitet. 
 * Wenn die Eingabelänge keine ganzzahlige Vielfache von 16 ist, wird false zurückgegeben, 
 * da AES nur mit vollständigen Blöcken arbeiten kann.
 * Danach wird über jeden 16-Byte-Block der Eingabe iteriert:
 * Wenn mode true ist (mode != 0), wird der Block entschlüsselt (decryptBlock() wird aufgerufen).
 * Wenn mode false ist (mode == 0), wird der Block verschlüsselt (encryptBlock() wird aufgerufen).
 * Nachdem alle Blöcke verarbeitet wurden, gibt die Funktion true zurück, was darauf 
 * hinweist, dass die Operation erfolgreich war und die verschlüsselten oder entschlüsselten 
 * Daten im Ausgabevektor out gespeichert sind.
 *
 * @param in Der Eingabe-Vektor (entweder Klartext oder Geheimtext).
 * @param out Der Ausgabe-Vektor (entweder Geheimtext oder Klartext).
 * @param mode Gibt an, ob die Verarbeitung eine Entschlüsselung ist (true für Entschlüsselung, false für Verschlüsselung).
 * @return true Wenn die Verarbeitung erfolgreich war.
 * @return false Wenn die Verarbeitung fehlgeschlagen ist.
 */
bool AESCipher::process(const vector<byte> &in, vector<byte> &out, bool mode)
{
    /*************************************************************************
     * Aufgabe 24a
     *************************************************************************/

    // Größe des Ausgabe-Vektors anpassen
    out.resize(in.size());

    // Prüfen, ob die Eingabelänge ein Vielfaches von 16 Bytes ist
    if (in.size() % 16)
        return false;

    // Über alle Blöcke iterieren
    for (int i = 0; i < in.size() / 16; i++)
    {
        if (mode)
        {
            // Entschlüsseln
            decryptBlock(&in[i * 16], &out[i * 16]);
        }
        else
        {
            // Verschlüsseln
            encryptBlock(&in[i * 16], &out[i * 16]);
        }
    }

    return true;
}

/**
 * @brief Setzt den Schlüssel für den AES-Algorithmus.
 *
 * Diese Methode setzt den Schlüssel (key) für den AES-Algorithmus und initialisiert den Schlüsselplan.
 * Sie ruft hierfür die setKey-Methode der Klassenvariable key_schedule auf.
 *
 * @param key Der zu setzende Schlüssel als Vektor von Bytes.
 * @return true Wenn der Schlüssel erfolgreich gesetzt wurde.
 * @return false Wenn das Setzen des Schlüssels fehlgeschlagen ist.
 */
bool AESCipher::setKey(const vector<byte> &key)
{
    /*************************************************************************
     * Aufgabe 21
     *************************************************************************/

    return key_schedule.setKey(key); // Setze den Schlüssel mittels setKey der key_schedule-Instan
}

void AESCipher::debugMessage(size_t round, string msg)
{
    if (debug_mode)
    {
        cout << "round[" << dec << setw(2) << setfill(' ') << round
             << "]." << msg << endl;
    }
}

/**
 * @brief Entschlüsselt einen Block von Geheimtext.
 *
 * Diese Methode entschlüsselt einen einzelnen Block von Geheimtext (cipher_text) und speichert das Ergebnis
 * im Klartext-Array (plain_text).
 * 
 * 1. Der Geheimtext wird in den Zustand state geladen.
 * 2. Der letzte Rundenschlüssel wird zum Zustand hinzugefügt.
 * 3. Die inverse ShiftRows-Transformation (state.invShiftRows()) wird angewendet, um die Verschiebung der Zeilen rückgängig zu machen.
 * 4. Die inverse SubBytes-Transformation (state.invSubBytes()) wird durchgeführt, um die Substitution der Bytes rückgängig zu machen.
 * 5. Für jede Runde (beginnend mit der vorletzten bis zur ersten Runde):
 * -> Der Rundenschlüssel für die aktuelle Runde wird zum Zustand hinzugefügt.
 * -> Die inverse MixColumns-Transformation (state.invMixColumns()) wird angewendet, um die MixColumns-Operation rückgängig zu machen.
 * -> Die inverse ShiftRows-Transformation und die inverse SubBytes-Transformation werden erneut durchgeführt, um die entsprechenden Operationen rückgängig zu machen.
 * 6. Nachdem alle Runden durchgeführt wurden, wird der Initial-Rundenschlüssel hinzugefügt.
 * 7. Der entschlüsselte Text wird aus dem Zustand state extrahiert und im Klartext gespeichert.
 *
 * @param cipher_text Ein Zeiger auf das Eingabe-Array, das den zu entschlüsselnden Geheimtext enthält.
 * @param plain_text Ein Zeiger auf das Ausgabe-Array, in das der entschlüsselte Klartext gespeichert wird.
 */
void AESCipher::decryptBlock(const byte *cipher_text, byte *plain_text)
{
    /*************************************************************************
     * Aufgabe 23
     *************************************************************************/
    int index = 1;

    // Setze den Zustand mit dem Geheimtext
    state.set(cipher_text);
    debugMessage(0, "iinput   " + state.format());

    // Füge den letzten Rundenschlüssel hinzu
    state.addKey(key_schedule.getRoundKey(key_schedule.getNrOfRounds()));
    debugMessage(index, "ik_sch   " + key_schedule.formatRoundKey(key_schedule.getNrOfRounds()));
    debugMessage(index, "istart    " + state.format());

    // Führe die inverse ShiftRows-Transformation durch
    state.invShiftRows();
    debugMessage(index, "is_row   " + state.format());

    // Führe die inverse SubBytes-Transformation durch
    state.invSubBytes();
    debugMessage(index, "is_box   " + state.format());

    // Führe die (Nr-1) Runden der inversen AES-Transformation durch
    for (int i = key_schedule.getNrOfRounds() - 1; i > 0; i--)
    {
        // Füge den Rundenschlüssel hinzu
        state.addKey(key_schedule.getRoundKey(i));
        debugMessage(index, "ik_sch   " + key_schedule.formatRoundKey(i));
        debugMessage(index, "ik_add   " + state.format());

        index++;

        // Führe die inverse MixColumns-Transformation durch
        state.invMixColumns();
        debugMessage(index, "istart   " + state.format());

        // Führe die inverse ShiftRows-Transformation durch
        state.invShiftRows();
        debugMessage(index, "is_row   " + state.format());

        // Führe die inverse SubBytes-Transformation durch
        state.invSubBytes();
        debugMessage(index, "is_box   " + state.format());
    }

    // Füge den Initial-Rundenschlüssel hinzu
    state.addKey(key_schedule.getRoundKey(0));
    debugMessage(index, "ik_sch   " + key_schedule.formatRoundKey(0));

    // Hole den entschlüsselten Text aus dem Zustand
    state.get(plain_text);
    debugMessage(index, "ioutput  " + state.format());
}

/**
 * @brief Verschlüsselt einen Block von Klartext.
 *
 * Diese Methode verschlüsselt einen einzelnen Block von Klartext (plain_text) und speichert das Ergebnis
 * im Geheimtext-Array (cipher_text).
 * Zunächst wird der Zustand state mit dem Klartext initialisiert und der Initial-Rundenschlüssel hinzugefügt. 
 * Anschließend werden die Runden der AES-Verschlüsselung durchgeführt, beginnend mit der ersten bis zur vorletzten Runde.
 * In jeder Runde werden die folgenden Schritte ausgeführt:
 * 1. Die SubBytes-Transformation (state.subBytes()) wird angewendet.
 * 2. Die ShiftRows-Transformation (state.shiftRows()) wird durchgeführt.
 * 3. Die MixColumns-Transformation (state.mixColumns()) wird ausgeführt.
 * 4. Der Rundenschlüssel für die aktuelle Runde wird zum Zustand hinzugefügt (state.addKey()).
 * Nach Abschluss der Runden wird in der letzten Runde die 
 * SubBytes-Transformation und die ShiftRows-Transformation erneut durchgeführt. 
 * Anschließend wird der letzte Rundenschlüssel hinzugefügt. Schließlich wird der verschlüsselte 
 * Text aus dem Zustand state extrahiert und im cipher_text gespeichert.
 * Die Debug-Nachrichten (debugMessage) geben Informationen über den aktuellen Zustand nach 
 * jeder Transformation aus, was hilfreich ist, um den Ablauf der Verschlüsselung nachzuvollziehen und zu überprüfen.
 *
 * @param plain_text Ein Zeiger auf das Eingabe-Array, das den zu verschlüsselnden Klartext enthält.
 * @param cipher_text Ein Zeiger auf das Ausgabe-Array, in das der verschlüsselte Geheimtext gespeichert wird.
 */
void AESCipher::encryptBlock(const byte *plain_text, byte *cipher_text)
{
    /*************************************************************************
     * Aufgabe 22
     *************************************************************************/

    // Setze den Zustand mit dem Klartext
    state.set(plain_text);
    debugMessage(0, "input   " + state.format());

    // Füge den Initial-Rundenschlüssel hinzu
    state.addKey(key_schedule.getRoundKey(0));
    debugMessage(0, "k_sch   " + key_schedule.formatRoundKey(0));

    // Führe die ersten (Nr-1) Runden durch
    for (int i = 1; i < key_schedule.getNrOfRounds(); i++)
    {
        // Führe die SubBytes-Transformation durch
        state.subBytes();
        debugMessage(i, "s_box   " + state.format());

        // Führe die ShiftRows-Transformation durch
        state.shiftRows();
        debugMessage(i, "s_row   " + state.format());

        // Führe die MixColumns-Transformation durch
        state.mixColumns();
        debugMessage(i, "m_col   " + state.format());

        // Füge den Rundenschlüssel zum State hinzu
        state.addKey(key_schedule.getRoundKey(i));
        debugMessage(i, "k_sch   " + key_schedule.formatRoundKey(i));
    }

    // Führe die SubBytes-Transformation durch
    state.subBytes();
    debugMessage(key_schedule.getNrOfRounds(), "s_box   " + state.format());

    // Führe die ShiftRows-Transformation durch
    state.shiftRows();
    debugMessage(key_schedule.getNrOfRounds(), "s_row   " + state.format());

    // Füge den Rundenschlüssel letzten zum State hinzu
    state.addKey(key_schedule.getRoundKey(key_schedule.getNrOfRounds()));
    debugMessage(key_schedule.getNrOfRounds(), "k_sch   " + key_schedule.formatRoundKey(key_schedule.getNrOfRounds()));

    // Hole den verschlüsselten Text aus dem Zustand
    state.get(cipher_text);
    debugMessage(key_schedule.getNrOfRounds(), "output  " + state.format());
}

vector<byte> AESCipher::toVector(const string &msg, size_t block_len)
{
    assert((block_len > 0) && (block_len <= 32));

    vector<byte> v;

    copy(msg.begin(), msg.end(), back_inserter(v));

    v.push_back(0);
    while (v.size() % block_len != 0)
    {
        v.push_back(1);
    }

    return v;
}

string AESCipher::toString(const vector<byte> &vec)
{
    auto it = vec.begin();
    string r;

    while ((*it != 0) and (it != vec.end()))
    {
        r.push_back(*it);
        it++;
    }
    return r;
}
