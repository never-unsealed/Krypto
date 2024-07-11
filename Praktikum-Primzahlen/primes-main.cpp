/*
 * @file primes-main.cpp
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "FortunaGenerator.h"
#include "MillerRabinTest.h"

using namespace std;

/**
 * @brief Führt die Aufgaben 2f und g durch.
 *
 * Erzeugt ein neues Objekt vom Typ FortunaGenerator und seeded den PRNG
 * mit dem Wert 0x7ab2af1a4561abf8. Anschließend werden 1000 Zufallsbytes generiert
 * und als INT auf dem Bildschirm ausgegeben.
 *
 * Aufgabe g erzeugt weitere 1000 Zufallsbytes mit dem gleichen Objekt und gibt
 * diese ebenfalls auf der Konsole aus.
 *
 */
void fortuna()
{
     cout << "Fortuna PRNG:" << endl
          << "============="
          << endl
          << endl;
     /*************************************************************************
      * Aufgabe 2fh.
      *************************************************************************/
     byte puffer[1000] = {};

     // Seed 0x7ab2af1a4561abf8.

     // Aufgabe 2f

     byte seed[8] = {0x7a, 0xb2, 0xaf, 0x1a, 0x45, 0x61, 0xab, 0xf8};

     FortunaGenerator Objekt = FortunaGenerator(); // Neues PRNG Objekt anlegen

     Objekt.reseed(seed, 8); // Seed mit dem Wert aus der Aufgabe

     Objekt.getBlock(puffer, 1000); // Generiere 1000 Zufallsbytes

     for (int i = 0; i < 1000; i++) // Ausgabe aller 1000 Bytes als INT
     {
          cout << int(puffer[i]);
     }

     cout << endl
          << endl;

     // Aufgabe 2h

     Objekt.getBlock(puffer, 1000); // Generiere 1000 weitere Zufallsbytes

     for (int i = 0; i < 1000; i++) // Ausgabe aller 1000 Bytes als INT
     {
          cout << int(puffer[i]);
     }

     cout << endl
          << endl;
}

/**
 * @brief Führt den witness-Test auf eine vorgegebene Zahl durch.
 *
 * Es wird durch die witness Methode des MillerRabinTest bewiesen,
 * dass 3 ein solider Beweis für die gegebene Zahl n ist.
 * Erklärung für 3 findet sich in der entsprechenden markdown-Datei.
 *
 */
void witnessTest()
{
     cout << "witness() Test:" << endl
          << "==============="
          << endl
          << endl;

     /*************************************************************************
      * Aufgabe 3d.
      *************************************************************************/

     Integer test_zahl("121399589920585397786647347761633"); // Lege die gegebene Zahl als Integer an

     bool success = MillerRabinTest::witness(2, test_zahl); // Führe witness Methode mit a=2 aus.

     cout << "witness Rueckgabewert mit a=2: " << success // Gebe Rückgabewert auf der Konsole aus.
          << endl
          << endl;
}

/**
 * @brief Testet, ob 4 gegebene Zahlen Primzahlen sind.
 *
 *  Nutzt den FortunaGenerator, um mit der statischen MillerRabinTest::isPrime-Methode
 *  zu prüfen, ob 4 gegebene Zahlen Primzahlen sind. Das Eregbnis wird mitsamt
 *  der jeweiligen Zahl auf der Konsole ausgegeben. Als Qualitätsparameter wird 50 verwendet.
 *
 */
void isPrimeTest()
{
     cout << "isPrime() Test:" << endl
          << "==============="
          << endl
          << endl;

     /*************************************************************************
      * Aufgabe 4b.
      *************************************************************************/

     const char *list[] = {// Lege konstantes Array der zu prüfenden Zahlen an.
                           "249236727971798483921811058804041930204535828339",
                           "167376539261987877304745111417636335723391598799",
                           "207241657224647904979925877995751997643166331621",
                           "203681457354349143050092586190992078578082388049"};

     FortunaGenerator rng = FortunaGenerator(); // Erstelle neue Instanz des FortunaGenerators

     for (int i = 0; i < 4; i++) // Eine Iteration pro zu prüfende Zahl
     {
          Integer current(list[i]); // Initialisere den C-String zu einem Integer

          cout << "Test für Zahl "
               << current << ": "
               << MillerRabinTest::isPrime(&rng, current, 50) // Führe den isPrime-Test für die aktuelle Zahl durch und gib das Ergebnis auf der Konsole aus
               << endl;
     }
}

