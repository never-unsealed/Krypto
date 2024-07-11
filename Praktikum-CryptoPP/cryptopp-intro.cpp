/**
 * @file cryptopp-intro.cpp
 */

#include <cassert>
#include <iostream>
#include <string>
#include "filters.h" // StringSink
#include "hex.h"     // HexEncoder
#include "sha.h"
#include "aes.h"
#include "modes.h"
#include "base64.h"
#include "files.h"    // FileSink
#include "osrng.h"    // BlockingRNG
#include "integer.h"  // Integer
#include "nbtheory.h" // RabinMillerTest
#include "BlumBlumShubGenerator.h"

using namespace std;
using namespace CryptoPP;

/**
 * @brief Fuehrt Base64-Decoding bzw. Encoding Operationen mittels CryptoPP durch.
 *
 * Diese Funktion nutzt die CryptoPP Library, um zunächst einen String in Bas64 zu enkodieren.
 * Anschließend wird ein gegebener String dekodiert und ausgegebenen. Zuletzt wird eine Datei
 * ausgelesen und deren Inhalt Base64-dekodiert und ausgegeben.
 */
void DataFlowExercise()
{
    /*************************************************************************
     * Aufgabe 2.
     *************************************************************************/
    cout << endl
         << "Data Flow Exercise:" << endl
         << "===================" << endl
         << endl;

    /*************************************************************************
     * Aufgabe 2.A)
     *************************************************************************/
    cout << "Aufgabe 2 a)" << endl;
    string src = "Kryptographie macht Spaß!!!";

    string dst;
    StringSink *sink = new StringSink(dst); // Neues Sink fuer den Ausgabe-String erstellen.

    HexEncoder *encoder = new HexEncoder(sink); // Neuer Encoder wird erstellt, der sink nutzt.

    StringSource(src, true, encoder); // Der src String wird eingelesen und enkodiert.
    cout << dst << endl;

    /*************************************************************************
     * Aufgabe 2. B)
     *************************************************************************/
    cout << "Aufgabe 2 b)" << endl;
    src = "4963682062696E2065696E20537472696E6720696E204865786164657A696D616C6B6F64696572756E6721";
    dst = "";
    StringSink *secSink = new StringSink(dst);     // Neues Sink fuer den Ausgabe-String erstellen.
    HexDecoder *decoder = new HexDecoder(secSink); // Neuer Decoder wird mit dem Ausgabesink initialisiert.

    StringSource(src, true, decoder); // Einlesen des Quell-Strings in den Dekodierer.
    cout << dst << endl;

    /*************************************************************************
     * Aufgabe 2. C)
     *************************************************************************/

    string str;

    FileSource("data/base64data.txt", true, new Base64Decoder(new StringSink(str))); // Einlesen der Datei, dekodieren des Inhalts und Ausgabe ueber ein Sink in die 'str' Variable
    cout << "Aufgabe 2 c)" << endl;
    cout << str << endl;
    /**/
}

/**
 * @brief Fuehrt AES Ent- und Verschluesselung mittels CryptoPP durch.
 *
 * Diese Funktion nutzt die CryptoPP Library, um zunächst eine Zeichenfolge mit einem
 * festgelegten Schluessel mit der AES-Methode im ECB Modus zu verschluesseln.
 * Das Resultat dieser Operation wird anschließend wieder entschluesselt und ausgegeben.
 * Zuletzt wird eine verschluesselte Nachricht aus einer Datei ausgelesen, entschuesselt
 * und der Klartext ausgegeben.
 */
