/**
 *	@file rsaattack-main.cpp
 */

#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"
#include "RSAOracle.h"
#include "RSAAttack.h"
#include "RSADecryptor.h"
#include "RSAEncryptor.h"

using namespace std;

/**
 * @brief Führt einen Test der Methode zur Berechnung der Quadratwurzel mittels binärer Suche durch.
 *
 * Diese Funktion testet die Implementierung der sqrt-Methode in der Klasse PublicKeyAlgorithmBox.
 * Es werden verschiedene Testfälle für die Berechnung der Quadratwurzel von gegebenen Ganzzahlen durchgeführt.
 */
void sqrtTest()
{
    /*********************************************************************
     * Aufgabe 17b
     *********************************************************************/

    cout << endl;
    cout << "SQRT Test" << endl;
    cout << "=========" << endl
         << endl;

    PublicKeyAlgorithmBox box; // Instanziierung eines Objekts der Klasse PublicKeyAlgorithmBox

    // Testfall 1
    Integer int1("3157242151326374471752634944"); // Definition der Integer-Zahl int1
    Integer s1;                                   // Variable zur Speicherung der Quadratwurzel
    bool result1 = box.sqrt(int1, s1);            // Aufruf der sqrt-Methode für int1
    if (result1)
    {
        cout << "Für x = 3157242151326374471752634944 wurde s = " << s1 << " gefunden." << endl; // Ausgabe bei erfolgreicher Berechnung
        cout << "Erwartetes Ergebnis: s = 56189341972712" << endl;                               // Erwartetes Ergebnis für int1
    }
    else
    {
        cout << "Für x = 3157242151326374471752634944 wurde keine Quadratwurzel gefunden." << endl; // Ausgabe bei fehlender Quadratwurzel
    }
    cout << endl; // Leerzeile in der Ausgabe

    // Testfall 2
    Integer int2("11175843681943819792704729"); // Definition der Integer-Zahl int2
    Integer s2;                                 // Variable zur Speicherung der Quadratwurzel
    bool result2 = box.sqrt(int2, s2);          // Aufruf der sqrt-Methode für int2
    if (result2)
    {
        cout << "Für x = 11175843681943819792704729 wurde s = " << s2 << " gefunden." << endl; // Ausgabe bei erfolgreicher Berechnung
        cout << "Erwartetes Ergebnis: s = 3343029117723" << endl;                              // Erwartetes Ergebnis für int2
    }
    else
    {
        cout << "Für x = 11175843681943819792704729 wurde keine Quadratwurzel gefunden." << endl; // Ausgabe bei fehlender Quadratwurzel
    }
    cout << endl; // Leerzeile in der Ausgabe

    // Testfall 3
    Integer int3("3343229819990029117723"); // Definition der Integer-Zahl int3
    Integer s3;                             // Variable zur Speicherung der Quadratwurzel
    bool result3 = box.sqrt(int3, s3);      // Aufruf der sqrt-Methode für int3
    if (result3)
    {
        cout << "Für x = 3343229819990029117723 wurde s = " << s3 << " gefunden." << endl; // Ausgabe bei erfolgreicher Berechnung
        cout << "Erwartetes Ergebnis: Es gibt keine Quadratwurzel." << endl;               // Ausgabe für int3, da keine Quadratwurzel existiert
    }
    else
    {
        cout << "Für x = 3343229819990029117723 wurde keine Quadratwurzel gefunden." << endl; // Ausgabe bei fehlender Quadratwurzel
    }
    cout << endl; // Leerzeile in der Ausgabe
}

/**
 * @brief Testet die Methode zur Faktorisierung von n basierend auf phi(n).
 *
 * Diese Funktion testet die Implementierung der factorizeN-Methode in der Klasse RSAAttack.
 * Es werden verschiedene Testfälle mit vorgegebenen Werten für n und phi(n) durchgeführt.
 */
