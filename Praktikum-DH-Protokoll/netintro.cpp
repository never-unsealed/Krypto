/*!
 * \file
 */

#include <iostream>
#include "MessageEncoder.h"
#include "netintro.h"

using asio::ip::tcp;

using namespace std;
using namespace CryptoPP;

// Maximum size of the receive buffer
#define MAXSIZE 100000

/**
 * @brief Funktion für Alice, die eine Verbindung zu einem Server herstellt und Daten sendet.
 *
 * Diese Funktion sendet eine Nachricht an den Server und überträgt Integer-Werte
 * mithilfe eines MessageEncoders. Sie erwartet eine Antwort vom Server und dekodiert
 * die empfangene Nachricht, um einen Integer-Wert zu extrahieren und auszugeben.
 *
 * @param host Die IP-Adresse oder der Hostname des Servers.
 * @param port Der Port, über den die Verbindung zum Server hergestellt wird.
 */
void alice(string host, string port)
{
    tcp::iostream stream(tcp::resolver::query{host, port});
    Integer a = 10232;
    Integer b = 8934;

    // a und b an data anhängen.
    MessageEncoder encoder;
    encoder.append(a);
    encoder.append(b);

    if (!stream)
    {
        cout << "Error: " << stream.error().message() << endl;
    }
    else
    {
        string message = "Hello Server, I am Alice!";
        stream << message << endl;

        getline(stream, message);
        cout << "Got: " << message << endl;

        /*************************************************************************
         * Aufgabe 3c.
         *************************************************************************/

        stream << encoder.encode() << endl; // Sende encoded Daten an den Server

        getline(stream, message); // Lese Antwort des Servers
        MessageEncoder decoder;   // Erstelle decoder Objekt

        if (decoder.decode(message)) // Überprüfe ob die Dekodierung erfolgreich war
        {
            cout << "Got sum: " << decoder.getInteger(0) << endl; // Gebe dekodierten Integer aus.
        }
    }
}

/**
 * @brief Funktion für Bob, die eingehende Verbindungen akzeptiert und bearbeitet.
 *
 * Diese Funktion empfängt eine Nachricht vom Client, sendet eine Antwort zurück
 * und verarbeitet dann die empfangenen kodierten Daten. Sie dekodiert die Nachricht,
 * berechnet die Summe der beiden erhaltenen Integer-Werte und sendet das Ergebnis
 * zurück an den Client.
 *
 * @param stream Ein tcp::iostream-Objekt, das die Verbindung zum Client repräsentiert.
 * @return True, wenn die Verarbeitung erfolgreich war, sonst False.
 */
bool bob(tcp::iostream &stream)
{
    cout << "Accepting incoming connection." << endl;

    string in_msg;
    getline(stream, in_msg);

    cout << "Got: " << in_msg << endl;

    string out_msg = "Hello from Bob.";
    stream << out_msg << endl;

    /*************************************************************************
     * Aufgabe 3c.
     *************************************************************************/

    MessageEncoder decoder; // Erstelle decoder Objekt
    MessageEncoder encoder; // Erstelle encoder Objekt
    Integer sum;

    getline(stream, in_msg); // Lese weitere Eingabe des Client aus

    if (!decoder.decode(in_msg)) // Versuche die Nachricht zu dekodieren
        return false;

    sum = decoder.getInteger(0) + decoder.getInteger(1); // Addiere die beiden Integer und speichere sie in Summe
    encoder.append(sum);                                 // Hänge das Ergebnis in die data-Variable des encoder-Objekts an.

    stream << encoder.encode() << endl; // Kodiere das Ergebnis und sende es an den Client.

    return true;
}

void serverBob(string port)
{
    try
    {

        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), stoi(port)));

        while (true)
        {
            cout << "Server Bob: waiting for incoming connections." << endl;
            tcp::iostream stream;
            if (!stream)
            {
                cout << "Error: " << stream.error().message() << endl;
            }
            else
            {
                acceptor.accept(*stream.rdbuf());
                bob(stream);
            }
        }
    }
    catch (exception &err)
    {
        cout << err.what() << endl;
    }
}
