#include <iostream>
#include <iostream>
#include <asio.hpp>
#include "MessageEncoder.h"
#include "netintro.h"

using asio::ip::tcp;
using namespace std;
using namespace CryptoPP;

// Maximum size of the receive buffer
#define MAXSIZE 100000


void help(string name) {
    cout << "Usage: " << name << " <mode>" << endl << endl;
    cout << "Modes:" << endl;
    cout << "  Alice <hostname> <port>" << endl;
    cout << "  Bob <port>" << endl;
}

int main(int argc, char **argv) {
    /*
     * Depending on the command line arguments, the program
     * acts as Alice or Bob
     */
    if (argc < 2) {
        help(argv[0]);
        return 1;
    }

    string mode = argv[1];

    /*
     * Act as Alice.
     */
    if (mode == "Alice") {
        if (argc < 3) {
            help(argv[0]);
            return 1;
        } else {
            string host(argv[2]);
            string port = argv[3];
            alice(host, port);
            return 0;
        }
    }

    /*
     * Act as Bob.
     */
    if (mode == "Bob") {
        if (argc < 2) {
            help(argv[0]);
            return 1;
        } else {
            serverBob(argv[2]);
            return 0;
        }
    }

    help(argv[0]);
    return 0;
}