void factorizeAttack()
{
    /*********************************************************************
     * Aufgabe 18b
     *********************************************************************/
    cout << endl;
    cout << "Factorize Attack" << endl;
    cout << "================" << endl
         << endl;

    // Erstelle ein Objekt von RSAAttack, das die Faktorisierung durchführt
    RSAAttack attacker;

    // Testfall 1
    Integer n1("127869459623070904125109742803085324131");
    Integer phi_n1("127869459623070904102412837477002840200");
    Integer p1, q1;

    // Führe die Faktorisierung für n1 mit phi_n1 durch und speichere das Ergebnis in result1
    bool result1 = attacker.factorizeN(n1, phi_n1, p1, q1);

    // Überprüfe, ob die Faktorisierung erfolgreich war
    if (result1)
    {
        // Falls erfolgreich, gib die gefundenen Primfaktoren p1 und q1 aus
        cout << "Für n = 127869459623070904125109742803085324131 wurden die Faktoren p = " << p1 << " und q = " << q1 << " gefunden." << endl;
    }
    else
    {
        // Falls fehlgeschlagen, gib eine entsprechende Fehlermeldung aus
        cout << "Faktorisierung für n = 127869459623070904125109742803085324131 fehlgeschlagen." << endl;
    }

    cout << endl;
}

/**
 * @brief Testet die Methode euklid zur Berechnung des ggt und des Kettenbruchs von a und b.
 *
 * Diese Funktion testet die Implementierung der euklid-Methode in der Klasse PublicKeyAlgorithmBox.
 * Die Testfälle der Aufgabenstellung werden ausgeführt und ausgegeben.
 */
void euklidTest()
{
    /*********************************************************************
     * Aufgabe 19b
     *********************************************************************/

    cout << endl;
    cout << "Euklid Test" << endl;
    cout << "===========" << endl
         << endl;

    Integer a("39");
    Integer b("112");
    vector<Integer> v;

    PublicKeyAlgorithmBox box;

    // Euklid Algorithmus ausführen
    Integer d = box.euklid(a, b, v);

    // Ausgabe des ggt
    cout << "Euklid Rueckgabewert: " << d << endl;
    cout << "Vector v Inhalt: ";

    // Ausgabe jeder Zahl im Vektor
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";

    cout << endl;
}

/**
 * @brief Berechnet die Konvergenzen von vorgegebenen Werten a und b und gibt diese aus.
 *
 * Diese Funktion testet die Berechnung der Konvergenzen für die Zahlen a und b.
 * Die berechneten Konvergenzen werden in den Vektoren c und d gespeichert und anschließend
 * ausgegeben.
 */
void convergentsTest()
{
    /*********************************************************************
     * Aufgabe 20b
     *********************************************************************/

    cout << endl;
    cout << "Convergents Test" << endl;
    cout << "================" << endl
         << endl;

    PublicKeyAlgorithmBox box;

    Integer a("39");
    Integer b("112");
    vector<Integer> c, d;

    // Berechne Konvergenzen von a und b
    box.computeConvergents(a, b, c, d);

    // Gebe Vektor c aus
    cout << "Vektor c: ";

    for (int i = 0; i < c.size(); i++)
        cout << c[i] << " ";

    cout << endl;

    // gebe Vektor d aus
    cout << "Vektor d: ";

    for (int i = 0; i < d.size(); i++)
        cout << d[i] << " ";

    cout << endl;
}

/**
 * @brief Testet die wienerAttack Methode der RSAAttack Klasse.
 *
 * Die Funktion testet die wienerAttack Methode anhand der in der Aufgaben gegebenen
 * Werte. Wenn der Angriff erfolgreich ist, werden p und q ausgegeben.
 */
void wienerAttack()
{
    /*********************************************************************
     * Aufgabe 21b
     *********************************************************************/

    cout << endl;
    cout << "Wiener Attack" << endl;
    cout << "=============" << endl
         << endl;

    // Gegebene Werte aus der Aufgabe
    Integer n("224497286580947090363360894377508023561");
    Integer e("163745652718951887142293581189022709093");
    Integer q, p;

    RSAAttack attack;

    // Führe Winer Attacke durch
    if (attack.wienerAttack(n, e, p, q))
    {
        // Falls erfolgreich, gebe p und q aus.
        cout << "RSAAttack.wienerAttack hat true zurueckgegeben" << endl;
        cout << "p=" << p << endl;
        cout << "q=" << q << endl;
    }
    else
    {
        // Fehlermeldung bzw. Attacke nicht erfolgreich.
        cout << "RSAAttack.wienerAttack hat false zurueckgegeben" << endl;
    }
}

