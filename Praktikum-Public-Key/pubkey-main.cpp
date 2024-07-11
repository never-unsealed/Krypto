/**
 *	@file pubkey-main.cpp
 */

#include <iostream>
#include <set>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace std;

/**
 * @brief Führt die Übung zu Integer-Operationen aus.
 *
 * Diese Funktion demonstriert grundlegende Operationen mit der Integer-Klasse,
 * einschließlich Initialisierung, Addition, Multiplikation und Modulo.
 */
void integerTest()
{
	/*********************************************************************
	 * Aufgabe 2.
	 *********************************************************************/

	cout << "Aufgabe 2:" << endl;
	cout << "==========" << endl
		 << endl;

	// Initialisierung von Integer-Objekten a, b und c mit verschiedenen Werten
	Integer a = Integer(23975);
	Integer b = Integer("12345678900987654321");
	Integer c = Integer::Power2(10); // Erzeugt eine Integer mit Wert 2^10 (1024)

	// Berechnung und Ausgabe des Ausdrucks (a + c) * b % 50001
	cout << ((a + c) * b) % 50001 << endl;
}

/**
 * @brief Führt die Übung zum erweiterten euklidischen Algorithmus (EEA) aus.
 *
 * Diese Funktion demonstriert die Verwendung des erweiterten euklidischen Algorithmus,
 * um den größten gemeinsamen Teiler (ggT) zweier Zahlen zu berechnen und gleichzeitig
 * die Koeffizienten x und y der Bézout-Identität zu finden.
 */
void eeaTest()
{
	/*********************************************************************
	 * Aufgabe 3.
	 *********************************************************************/

	cout << endl;
	cout << "Aufgabe 3:" << endl;
	cout << "==========" << endl
		 << endl;
	// Deklaration der Variablen für a, b, d (ggT), x und y
	Integer a, b, d, x, y;
	PublicKeyAlgorithmBox Box = PublicKeyAlgorithmBox();

	// Testfall a
	a = Integer(2987);
	b = Integer(1279865);
	Box.EEA(a, b, d, x, y); // Aufruf des EEA
	cout << "gcd(2987, 1279865) =  d = " << d << ", x = " << x << ", y = " << y << endl;

	// Testfall b
	d = 0;
	x = 0;
	y = 0;
	a = Integer(78845945);
	b = Integer(2503417846);
	Box.EEA(a, b, d, x, y); // Aufruf des EEA
	cout << "gcd(78845945, 2503417846) =  d = " << d << ", x = " << x << ", y = " << y << endl;

	// Testfall c
	d = 0;
	x = 0;
	y = 0;
	a = Integer(57913313);
	b = Integer(173739939);
	Box.EEA(a, b, d, x, y); // Aufruf des EEA
	cout << "gcd(57913313, 173739939) =  d = " << d << ", x = " << x << ", y = " << y << endl;
}

/**
 * @brief Führt die Übung zur Berechnung des multiplikativen Inversen aus.
 *
 * Diese Funktion demonstriert die Verwendung des Algorithmus zur Berechnung des
 * multiplikativen Inversen einer Zahl a modulo n.
 */
void multInverseTest()
{
	/*********************************************************************
	 * Aufgabe 4.
	 *********************************************************************/

	cout << endl;
	cout << "Aufgabe 4:" << endl;
	cout << "==========" << endl
		 << endl;

	// Deklaration der Variablen für a, n, a_inv (Inverses)
	Integer a, n, a_inv;
	PublicKeyAlgorithmBox Box = PublicKeyAlgorithmBox();

	// Testfall a
	a = Integer(10353);
	n = Integer(820343);
	a_inv = 0;
	if (Box.multInverse(a, n, a_inv))
	{
		cout << "Das Inverse von: 10353 mod 820343 = " << a_inv << endl;
	}
	else
	{
		cout << "10353 mod 820343 ist nicht invertierbar." << endl;
	}

	// Testfall b
	a = Integer(10353);
	n = Integer(820344);
	a_inv = 0;
	if (Box.multInverse(a, n, a_inv))
	{
		cout << "Das Inverse von: 10353 mod 820344 = " << a_inv << endl;
	}
	else
	{
		cout << "10353 mod 820344 ist nicht invertierbar." << endl;
	}

	// Testfall c
	a = Integer(562312);
	n = Integer(57913313);
	a_inv = 0;
	if (Box.multInverse(a, n, a_inv))
	{
		cout << "Das Inverse von: 562312 mod 57913313 = " << a_inv << endl;
	}
	else
	{
		cout << "562312 mod 57913313 ist nicht invertierbar." << endl;
	}
}