void EncryptionExercise()
{
    /*************************************************************************
     * Aufgabe 3.
     *************************************************************************/
    cout << endl
         << "Encryption Exercise:" << endl
         << "====================" << endl
         << endl;

    cout << "Aufgabe 3 a)" << endl;

    try
    {
        string cipher_text = "";
        byte key[16];
        string encoded_key = "08a8cbfe7a3d1262c8abc3d1197dfefe";
        StringSource(encoded_key, true,            // Einlesen des Hex-Kodierten Strings
                     new HexDecoder(               // Dekodieren des Strings
                         new ArraySink(key, 16))); // Befuellen des key Arrays mit dem dekodierten Schluessel

        string plain_text = "Streng geheime Botschaft";

        ECB_Mode<AES>::Encryption aes_enc; // Erstellen des AES-Encryption Objekts im ECB Modus

        aes_enc.SetKey(key, sizeof(key)); // Setzen des Schluessels fuer die Operation

        StringSource(plain_text, true,                                                  // Einlesen des Klartextes
                     new StreamTransformationFilter(aes_enc,                            // Anwenden des Filters, um Pipelining mit AES zu ermoeglichen
                                                    new HexEncoder(                     // Kodieren des Ergebnisses
                                                        new StringSink(cipher_text)))); // Schreiben der Ausgabe in die cipher_text-Variable
        cout << cipher_text << endl;

        cout << "Aufgabe 3 b)" << endl;

        string decrypted = "";

        ECB_Mode<AES>::Decryption aes_dec; // Erstellen des AES-Decryption Objekts im ECB Modus

        aes_dec.SetKey(key, sizeof(key)); // Setzen des Schluessels

        StringSource(cipher_text, true,                                               // Einlesen des verschluesselten Textes
                     new HexDecoder(                                                  // Dekodieren des Hex-Strings
                         new StreamTransformationFilter(aes_dec,                      // Entschluesselung
                                                        new StringSink(decrypted)))); // Schreiben der Ausgabe in die decrypted-Variable
        cout << decrypted << endl;

        cout << "Aufgabe 3 c)" << endl;

        byte keyfile[16];
        byte iv[16];
        string encoded_keyfile = "47656865696D65725363686CC3BC7373";
        string encoded_iv = "496E697469616C6973696572756E6773";
        string decrypted_file;

        // Initialisieren des Schluessels
        StringSource(encoded_keyfile, true,
                     new HexDecoder(
                         new ArraySink(keyfile, 16)));

        // Initialisieren des Initialisierungsvektors
        StringSource(encoded_iv, true,
                     new HexDecoder(
                         new ArraySink(iv, 16)));

        CTR_Mode<AES>::Decryption aes_dec_ctr; // Erstellen des AES-Decryption Objekts im CTR Modus

        aes_dec_ctr.SetKeyWithIV(keyfile, 16, iv); // Setzen des Schluessels

        FileSource("data/aescipher.txt", true,                                        // Einlesen der Datei
                   new Base64Decoder(                                                    // Base64-Dekodierung des Dateiinhalts.
                       new StreamTransformationFilter(aes_dec_ctr,                       // Entschluesselung
                                                      new StringSink(decrypted_file)))); // Schreiben der Ausgabe in decrypted_file

        cout << decrypted_file << endl;
    }
    catch (const Exception &e)
    {
        cerr << e.what() << endl;
        assert(false);
    }
}

/**
 * @brief Fuehrt Hashing-Operationen mittels CryptoPP durch.
 *
 * Diese Funktion erstellt eine SHA-256 Pruefsumme eines festgelegten
 * Strings und verifiziert anschließend den String mittels der Pruefsumme.
 * Anschließend wird die Nachricht veraendert und der gleiche Test durchgefuehrt,
 * um zu demonstrieren, was passiert, wenn die Puefsumme nicht mit der Nachricht
 * uebereinstimmt. Zuletzt wird ein Beispiel mit der Flag "HASH_AT_END" aufgezeigt.
 */
void HashExercise()
{
    /*************************************************************************
     * Aufgabe 4.
     *************************************************************************/
    cout << endl
         << "Hash Exercise:" << endl
         << "==============" << endl
         << endl;

    cout << "Aufgabe 4 a)" << endl;

    string message = "Kryptographie macht immer noch Spaß!!!";
    string digest;
    SHA256 hash;

    StringSource(message, true,                           // Einlesen der Nachricht.
                 new HashFilter(hash,                     // Hashen der Nachricht.
                                new StringSink(digest))); // Schreiben der Ausgabe in digest.

    // Initialisieren der Flags
    word32 flags = HashVerificationFilter::HASH_AT_BEGIN | HashVerificationFilter::PUT_RESULT;

    bool result;
    hash.Restart();
    StringSource(digest + message, true,                                                    // Einlesen des Digst und der Nachricht.
                 new HashVerificationFilter(hash,                                           // Verifikation der Nachricht.
                                            new ArraySink((byte *)&result, sizeof(result)), // Ergebnis der Pruefung in result.
                                            flags));

    // "result" erhaelt nun entweder 1 fuer TRUE oder 0 fuer FALSE
    cout << "Verifikation: " << result << endl;

    cout << "Aufgabe 4 b)" << endl;

    message[0] = 'P'; // Veraendern der Nachricht.

    hash.Restart();
    StringSource(digest + message, true,                                                    // Einlesen des Digst und der Nachricht.
                 new HashVerificationFilter(hash,                                           // Verifikation der Nachricht.
                                            new ArraySink((byte *)&result, sizeof(result)), // Ergebnis der Pruefung in result.
                                            flags));

    cout << "Verifikation: " << result << endl;

    cout << "Aufgabe 4 c)" << endl;

    // Initialisieren der Flags mit HASH_AT_END
    flags = HashVerificationFilter::HASH_AT_END | HashVerificationFilter::PUT_RESULT;

    // Originale Nachricht wiederherstellen.
    message[0] = 'K';

    // Gleich wie oben, aber diesmal mit der message zuerst.
    StringSource(message + digest, true,
                 new HashVerificationFilter(hash,
                                            new ArraySink((byte *)&result, sizeof(result)),
                                            flags));

    cout << "Verifikation: " << result << endl;
}

