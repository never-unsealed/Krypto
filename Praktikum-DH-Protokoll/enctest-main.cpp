/*
 * @file enctest.cpp
 */

#include <iostream>
#include "MessageEncoder.h"
#include <integer.h>

using namespace std;
using namespace CryptoPP;

/**
 * @brief Testfunktion für die Klasse MessageEncoder.
 *
 * Diese Funktion demonstriert die Verwendung der Klasse MessageEncoder
 * durch verschiedene Testfälle.
 */
void encoderTest() {
    // Ausgabe für den Testbeginn
    cout << "MessageEncoderTest" << endl <<
         "==================" << endl;

    // Initialisierung eines MessageEncoder-Objekts
    MessageEncoder me;

    // Initialisierung von Integer- und String-Variablen für Tests
    Integer i("10892"), j("459111231");
    string s = "Hallo Welt!";
    string key = "phair8eo3AneeG4u";

    // Aufgabe 2b)
    me.append(Integer("10892"));
    me.append("Hallo Welt!");
    me.append(Integer("459111231"));

    // Ausgabe der kodierten Nachricht
    cout << me.encode() << endl;

    // Signieren der Nachricht mit einem Schlüssel und Ausgabe der Prüfsumme
    string digest = me.sign(key);
    cout << "Prüfsumme: " << digest << endl;

    // Überprüfen der Signatur
    cout << "Check:" << me.verify(key, digest) << endl;

    // Abrufen eines Integer-Werts aus dem kodierten Nachrichtenpaket
    Integer i2 = me.getInteger(1);
    cout << i2 << endl;

    // Kodieren, Dekodieren und erneutes Kodieren der Nachricht
    string msg = me.encode();
    me.decode(msg);

    // Ausgabe der erneut kodierten Nachricht und abgerufener Werte
    cout << me.encode() << endl;
    cout << me.getInteger(0) << endl;
    cout << me.getString(1) << endl;
    cout << me.getInteger(2) << endl;

    // Löschen der aktuellen Nachricht im MessageEncoder
    me.clear();

    // Erstellen einer neuen Nachricht und Kodierung
    me.append(Integer("471108155150"));
    me.append("Ich bin eine Zeichenfolge, die");
    me.append(" sich aus zwei Teilen zusammensetzt");
    cout << endl << me.encode() << endl;

    // Signieren der neuen Nachricht mit einem anderen Schlüssel
    cout << me.sign("ahVoh4naghie") << endl;

    // Aufgabe 2c)
    cout << "---------------" << endl;
    cout << "Aufgabe 2c)" << endl;

    // Erneute Verwendung des MessageEncoder-Objekts für eine neue Nachricht
    me.clear();
    me.append(Integer("98921323668113423232"));
    me.append("Kryptografie macht Spass!");
    me.append("Crypto++ ist eine Kryptografie-Bibliothek");
    me.append(Integer("456291260721096"));
    string encoded = me.encode();

    // Ausgabe der kodierten Nachricht
    cout << encoded << endl;

    // Aufgabe 2d)
    cout << "---------------" << endl;
    cout << "Aufgabe 2d)" << endl;

    // Dekodieren der zuvor kodierten Nachricht und Ausgabe der Inhalte
    me.decode(encoded);
    cout << me.getInteger(0) << endl;
    cout << me.getString(1) << endl;
    cout << me.getString(2) << endl;
    cout << me.getInteger(3) << endl;

    // Aufgabe 2e)
    cout << "---------------" << endl;
    cout << "Aufgabe 2e)" << endl;

    // Dekodieren einer vorgegebenen kodierten Nachricht
    encoded = "6DB03BF70E#4963682062696E2065696E65205A65696368656E666F6C67652C20646965#207369636820617573207A776569205465696C656E207A7573616D6D656E7365747A74";
    me.decode(encoded);

    // Ausgabe der abgerufenen Integer- und String-Werte
    cout << me.getInteger(0) << endl;
    cout << me.getString(1) << endl;
    cout << me.getString(2) << endl;

    // Aufgabe 2f)
    cout << "---------------" << endl;
    cout << "Aufgabe 2f)" << endl;

    // Überprüfen der Signatur mit einem vorgegebenen Schlüssel und Prüfsumme
    key = "ahVoh4naghie";
    digest = "937A6984EE71C06E808AB996BF76C246AC29E198B6637B02C37ABDE92D6A46C8";
    cout << "Check:" << me.verify(key, digest) << endl;
}

int main(int argc, char **argv) {
	encoderTest();
	return 0;
}
