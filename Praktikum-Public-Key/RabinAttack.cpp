#include "PublicKeyAlgorithmBox.h"
#include "RabinDecryptor.h"
#include "RabinAttack.h"
#include <iostream>

using namespace std;

RabinAttack::RabinAttack()
{
}

RabinAttack::~RabinAttack()
{
}

/**
 * @brief Faktorisierung von n mittels Rabin-Angriff.
 *
 * Diese Methode versucht, den Wert n durch einen Angriff auf das Rabin-Kryptosystem zu faktorisieren.
 * Es wird eine zufällige Ganzzahl r erzeugt und y = r^2 mod n berechnet.
 * Der Rabin-Entschlüsseler wird verwendet, um mögliche Quadratwurzeln von y zu finden.
 * Die Faktoren werden durch den größten gemeinsamen Teiler (GCD) berechnet.
 *
 * @param n Der zu faktorisierende Wert.
 * @param f Der gefundene Faktor von n.
 * @param max_tries Maximale Anzahl der Versuche, bevor aufgegeben wird.
 * @param rabin_dec Der Rabin-Entschlüsseler, der zur Entschlüsselung verwendet wird.
 * @return Die Anzahl der Versuche, die zum Finden eines Faktors benötigt wurden, oder -1, wenn kein Faktor gefunden wurde.
 */
int RabinAttack::factorize(const Integer &n, Integer &f, int max_tries,
                           RabinDecryptor &rabin_dec)
{
  /******************************************************************************
   * Aufgabe 13a
   ******************************************************************************/

  PublicKeyAlgorithmBox box; // Initialisiere die Box für kryptographische Algorithmen

  for (int i = 0; i < max_tries; i++) // Schleife über die maximale Anzahl der Versuche
  {
    Integer r = box.randomInteger(n - 1); // Generiere eine zufällige Ganzzahl r im Bereich [1, n-1]
    Integer y = box.modularExponentation(r, 2, n); // Berechne y = r^2 mod n

    vector<Integer> v; // Deklariere einen Vektor zur Speicherung der möglichen Quadratwurzeln von y

    if (rabin_dec.compute(y, v)) // Verwende den Rabin-Entschlüsseler, um mögliche Quadratwurzeln von y zu berechnen
    {
      for (int k = 0; k < v.size(); k++) // Schleife über alle gefundenen Quadratwurzeln
      {
        if (v[k] % n == r || (n - v[k]) % n == r) // Überprüfe, ob v[k] eine triviale Lösung ist
        {
          continue; // Wenn ja, überspringe diese Lösung
        }
        else
        {
          f = Integer::Gcd(v[k] - r, n); // Berechne den größten gemeinsamen Teiler (GCD) von v[k] - r und n

          if (f != 0 && f < n) // Überprüfe, ob ein nicht-trivialer Faktor gefunden wurde
          {
            return i + 1; // Gib die Anzahl der Versuche zurück, die zum Finden des Faktors benötigt wurden
          }
        }
      }
    }
  }

  return -1; // Gib -1 zurück, wenn kein Faktor nach der maximalen Anzahl der Versuche gefunden wurde
}
