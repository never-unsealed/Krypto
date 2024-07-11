#include <vector>
#include "RabinDecryptor.h"
#include "PublicKeyAlgorithmBox.h"
#include <iostream>

using namespace std;

/**
 * @brief Konstruktor für die Klasse RabinDecryptor.
 *
 * Initialisiert die Instanz mit den gegebenen Parametern und setzt das Padding.
 *
 * @param p Ein Primfaktor für die Rabin-Entschlüsselung.
 * @param q Ein Primfaktor für die Rabin-Entschlüsselung.
 * @param padding Eine Ganzzahl, die für die Rabin-Padding-Anforderung verwendet wird.
 */
RabinDecryptor::RabinDecryptor(const Integer &p, const Integer &q,
                               const Integer &padding)
{
     /******************************************************************************
      * Aufgabe 11c  & 12a
      ******************************************************************************/

     // Initialisieren der Klassenvariablen
     this->p = p;
     this->q = q;
     this->padding = padding;
}

/**
 * @brief Entschlüsselt den Geheimtext y mit dem Rabin-Entschlüsselungsalgorithmus.
 *
 * @param y Der zu entschlüsselnde Geheimtext.
 * @param xv Vektor zur Speicherung möglicher Klartexte.
 * @return true, wenn die Entschlüsselung erfolgreich war, sonst false.
 */
bool RabinDecryptor::compute(const Integer &y, vector<Integer> &xv)
{
     /******************************************************************************
      * Aufgabe 11d
      ******************************************************************************/

     // Initialisierung einer Instanz von PublicKeyAlgorithmBox zur Verwendung der benötigten Algorithmen
     PublicKeyAlgorithmBox Box = PublicKeyAlgorithmBox();

     // Berechnung des Modulus n als Produkt der beiden Primfaktoren p und q
     Integer n = p * q;

     // Variablen zur Speicherung des größten gemeinsamen Teilers (gcd) sowie der Koeffizienten mq und mp
     Integer gcd, mq, mp;

     // Aufruf des erweiterten Euklidischen Algorithmus zur Berechnung von mq und mp
     // Dabei wird auch gcd zurückgegeben, ist aber in diesem Kontext nicht weiter verwendbar
     Box.EEA(p, q, gcd, mq, mp);

     // Berechnung der Wurzeln x1 und x2 modulo p
     Integer x1 = Box.modularExponentation(y, (p + 1) / 4, p);
     Integer x2 = p - x1;

     // Berechnung der Wurzeln y1 und y2 modulo q
     Integer y1 = Box.modularExponentation(y, (q + 1) / 4, q);
     Integer y2 = q - y1;

     // Berechnung der vier möglichen Klartexte v1, v2, v3 und v4
     Integer v1 = (x1 * q * mp + y1 * p * mq) % n;
     Integer v2 = (x1 * q * mp + y2 * p * mq) % n;
     Integer v3 = (x2 * q * mp + y1 * p * mq) % n;
     Integer v4 = (x2 * q * mp + y2 * p * mq) % n;

     // Speichern der berechneten Klartexte in den Vektor xv
     xv.clear();
     xv.push_back(v1);
     xv.push_back(v2);
     xv.push_back(v3);
     xv.push_back(v4);

     // Rückgabe von true, da Rabin-Entschlüsselung typischerweise immer erfolgreich ist
     return true;
}

/**
 * @brief Entschlüsselt den Geheimtext y mit dem Rabin-Entschlüsselungsalgorithmus
 *        und speichert den Klartext in x.
 *
 * @param y Der zu entschlüsselnde Geheimtext.
 * @param x Referenz auf eine Variable vom Typ Integer, in der der Klartext gespeichert wird.
 * @return true, wenn die Entschlüsselung erfolgreich war, sonst false.
 */
bool RabinDecryptor::compute(const Integer &y, Integer &x)
{
     /******************************************************************************
      * Aufgabe 11e
      ******************************************************************************/

     // Initialisierung einer Instanz von PublicKeyAlgorithmBox zur Verwendung der benötigten Algorithmen
     PublicKeyAlgorithmBox Box = PublicKeyAlgorithmBox();

     // Schritt 1: Berechnung der Wurzeln m1 und m2
     Integer m1 = Box.modularExponentation(y, (p + 1) / 4, p);
     Integer m2 = Box.modularExponentation(y, (q + 1) / 4, q);

     // Schritt 2: Verwendung der erweiterten Euklidischen Algorithmus-Funktion (EEA)
     // Berechnung von a und b, wobei gcd(p, q) = 1 ist, da p und q Primzahlen sind
     Integer gcd, a, b;
     Box.EEA(p, q, gcd, a, b);

     // Schritt 3: Berechnung des Modulus n
     Integer n = p * q;

     // Schritt 4: Berechnung des ersten möglichen Klartexts x1
     Integer x1 = (a * p * m2 + b * q * m1) % n;
     if (x1 < 0)
          x1 += n; // Sicherstellen, dass x1 positiv ist

     // Speichern des berechneten Klartexts in der Referenzvariablen x
     x = x1;

     // Rückgabe von true, da die Entschlüsselung erfolgreich war
     return true;
}

/**
 * @brief Entschlüsselt eine Nachricht mit Padding.
 *
 * Diese Methode entschlüsselt den verschlüsselten Integer-Wert y und entfernt das Padding, um den ursprünglichen Wert x zu erhalten.
 * Es wird zunächst die compute-Methode aufgerufen, um alle möglichen Entschlüsselungen in einem Vektor zu erhalten.
 * Danach wird der richtige entschlüsselte Wert gefunden, indem das Padding entfernt wird.
 *
 * @param y Der verschlüsselte Text.
 * @param x Der entschlüsselte Text ohne Padding.
 * @return true, wenn die Entschlüsselung erfolgreich war und der ursprüngliche Wert gefunden wurde, ansonsten false.
 */
bool RabinDecryptor::compute2(const Integer &y, Integer &x)
{
     /******************************************************************************
      * Aufgabe 12b
      ******************************************************************************/

     // Deklariere Vektor v zur Speicherung der möglichen Entschlüsselungen
     vector<Integer> v;

     // Rufe compute Methode auf, um mögliche Entschlüsselungen zu erhalten
     if (!compute(y, v))
          return false; // Rückgabe false, wenn die compute Methode fehlschlägt

     Integer j = 1; // Initialisiere j auf 1
     Integer i = 0; // Initialisiere i auf 0

     // Berechne die Anzahl der Ziffern in padding
     for (Integer temp = padding; temp != 0; temp /= 10)
     {
          i++; // Erhöhe i für jede Ziffer in padding
     }

     // Multipliziere j mit 10 für jede Ziffer in padding
     for (Integer w = 0; w < i; w++)
     {
          j *= 10; // Multipliziere j mit 10
     }

     // Iteriere über alle Einträge des Vektors v
     for (int f = 0; f < v.size(); f++)
     {
          // Berechne den aktuellen Wert modulo j
          Integer current = v[f] % j;

          // Überprüfe, ob der aktuelle Wert gleich padding ist
          if (current == padding)
          {
               x = v[f] / j; // Entferne das Padding
               return true; // Rückgabe true, wenn der ursprüngliche Wert gefunden wurde
          }
     }

     return false; // Rückgabe false, wenn kein gültiger entschlüsselter Wert gefunden wurde
}

RabinDecryptor::~RabinDecryptor()
{
}