/**
 * @brief Implementiert den modexp Algorithmus aus dem Vorkurs der Vorlesung.
 */
Integer modexp(Integer a, Integer b, Integer n)
{
    Integer c = 0;
    Integer d = 1;
    size_t k = b.BitCount(); // Zahl der gesetzten Bits

    for (int i = k - 1; i >= 0; i--)
    {
        c = 2 * c;
        d = (d * d) % n;

        if (b.GetBit(i)) // Ist das Bit an Stelle i gesetzt (also 1)?
        {
            c++;
            d = (d * a) % n;
        }
    }

    return d;
}

/**
 * @brief Demonstriert den Integer-Typ der CryptoPP Bibliothek.
 *
 * Diese Funktion erstellt Zahlen vom Typ Integer und nutzt diese, um die
 * oben Implementierte modexp und die von CryptoPP bereitgestellte Funktion
 * a_exp_b_mod_c aufzurufen. Die Ausgaben dieser Funktionen werden verglichen.
 * Zuletzt wird mit der modexp ermittelt, ob es sich bei einer Zahl um einen
 * quadratischen Rest handelt.
 */
void IntegerExercise()
{
    /*************************************************************************
     * Aufgabe 5bc.
     *************************************************************************/
    cout << endl
         << "IntegerExercise:" << endl
         << "================" << endl
         << endl;

    cout << "Aufgabe 5 b)" << endl;

    // Erstellen der Integer-Variablen.
    Integer a("371");
    Integer b("18961551");
    Integer n("2371");

    // Vergleich der Ausgabe von modexp (siehe obem) und a_exp_b_mod_c
    if (modexp(a, b, n) == a_exp_b_mod_c(a, b, n))
    {
        cout << "Ausgabe gleich!" << endl;
    }

    cout << "Aufgabe 5 c)" << endl;

    // Erstellen der Integer-Variablen.
    Integer x("58788");
    Integer y("95957");
    Integer z((y - 1) / 2);

    // Ein Rueckgabewert von 1 gibt an, dass es sich um einen quadratischen Rest handelt.
    if (modexp(x, z, y) == 1)
    {
        cout << "Test erfolgreich." << endl;
    }
    else
    {
        cout << "Kein quadratischer Rest." << endl;
    }
}

/**
 * @brief Fuehrt Operationen mit Psuedozufallszahlen durch.
 *
 * Nutzt den BlumBlumShubGenerator, um zunaechst mit gegebenen Werten eine Folge von
 * 20 Zufallsbits zu erzeugen und gibt diese anschliessend aus. Es wird ein Array mit 100 Zufallsbytes erstellt und ausgegeben.
 * Ein Integer der Groesse 200 Bytes wird erzeugt und ausgegeben. Letztendlich wird eine Zufallszahl
 * aus der Menge {0, 1, . . . , 2^30 − 1} erzeugt und ausgegeben.
 */
void RNGExercise()
{
    /*************************************************************************
     * Aufgabe 6cfhj.
     *************************************************************************/
    cout << endl
         << "RNGExercise:" << endl
         << "============" << endl
         << endl;

    cout << "Aufgabe 6 c)" << endl;

    // Initialisieren von Integern mit den gegebenen Werten.
    Integer p("1252910265243849922375596598575099209083498535192739493227403");
    Integer q("1476222059624949757818143837507324048590620075519516306265283");
    Integer s("42");

    // Erstellen des Generator-Objekts.
    BlumBlumShubGenerator *generator = new BlumBlumShubGenerator(p * q, s);

    for (unsigned int i = 0; i < 20; i++)
        cout << generator->getBit() << " "; // Generierung und Ausgabe der Bits.

    cout << endl;

    cout << "Aufgabe 6 f)" << endl;

    // Erstellen des Test Arrays mit 100 Bytes.
    byte testArray[100];

    // Fuellen des Arrays mit Zufallsbytes.
    generator->getBlock(testArray, 100);

    // Ausgabe

    for (unsigned int i = 0; i < 100; i++)
        cout << (int)testArray[i] << " "; // Ausgabe der Bytes.

    cout << endl;

    cout << "Aufgabe 6 h)" << endl;

    // Erstellen und Ausgeben eines Integers mit 200 Bytes.
    cout << generator->getInteger(200) << endl;

    cout << "Aufgabe 6 j)" << endl;

    // Erstellen des Grenzwertes als Integer.
    Integer m("1073741824"); // 2^30

    // Ausgabe des Integers.
    cout << generator->getInteger(m) << endl;
}

int main()
{
    DataFlowExercise();
    EncryptionExercise();
    HashExercise();
    IntegerExercise();
    RNGExercise();

    return 0;
}