/*!
 * \file
 * \brief Implementation of the Diffie-Hellman protocol
 */

#include <cassert>
#include <fstream>
#include <asio.hpp>
#include "FortunaGenerator.h"
#include "MessageEncoder.h"
#include "DHProtocol.h"

using asio::ip::tcp;
using namespace std;
using namespace CryptoPP;

DHProtocol::DHProtocol(Integer &p, Integer &q, Integer &alpha)
{
    this->p = p;
    this->q = q;
    this->alpha = alpha;
}

DHProtocol::DHProtocol(const string &param_file)
{
    ifstream file;
    string data;
    MessageEncoder encoder;

    file.open(param_file.c_str());
    if (file.is_open() == false)
    {
        cout << "Could not open file: " << param_file << endl;
        assert(false);
    }

    file >> data;
    file.close();

    if (encoder.decode(data) == false)
    {
        cout << "Could not decode DH parameters." << endl;
        assert(false);
    }

    if (encoder.size() != 3)
    {
        cout << "Wrong number of DH parameters." << endl;
        assert(false);
    }

    q = encoder.getInteger(0);
    p = encoder.getInteger(1);
    alpha = encoder.getInteger(2);

    /*
     cout << "q = " << q << endl;
     cout << "p = " << p << endl;
     cout << "alpha = " << alpha << endl;
     */
}

/**
 * @brief Implementiert die Clientseite des DH Protokolls.
 *
 * Generiert einen Wert a mithilfe des FortunaGenerators und berechnet:
 * calc = alpha hoch a mod p. Anschließend wird calc an kodiert an Bob gesendet und seine Antwort ausgelesen und dekodiert (Wert b).
 * Der Schlüssel kann durch modulare Exponentation calc^b mod p berechnet werden.
 *
 * @param host Der Hostname von Bob.
 * @param port Der Port, auf dem Bob lauscht.
 */
void DHProtocol::alice(const string &host, const string &port)
{
    /*************************************************************************
     * Aufgabe 4c.
     *************************************************************************/

    FortunaGenerator prng;
    string response;
    MessageEncoder encoder;
    MessageEncoder decoder;

    Integer a = prng.getInteger(this->p); // Erhalte eine Zufallszahl a mit a <= p - 1

    Integer calc = a_exp_b_mod_c(this->alpha, a, p); // Berechne alpha hoch a mod p

    tcp::iostream stream(tcp::resolver::query{host, port}); // Verbinde zu bob

    // Überprüfe, ob Verbindung erfolfreich war
    if (!stream)
    {
        cout << "Verbindung fehlgeschlagen" << endl;
        return;
    }

    // Füge das Ergebnis von a_exp_b_mod_c in den data-Vektor des Encoders hinzu
    encoder.append(calc);

    stream << encoder.encode() << endl; // Sende das kodierte Ergebnis an Bob

    getline(stream, response); // Lese die Antwort von Bob aus

    cout << "Antwort von Bob: " << response << endl;

    // Dekodiere die Antwort von Bob
    if (!decoder.decode(response))
    {
        cout << "Decode fehlgeschlagen" << endl;
        return;
    }

    // Berechne den Schlüssel und gebe ihn aus.
    cout << "Der Schlüssel lautet: " << (a_exp_b_mod_c(decoder.getInteger(0), a, this->p)) << endl;
}

void DHProtocol::serverBob(const string &port)
{
    cout << "Acting as Bob." << endl;

    try
    {

        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), stoi(port)));

        while (true)
        {
            cout << endl
                 << "Server Bob: waiting for incoming connections." << endl;
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

/**
 * @brief Implementiert die Serverseite des DH Protokolls.
 *
 * Generiert einen Wert b mithilfe des FortunaGenerators.
 * Empfängt den Wert A von Alice (Client), dekodiert diesen und berechnet als modulare Exponentation den Wert B (Variable calc).
 * B = alpha^b mod p. Sendet B kodiert an Alice zurück. Berechnet den Schlüssel durch A^b mod p.
 *
 * @param alice_strm Ein Netzwerk-Stream mit dem Client (Alice).
 */
bool DHProtocol::bob(tcp::iostream &alice_strm)
{
    /*************************************************************************
     * Aufgabe 4c.
     *************************************************************************/

    FortunaGenerator prng;
    MessageEncoder decoder; // Erstelle decoder Objekt
    MessageEncoder encoder; // Erstelle encoder Objekt
    string in_msg;
    Integer b = prng.getInteger(this->p);
    Integer calc;

    getline(alice_strm, in_msg); // Lese Wert A des Client aus

    // Dekodiere die Eingabe des Clients
    if (!decoder.decode(in_msg))
    {
        cout << "Decode fehlgeschlagen" << endl;
        return false;
    }

    // Berechne B
    calc = a_exp_b_mod_c(this->alpha, b, this->p);

    // Füge B an den data-Vektor des Encoders hinzu.
    encoder.append(calc);

    // Sende B an Alice
    alice_strm << encoder.encode() << endl;

    // Berechne Schlüssel und gebe ihn aus
    cout << "Der Schlüssel lautet: " << (a_exp_b_mod_c(decoder.getInteger(0), b, p)) << endl;

    return true;
}

void DHProtocol::serverOscar(const string &inport, const string &host, const string &port)
{
    cout << endl
         << "Acting as Oscar." << endl;

    try
    {

        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), stoi(inport)));

        while (true)
        {
            cout << endl
                 << "Server Oscar: waiting for incoming connections." << endl;
            tcp::iostream stream;
            if (!stream)
            {
                cout << "Error: " << stream.error().message() << endl;
            }
            else
            {
                acceptor.accept(*stream.rdbuf());
                oscar(stream, host, port);
            }
        }
    }
    catch (exception &err)
    {
        cout << err.what() << endl;
    }
}