/**
 * @brief Führt eine Exponentation durch und gibt das Ergebnis auf der Konsole aus.
 *
 *  Nutzt die MillerRabinTest::exp-Methode, um 13^57 auszurechnen (in Aufgabe gegeben)
 *  und gibt das Ergebnis auf der Konsole aus.
 *
 */
void expTest()
{
     cout << endl;
     cout << "Exponentiation:" << endl
          << "===============" << endl
          << endl;

     /*************************************************************************
      * Aufgabe 6b.
      *************************************************************************/

     cout << MillerRabinTest::exp(13, 57) << endl;
}

/**
 * @brief Sucht nach den Quadratwurzeln von 4 gegebenen Zahlen.
 *
 *  Ermittelt mittels der MillerRabinTest::searchSqrt-Methode die Quadratwurzeln
 *  von 4 gegebenen Zahlen. Die Ergebnisse werden zusammen mit der Information,
 *  ob es sich jeweils um eine ganzzahlige Wurzel handelt, auf der Konsole ausgegeben.
 *
 */
void sqrtTest()
{
     cout << endl;
     cout << "Sqrt Test" << endl
          << "=========" << endl
          << endl;

     /*************************************************************************
      * Aufgabe 5b.
      *************************************************************************/

     // Lege konstantes Array der zu prüfenden Zahlen an.
     const char *liste[] = {
         "11944789308433963188362869029471041566864144",
         "35834367925301889565088607088413124700592432",
         "82459222190377411904521629217281504933358750190521",
         "61374642523762897753653566439246517537628495634413694129"};

     for (int i = 0; i < 4; i++) // Iteriere über jede der gegebenen Zahlen
     {
          Integer current(liste[i]); // Initialisere den C-String zu einem Integer
          Integer wurzel;
          bool ist_ganzzahl = MillerRabinTest::searchSqrt(current, wurzel); // Suche nach Quadratwurzel mittels searchSqrt;

          cout << "Wurzel fuer " // Ausgabe auf der Konsole
               << current << ": "
               << wurzel << " "
               << " Ganzzahl? " << ist_ganzzahl
               << endl;
     }
}

/**
 * @brief Sucht nach den k-ten Wurzeln von 4 gegebenen Zahlen.
 *
 *  Ermittelt mittels der MillerRabinTest::searchRoot-Methode die k-ten Wurzeln
 *  von 4 gegebenen Zahlen. Die Ergebnisse werden zusammen mit der Information,
 *  ob es sich jeweils um eine ganzzahlige Wurzel handelt, auf der Konsole ausgegeben.
 *
 */
void searchRootTest()
{
     cout << endl;
     cout << "Search Root Test:" << endl
          << "=================" << endl
          << endl;

     /*************************************************************************
      * Aufgabe 7b.
      *************************************************************************/

     // Lege konstantes Array der zu prüfenden Zahlen an.
     const char *liste[] = {
         "433769751949605918939094356724813641",
         "5639006775344876946208226637422577333",
         "70462039047496669316177512730246460325447",
         "70462039047496669316177512730246460325447"};

     const int k[] = {7, 7, 5, 9};

     for (int i = 0; i < 4; i++) // Iteriere über jede der gegebenen Zahlen
     {
          Integer current(liste[i]); // Initialisere den C-String zu einem Integer
          Integer wurzel;
          bool ist_ganzzahl = MillerRabinTest::searchRoot(current, k[i], wurzel); // Suche nach k[i]-ten Wurzel mittels searchRoot;

          cout << i << "-te Wurzel fuer " // Ausgabe auf der Konsole
               << current << ": "
               << wurzel << " "
               << " Ganzzahl? " << ist_ganzzahl
               << endl;
     }
}

