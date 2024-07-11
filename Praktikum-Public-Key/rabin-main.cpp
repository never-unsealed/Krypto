/**
 *	@file rabin-main.cpp
 */

#include <iostream>
#include <vector>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"
#include "RabinDecryptor.h"
#include "RabinEncryptor.h"
#include "RabinAttack.h"

using namespace std;

/**
 * @brief Generiert Rabin-Parameter p und q.
 *
 * Diese Funktion generiert zwei 256-Bit Rabin-Primzahlen p und q. Dazu werden
 * Primzahltests mit dem Miller-Rabin-Test durchgeführt, um sicherzustellen,
 * dass die generierten Zahlen prim sind.
 */
void rabinParameters()
{
	/*********************************************************************
	 * Aufgabe 9.
	 *********************************************************************/

	// Ausgabe der Überschrift für die Aufgabe
	cout << "Rabin Parameters" << endl;
	cout << "================" << endl
		 << endl;

	// Festlegen der Bitlänge der Primzahlen und der Anzahl der Testdurchläufe für den Miller-Rabin-Test
	unsigned int bitlen = 256; // Bitlänge der Primzahlen
	unsigned int s = 50;	   // Anzahl der Testdurchläufe für den Miller-Rabin-Test
	PublicKeyAlgorithmBox Box = PublicKeyAlgorithmBox();

	Integer p, q;

	// Generiere erste 256-Bit Rabin-Primzahl p
	unsigned int tries_p = Box.randomRabinPrime(p, bitlen, s);
	cout << "p = " << p << endl;
	cout << "Anzahl der Versuche für p: " << tries_p << endl
		 << endl;

	// Generiere zweite 256-Bit Rabin-Primzahl q
	unsigned int tries_q = Box.randomRabinPrime(q, bitlen, s);
	cout << "q = " << q << endl;
	cout << "Anzahl der Versuche für q: " << tries_q << endl;
}

/**
 * @brief Führt den Test der modularen Quadratwurzelmoduloperation für Primzahlen durch.
 *
 * Diese Funktion testet die Berechnung der modularen Quadratwurzelmoduloperation für gegebene Werte von y und p.
 * Sie verwendet die Funktion `modPrimeSqrt` der Klasse `PublicKeyAlgorithmBox`, um die modularen Quadratwurzeln zu berechnen
 * und gibt die Ergebnisse auf der Konsole aus.
 */
void sqrtModPrimeExercise()
{
	/*********************************************************************
	 * Aufgabe 10.
	 *********************************************************************/

	// Ausgabe der Überschrift für die Aufgabe
	cout << endl;
	cout << "sqrtModPrime Test" << endl;
	cout << "=================" << endl
		 << endl;

	// Initialisierung der gegebenen Werte für y und p
	Integer y = Integer("400040001");
	Integer p = Integer("884249923643");
	vector<Integer> v1;
	PublicKeyAlgorithmBox Box = PublicKeyAlgorithmBox();

	// Berechnung der modularen Quadratwurzeln für y modulo p
	if (Box.modPrimeSqrt(y, p, v1))
	{
		cout << "y = " << y << ", p = " << p << endl;
		cout << "v0 = " << v1[0] << ", v1 = " << v1[1] << endl
			 << endl;
	}

	// Neue Werte für y und p für den nächsten Testfall
	y = Integer("644640535034");
	p = Integer("868380007367");
	vector<Integer> v2;

	// Berechnung der modularen Quadratwurzeln für y modulo p
	if (Box.modPrimeSqrt(y, p, v2))
	{
		cout << "y = " << y << ", p = " << p << endl;
		cout << "v0 = " << v2[0] << ", v1 = " << v2[1] << endl
			 << endl;
	}
}

/**
 * @brief Demonstration des Rabin-Kryptosystems.
 *
 * Diese Funktion zeigt die Verwendung des Rabin-Kryptosystems. Es werden sowohl die Verschlüsselungs- als auch die
 * Entschlüsselungsprozesse demonstriert. Zusätzlich wird die Funktion compute2 getestet, die mit Padding arbeitet.
 */