/**
 * @brief Fängt die DH-Schlüsselverhandlung zwischen Server und Client ab.
 *
 * Gibt sich aus Sicht des Clients als Server aus und verhandelt wie bob einen Schlüssel mit Alice.
 * Gibt sich aus Sicht des Servers als Client aus und verhandelt wie Alice einen separaten Schlüssel mit Bob.
 * Dadurch kann sämtlicher Datenverkehr zwischen Alice und Bob mitgelesen werden.
 *
 * @param alice_strm Ein Netzwerk-Stream zu Alice.
 * @param host Der Hostname von Bob.
 * @param port Der Port, auf dem Bob lauscht.
 */
bool DHProtocol::oscar(tcp::iostream &alice_strm, const string &host, const string &port)
{
    /*************************************************************************
     * Aufgabe 4e.
     *************************************************************************/

    FortunaGenerator prng;
    MessageEncoder decoder_alice; // Erstelle decoder Objekt
    MessageEncoder encoder_alice; // Erstelle encoder Objekt
    MessageEncoder decoder_bob;   // Erstelle decoder Objekt
    MessageEncoder encoder_bob;   // Erstelle encoder Objekt
    string in_msg, response;
    Integer calc_alice, calc_bob;

    Integer a = prng.getInteger(this->p); // Erhalte eine Zufallszahl a mit a <= p - 1
    Integer b = prng.getInteger(this->p); // Erhalte eine Zufallszahl a mit b <= p - 1

    // 1. Verhandle Schlüssel mit Alice (und gebe dich als Bob aus)

    getline(alice_strm, in_msg); // Lese Wert A des Client aus

    cout << "Lese A von Alice aus..." << endl;

    // Dekodiere die Eingabe des Clients
    if (!decoder_alice.decode(in_msg))
    {
        cout << "Decode fehlgeschlagen" << endl;
        return false;
    }

    // Berechne B
    calc_alice = a_exp_b_mod_c(this->alpha, b, this->p);

    // Füge B an den data-Vektor des Encoders hinzu.
    encoder_alice.append(calc_alice);

    // Sende B an Alice
    alice_strm << encoder_alice.encode() << endl;

    cout << "Sende B an Alice..." << endl;

    // Berechne Schlüssel und gebe ihn aus
    cout << "Der Schlüssel fuer Alice lautet: " << (a_exp_b_mod_c(decoder_alice.getInteger(0), b, p)) << endl;

    // 2. Verhandle Schlüssel mit Bob (und gebe dich als Alice aus)

    calc_bob = a_exp_b_mod_c(this->alpha, a, this->p); // Berechne alpha hoch a mod p

    tcp::iostream stream(tcp::resolver::query{host, port}); // Verbinde zu bob

    // Überprüfe, ob Verbindung erfolfreich war
    if (!stream)
    {
        cout << "Verbindung fehlgeschlagen" << endl;
        return false;
    }

    // Füge das Ergebnis von a_exp_b_mod_c in den data-Vektor des Encoders hinzu
    encoder_bob.append(calc_bob);

    cout << "Sende A an Bob..." << endl;

    stream << encoder_bob.encode() << endl; // Sende das kodierte Ergebnis an Bob

    getline(stream, response); // Lese die Antwort von Bob aus

    cout << "Lese B von Bob aus..." << endl;

    // Dekodiere die Antwort von Bob
    if (!decoder_bob.decode(response))
    {
        cout << "Decode fehlgeschlagen" << endl;
        return false;
    }

    // Berechne den Schlüssel und gebe ihn aus.
    cout << "Der Schlüssel für Bob lautet: " << (a_exp_b_mod_c(decoder_bob.getInteger(0), a, this->p)) << endl;

    return true;
}
