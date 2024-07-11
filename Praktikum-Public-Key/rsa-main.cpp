/**
 *	@file rsa-main.cpp
 */

#include <cassert>
#include <iostream>
#include "PublicKeyAlgorithmBox.h"
#include "RSAEncryptor.h"
#include "RSADecryptor.h"
#include "RSAAttack.h"
#include "RSAOracle.h"

using namespace std;

/**
 * @brief Führt die Generierung von RSA-Parametern durch und gibt sie aus.
 *
 * Diese Funktion ruft die Methode generateRSAParams auf, um die RSA-Parameter p, q, e und d zu generieren,
 * und gibt diese Parameter anschließend auf der Konsole aus.
 */
void rsaParameters()
{
	/*********************************************************************
	 * Aufgabe 15
	 *********************************************************************/

	cout << endl;								// Ausgabe eines Zeilenumbruchs
	cout << "RSA Parameter Generation" << endl; // Ausgabe des Titels
	cout << "========================" << endl
		 << endl; // Ausgabe einer Trennlinie und eines weiteren Zeilenumbruchs

	PublicKeyAlgorithmBox box; // Initialisierung eines Objekts der Klasse PublicKeyAlgorithmBox

	Integer p, q, d, e; // Deklaration der Integer-Variablen p, q, d und e zur Speicherung der RSA-Parameter

	box.generateRSAParams(p, q, e, d); // Aufruf der Methode zur Generierung der RSA-Parameter

	// Ausgabe der generierten RSA-Parameter auf der Konsole
	cout << "p=" << p << endl;
	cout << "q=" << q << endl;
	cout << "d=" << d << endl;
	cout << "e=" << e << endl;
}

/**
 * @brief Führt einen Test des RSA-Kryptosystems durch.
 * 
 * Diese Funktion führt eine Reihe von Tests für das RSA-Kryptosystem durch, einschließlich Verschlüsselung, Entschlüsselung mit dem privaten Schlüssel und Anwendung des Chinesischen Restsatzes (CRT) sowie des Garner-Algorithmus.
 * Sie verwendet vordefinierte Primzahlen, Schlüssel und zu verschlüsselnde/entschlüsselnde Ganzzahlen für die Tests.
 */
void rsaTest()
{
	/*********************************************************************
	 * Aufgabe 16g
	 *********************************************************************/

	cout << endl; 
	cout << "RSA Cryptosystem" << endl; 
	cout << "================" << endl << endl; 

	Integer p("15192846618168946907"); // Definition der Primzahl p
	Integer q("10041530829891794273"); // Definition der Primzahl q
	Integer e("141290156426204318982571851806193576543"); // Definition des öffentlichen Exponenten e
	Integer d("73707354481439936713886319521045114527"); // Definition des privaten Exponenten d
	Integer x("79372353861768787619084471254314002875"); // Zu verschlüsselnde Ganzzahl x
	Integer y("47915958473033255778832465116435774510"); // Erwarteter verschlüsselter Wert y

	Integer test; // Variable für den verschlüsselten Testwert
	Integer test_decrypted; // Variable für den entschlüsselten Testwert

	RSAEncryptor encryptor(p * q, e); // Erzeugen eines RSAEncryptor-Objekts mit den Primzahlen p und q sowie dem öffentlichen Exponenten e
	RSADecryptor decryptor(p, q, d); // Erzeugen eines RSADecryptor-Objekts mit den Primzahlen p und q sowie dem privaten Exponenten d

	if (!encryptor.compute(x, test)) // Verschlüsselung der Ganzzahl x; Rückgabe false bei Misserfolg
	{
		cout << "Verschluesselung fehlgeschlagen" << endl; // Ausgabe bei fehlgeschlagener Verschlüsselung
		return;
	}

	if (test == y) // Überprüfung, ob der verschlüsselte Testwert test mit dem erwarteten Wert y übereinstimmt
	{
		cout << "Rueckgabewert stimmt mit erwartetem Wert ueberein!" << endl; // Ausgabe bei erfolgreicher Übereinstimmung
	}
	else
	{
		cout << "Rueckgabewert stimmt nicht mit erwartetem Wert ueberein!" << endl; // Ausgabe bei fehlender Übereinstimmung
	}

	cout << endl; // Leerzeile in der Ausgabe

	cout << "RSA Decryptor Test:" << endl; 

	if (!decryptor.compute(test, test_decrypted)) // Entschlüsselung des verschlüsselten Testwerts test; Rückgabe false bei Misserfolg
	{
		cout << "Entschluesselung fehlgeschlagen" << endl; // Ausgabe bei fehlgeschlagener Entschlüsselung
		return;
	}

	if (test_decrypted == x) // Überprüfung, ob der entschlüsselte Testwert test_decrypted mit der ursprünglichen Ganzzahl x übereinstimmt
	{
		cout << "Entschluesselung erfolgreich!" << endl; // Ausgabe bei erfolgreicher Entschlüsselung
	}

	cout << "RSA CRT Test:" << endl; 

	if (!decryptor.crt(test, test_decrypted)) // CRT-Entschlüsselung des verschlüsselten Testwerts test; Rückgabe false bei Misserfolg
	{
		cout << "CRT Entschluesselung fehlgeschlagen" << endl; // Ausgabe bei fehlgeschlagener CRT-Entschlüsselung
		return;
	}

	if (test_decrypted == x) // Überprüfung, ob der CRT-entschlüsselte Testwert test_decrypted mit der ursprünglichen Ganzzahl x übereinstimmt
	{
		cout << "CRT Entschluesselung erfolgreich!" << endl; // Ausgabe bei erfolgreicher CRT-Entschlüsselung
	}

	cout << "RSA GARNER Test:" << endl; 

	if (!decryptor.garner(test, test_decrypted)) // Garner-Entschlüsselung des verschlüsselten Testwerts test; Rückgabe false bei Misserfolg
	{
		cout << "GARNER Entschluesselung fehlgeschlagen" << endl; // Ausgabe bei fehlgeschlagener Garner-Entschlüsselung
		return;
	}

	if (test_decrypted == x) // Überprüfung, ob der Garner-entschlüsselte Testwert test_decrypted mit der ursprünglichen Ganzzahl x übereinstimmt
	{
		cout << "GARNER Entschluesselung erfolgreich!" << endl; // Ausgabe bei erfolgreicher Garner-Entschlüsselung
	}
}

int main()
{
	rsaParameters();
	rsaTest();

	return 0;
}