void rabinDemo()
{
	/*********************************************************************
	 * Aufgaben 11 und 12
	 *********************************************************************/

	cout << endl;						  // Ausgabe eines Zeilenumbruchs
	cout << "Rabin Cryptosystem" << endl; // Ausgabe des Titels
	cout << "==================" << endl
		 << endl; // Ausgabe einer Trennlinie und eines weiteren Zeilenumbruchs

	Integer p("728768879148869666628372866383");  // Initialisierung von p mit einem großen Integer-Wert
	Integer q("1178365175275537416785439551531"); // Initialisierung von q mit einem großen Integer-Wert

	Integer x("234131892323212"); // Initialisierung von x mit einem großen Integer-Wert

	// Berechne n
	Integer n = p * q; // Berechnung von n als Produkt von p und q

	// Initialisiere den Encryptor und Decryptor
	RabinEncryptor encryptor(n);	// Initialisierung des Verschlüsselers mit n
	RabinDecryptor decryptor(p, q); // Initialisierung des Entschlüsselers mit p und q

	// Verschlüssle x
	Integer y;					 // Deklaration der Variable y für das verschlüsselte Ergebnis
	if (encryptor.compute(x, y)) // Verschlüssle x und speichere das Ergebnis in y, überprüfe den Erfolg
	{
		cout << "Verschlüsselung:" << endl; // Ausgabe des Verschlüsselungstitels
		cout << "x = " << x << endl;		// Ausgabe des ursprünglichen Wertes x
		cout << "y = " << y << endl
			 << endl; // Ausgabe des verschlüsselten Wertes y und eines Zeilenumbruchs

		// Überprüfen Sie, ob das verschlüsselte Ergebnis korrekt ist
		Integer expectedY("54817743002848138258673996944"); // Erwarteter verschlüsselter Wert
		if (y == expectedY)									// Überprüfung, ob das Ergebnis y dem erwarteten Wert entspricht
		{
			cout << "Die Verschlüsselung war erfolgreich und das Ergebnis stimmt mit dem erwarteten Wert überein." << endl
				 << endl; // Erfolgreiche Verschlüsselung
		}
		else
		{
			cout << "Die Verschlüsselung war erfolgreich, aber das Ergebnis stimmt nicht mit dem erwarteten Wert überein." << endl
				 << endl; // Verschlüsselung erfolgreich, Ergebnis abweichend
		}

		// Entschlüssle y
		vector<Integer> xv;			  // Deklaration eines Vektors zur Speicherung der entschlüsselten Werte
		if (decryptor.compute(y, xv)) // Entschlüssele y und speichere die Ergebnisse in xv, überprüfe den Erfolg
		{
			// Überprüfen Sie, ob die entschlüsselten Werte korrekt sind
			vector<Integer> expectedXv = {
				Integer("858755868013614750163033981532656431854843747105980613759161"),
				Integer("468253878381255984906252283528370901639136709523733710358252"),
				Integer("390501989632358765256781698004285530215707037816378795724121"),
				Integer("234131892323212")}; // Erwartete entschlüsselte Werte

			bool success = true;				   // Flag zur Überprüfung des Erfolgs
			for (size_t i = 0; i < xv.size(); ++i) // Iteration über die entschlüsselten Werte
			{
				if (expectedXv[i] != xv[i]) // Überprüfung, ob der aktuelle entschlüsselte Wert dem erwarteten Wert entspricht
				{
					success = false; // Setze das Erfolgs-Flag auf false, falls ein Wert nicht übereinstimmt
				}
			}
			if (success) // Wenn alle Werte übereinstimmen
			{
				cout << "Die Entschlüsselung war erfolgreich, da alle berechneten Werten mit den erwarteten Werten übereinstimmen" << endl; // Ausgabe der Erfolgsmeldung
			}
			else
			{
				cout << "Entschlüsselung fehlgeschlagen" << endl; // Ausgabe der Fehlermeldung
			}
		}
	}

	cout << "---------------------" << endl; // Ausgabe einer Trennlinie
	cout << "compute2 Test" << endl;		 // Ausgabe des Titels für den compute2-Test

	// Initialisiere den Encryptor und Decryptor
	Integer padding("987654321");			  // Initialisierung von padding mit einem großen Integer-Wert
	RabinEncryptor encryptor2(n, padding);	  // Initialisierung des zweiten Verschlüsselers mit n und padding
	RabinDecryptor decryptor2(p, q, padding); // Initialisierung des zweiten Entschlüsselers mit p, q und padding

	if (encryptor2.compute2(x, y)) // Verschlüssele x mit padding und speichere das Ergebnis in y, überprüfe den Erfolg
	{
		cout << "Verschlüsselung:" << endl; // Ausgabe des Verschlüsselungstitels
		cout << "x = " << x << endl;		// Ausgabe des ursprünglichen Wertes x
		cout << "y = " << y << endl
			 << endl; // Ausgabe des verschlüsselten Wertes y und eines Zeilenumbruchs

		// Entschlüssle y
		Integer x;					   // Deklaration der Variable x für das entschlüsselte Ergebnis
		if (decryptor2.compute2(y, x)) // Entschlüssele y mit padding und speichere das Ergebnis in x, überprüfe den Erfolg
		{
			cout << "Entschlüsselung erfolgreich" << endl; // Ausgabe der Erfolgsmeldung
			cout << "x=" << x << endl;							   // Ausgabe des entschlüsselten Wertes x
		}
		else
		{
			cout << "decryptor2.compute2 fehlgeschlagen" << endl; // Ausgabe der Fehlermeldung
		}
	}
	else
	{
		cout << "encryptor2.compute2 fehlgeschlagen" << endl; // Ausgabe der Fehlermeldung
	}
}