/**
 * @brief Führt die Übung zur modularen Exponentiation aus.
 *
 * Diese Funktion demonstriert die Verwendung der modularen Exponentiation zur Berechnung von
 * Potenzen mit großen Exponenten unter einem Modul.
 */
void modExpTest()
{
	/*********************************************************************
	 * Aufgabe 5.
	 *********************************************************************/

	cout << endl;
	cout << "Aufgabe 5:" << endl;
	cout << "==========" << endl
		 << endl;

	// Deklaration der Variablen für Basis, Exponent, Modul und Ergebnis
	Integer a, b, n, result;
	PublicKeyAlgorithmBox Box = PublicKeyAlgorithmBox();

	// Testfall a
	a = Integer(2);
	b = Integer(100000);
	n = Integer(23);
	result = Box.modularExponentation(a, b, n);
	cout << "2^100000 mod 23 = " << result << endl;

	// Testfall b
	a = Integer(2343947997);
	b = Integer(765);
	n = Integer(111);
	result = Box.modularExponentation(a, b, n);
	cout << "2343947997^765 mod 111 = " << result << endl;
}

/**
 * @brief Führt die Übung zur Zufallszahlenerzeugung mit verschiedenen Zufallszahlengeneratoren aus.
 *
 * Diese Funktion demonstriert die Verwendung von Zufallszahlenerzeugung mit
 * verschiedenen Generatoren (BlockingRng und NonblockingRng) für verschiedene Bitlängen.
 */
void randExercise()
{
	/*********************************************************************
	 * Aufgabe 6.
	 *********************************************************************/

	cout << endl;
	cout << "Aufgabe 6:" << endl;
	cout << "==========" << endl
		 << endl;

	Integer a;

	// Erzeugen eines Zufallszahlengenerators vom Typ BlockingRng
	BlockingRng blockingRng;

	// Zufallszahl mit 128 Bits erzeugen und ausgeben
	a.Randomize(blockingRng, 128);
	cout << " (BlockingRNG) 128 Bit Zufallszahl " << a << endl;

	// Zufallszahl mit 1024 Bits erzeugen und ausgeben
	a.Randomize(blockingRng, 1024);
    cout << " (BlockingRNG) 1024 Bit Zufallszahl " << a << endl;

	// Erzeugen eines Zufallszahlengenerators vom Typ NonblockingRng
	NonblockingRng nonblockingRng;

	// Zufallszahl mit 128 Bits erzeugen und ausgeben
	a.Randomize(nonblockingRng, 128);
    cout << " (NonblockingRNG) 128 Bit Zufallszahl " << a << endl;

	// Zufallszahl mit 1024 Bits erzeugen und ausgeben
	a.Randomize(nonblockingRng, 1024);
    cout << " (NonblockingRNG) 1024 Bit Zufallszahl " << a << endl;
}

/**
 * @brief Führt die Übung zu Miller-Rabin-Primzahltests aus.
 *
 * Diese Funktion demonstriert die Verwendung des Miller-Rabin-Tests zur Bestimmung
 * der Primzahlhaftigkeit mehrerer großer Zahlen aus einem vordefinierten Vektor.
 */
void millerRabinExercise()
{
	/*********************************************************************
	 * Aufgabe 7.
	 *********************************************************************/

	cout << endl;
	cout << "Aufgabe 7:" << endl;
	cout << "==========" << endl
		 << endl;

	// Liste von zu testenden Zahlen als Integer-Objekte
	vector<Integer> numbers = {
		Integer("279226292160650115722581212551219487007"),
		Integer("247278711133334795867191516244139839983"),
		Integer("192172622525902080249109244057747132167"),
		Integer("177387942943728133030691912202779547031")};

	// Erzeugen eines Objekts der Klasse PublicKeyAlgorithmBox
	PublicKeyAlgorithmBox Box = PublicKeyAlgorithmBox();

	unsigned int s = 100; // Anzahl der Testdurchläufe für den Miller-Rabin-Test

	// Iteration über jede Zahl in der Liste und Durchführung des Tests
	for (Integer n : numbers)
	{
		// Prüfe mit dem Miller-Rabin-Test, ob n eine Primzahl ist
		if (Box.millerRabinTest(n, s))
		{
			cout << n << " ist eine Primzahl." << endl;
		}
		else
		{
			cout << n << " ist keine Primzahl." << endl;
		}
	}
}

int main(int argc, char **argv)
{

	integerTest();
	eeaTest();
	multInverseTest();
	modExpTest();
	randExercise();
	millerRabinExercise();

	return 0;
}