/**
 * @brief Testet die half Methode der RSAOracle Klasse.
 *
 * Die Funktion testet die half Methode anhand der in der Aufgaben gegebenen
 * Werte. Die Funktion gib die Ergebnisse der half-Operation auf der Konsole aus.
 */
void oracleTest()
{
    /*********************************************************************
     * Aufgabe 22c
     *********************************************************************/

    cout << endl;
    cout << "RSAOracle Test" << endl;
    cout << "==============" << endl
         << endl;

    // Gegebene Initialsierungswerte
    Integer p("16015510136412338011");
    Integer q("12177032305856164321");
    Integer d("946975621");

    RSAOracle oracle(p, q, d);

    // Gegebene Testwerte
    Integer test_vars[] =
        {
            Integer("116415012259126332853105614449093205668"),
            Integer("74304303162215663057995326922844871006"),
            Integer("102949691974634609941445904667722882083"),
            Integer("42549620926959222864355800078420537413")};

    for (int i = 0; i < 4; i++)
    {
        // Ausgabe für jeden Testwert
        cout << "Testwert " << test_vars[i] << " ergibt h=" << oracle.half(test_vars[i]) << endl;
    }
}

/**
 * @brief Führt den Half-Angriff durch, um eine verschlüsselte Nachricht zu entschlüsseln.
 *
 * Diese Funktion testet den Half-Angriff auf ein RSA-verschlüsseltes System. Sie erstellt 
 * einen RSA-Oracle und einen RSA-Verschlüsseler, um die verschlüsselte Nachricht y zu berechnen
 * und versucht dann, die ursprüngliche Nachricht x durch den Half-Angriff zu knacken.
 */
void halfAttack()
{
    /*********************************************************************
     * Aufgabe 23b
     *********************************************************************/

    cout << endl;
    cout << "Half Attack" << endl;
    cout << "===========" << endl << endl;

    // Erstelle ein Objekt von RSAAttack, das den Half-Angriff durchführt
    RSAAttack attack;

    // Initialisiere die gegebenen Werte als Integer
    Integer p("12889769717276679053");
    Integer q("17322528238664264177");
    Integer e("55051594731967684255289987977028610689");
    Integer d("149154082258429024247010774747829057473");
    Integer x("167092961114842952923160287194683529938");

    // Deklariere Variablen für den verschlüsselten Text y und die geknackte Nachricht cracked
    Integer y, cracked;

    // Erstelle ein Objekt von RSAOracle mit den Parametern p, q und d
    RSAOracle oracle(p, q, d);
    // Erstelle ein Objekt von RSAEncryptor mit dem Modulus n=p*q und dem öffentlichen Schlüssel e
    RSAEncryptor encryptor(p * q, e);

    // Verschlüssele die Nachricht x und speichere das Ergebnis in y
    if (!encryptor.compute(x, y))
    {
        cout << "RSAEncryptor.compute fehlgeschlagen" << endl;
        return;
    }

    // Führe den Half-Angriff durch und speichere das Ergebnis in cracked
    cracked = attack.halfAttack(p * q, e, y, oracle);

    cout << "Cracked=" << cracked << endl;

    // Überprüfe, ob die geknackte Nachricht der ursprünglichen Nachricht x entspricht
    if (cracked == x)
    {
        // Wenn ja, gib eine Erfolgsmeldung aus
        cout << "RSAAttack.halfAttack erfolgreich" << endl;
    }
    else
    {
        // Andernfalls gib eine Fehlermeldung aus
        cout << "RSAAttack.halfAttack nicht erfolgreich. Cracked != x" << endl;
    }
}

void randomTest()
{
    NonblockingRng rng;
    Integer a;

    a.Randomize(rng, 200);
    cout << a << endl;
}

int main()
{
    euklidTest();
    convergentsTest();
    sqrtTest();
    factorizeAttack();
    wienerAttack();
    oracleTest();
    halfAttack();
    // randomTest();

    return 0;
}