/**
 * @brief Demonstration eines Angriffs auf das Rabin-Kryptosystem.
 *
 * Diese Funktion führt einen Angriff auf das Rabin-Kryptosystem durch, um die Faktoren von n zu berechnen.
 * Es wird versucht, n zu faktorisieren, indem ein RabinDecryptor verwendet wird. Wenn der Angriff erfolgreich ist,
 * werden die Faktoren p und q ausgegeben.
 */
void rabinAttack()
{
	/*********************************************************************
	 * Aufgabe 13
	 *********************************************************************/

	cout << endl; // Ausgabe eines Zeilenumbruchs
	cout << "Rabin Attack" << endl; // Ausgabe des Titels
	cout << "============" << endl << endl; // Ausgabe einer Trennlinie und eines weiteren Zeilenumbruchs

	RabinAttack attack; // Initialisierung eines RabinAttack-Objekts

	Integer p("728768879148869666628372866383");  // Initialisierung von p mit einem großen Integer-Wert
	Integer q("1178365175275537416785439551531"); // Initialisierung von q mit einem großen Integer-Wert

	Integer n = p * q; // Berechnung von n als Produkt von p und q
	Integer f; // Deklaration der Variable f zur Speicherung des gefundenen Faktors

	RabinDecryptor decryptor(p, q); // Initialisierung eines RabinDecryptor mit p und q

	// Versuche, n zu faktorisieren, indem die Methode factorize aufgerufen wird
	if (attack.factorize(n, f, 10, decryptor) != -1) // Überprüfung, ob die Faktorisierung erfolgreich war
	{
		cout << "RabinAttack.factorize ergab " << endl; // Ausgabe einer Erfolgsmeldung
		cout << "p=" << f << endl; // Ausgabe des gefundenen Faktors p
		cout << "q=" << n / f << endl; // Berechnung und Ausgabe des zweiten Faktors q
	}
	else
	{
		cout << "RabinAttack.factorize fehlgeschlagen" << endl; // Ausgabe einer Fehlermeldung
	}
}

int main()
{

	rabinParameters();
	sqrtModPrimeExercise();
	rabinDemo();
	rabinAttack();

	return 0;
}