/**
 * @brief Testet, ob es sich bei gegebenen Zahlen um Ganzzahlpotenzen handelt.
 *
 *  Ermittelt mittels der MillerRabinTest::isIntegerPower-Methode ob es sich bei gegebenen
 *  Zahlen um Ganzzahlpotenzen handelt. Das Ergebnis wird jeweils für jede Zahl auf der
 *  Konsole ausgegeben.
 *
 */
void isIntegerPowerTest()
{
     cout << endl;
     cout << "isIntegerPowerTest() Test:" << endl
          << "=========================="
          << endl
          << endl;

     /*************************************************************************
      * Aufgabe 8b.
      *************************************************************************/

     // Lege konstantes Array der zu prüfenden Zahlen an.
     const char *liste[] = {
         "11944789308433963188362869029471041566864144",
         "35834367925301889565088607088413124700592432",
         "82459222190377411904521629217281504933358750190521",
         "61374642523762897753653566439246517537628495634413694129",
         "3124432031290254610011894949223517352998211575328796815860858733"};

     for (int i = 0; i < 5; i++) // Iteriere über jede der gegebenen Zahlen
     {
          Integer current(liste[i]); // Initialisere den C-String zu einem Integer
          Integer basis;
          unsigned int exponent;
          bool ist_ganzzahlpotenz = MillerRabinTest::isIntegerPower(current, basis, exponent);

          // Ausgabe der Werte auf der Konsole
          if (ist_ganzzahlpotenz)
          {
               cout << "Die Zahl "
                    << current
                    << " ist eine Ganzzahlpotenz bestehend aus Basis: "
                    << basis
                    << " und Exponent "
                    << exponent << endl;
          }
          else
          {
               cout << "Die Zahl "
                    << current
                    << " ist keine Ganzzahlpotenz"
                    << endl;
          }
     }
}

/**
 * @brief Erzeugt Primzahlen mit jeweils 512, 1024 und 2048 bits.
 *
 *  Nutzt den Fortuna PRNG und die Miller Rabin Tests um Primzahlen
 *  mit in der Aufgabe vorgegebenen Breiten zu generieren.
 *
 */
void randomPrimes()
{
     cout << endl;
     cout << "Random Primes:" << endl
          << "==============" << endl
          << endl;

     /*************************************************************************
      * Aufgabe 9.
      *************************************************************************/
     FortunaGenerator O = FortunaGenerator();     // Erstellen neues PRNG Objekt

     Integer a = O.getInteger(64);  // Generieren von 3 Pseudozufallszahlen (64, 128 und 256 bytes jeweils)
     Integer b = O.getInteger(128);
     Integer c = O.getInteger(256);

     while (MillerRabinTest::isPrime(&O, a, 50) == false) // IsPrime überprüft, ob a eine Primzahl ist.
     {
          a = O.getInteger(64);    // Neue Zufallszahl generieren, falls a keine Primzahl ist
     }

     cout << "512 bit Primzahl:" << a << endl; // Konsolenausgabe

     while (MillerRabinTest::isPrime(&O, b, 50) == false) // IsPrime überprüft, ob b eine Primzahl ist.
     {
          b = O.getInteger(128); // Neue Zufallszahl generieren, falls b keine Primzahl ist.
     }

     cout << "1024 bit Primzahl:" << b << endl; // Konsolenausgabe

     while (MillerRabinTest::isPrime(&O, c, 50) == false) // IsPrime überprüft, ob c eine Primzahl ist.
     {
          c = O.getInteger(256); // Neue Zufallszahl generieren, falls c keine Primzahl ist.
     }

     cout << "2048 bit Primzahl:" << c << endl; // Konsolenausgabe
}

int main()
{
     cout << "Generierung von Primzahlen:" << endl
          << "==========================="
          << endl
          << endl;

     fortuna();
     witnessTest();
     isPrimeTest();
     sqrtTest();
     searchRootTest();
     expTest();
     randomPrimes();

     return 0;
}
