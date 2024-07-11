#include <iostream>
#include <cstdlib>
#include "DHProtocol.h"

using namespace std;

void help(string name) {
	cout << "Usage: " << name << " <mode>" << endl << endl;
	cout << "Modes:" << endl;
	cout << "  Alice <hostname> <port>" << endl;
	cout << "  Bob <port>" << endl;
	cout << "  Oscar <inport> <hostname> <port>" << endl;
}

int main(int argc, char **argv) {
	DHProtocol dh("data/dhparams.txt");

	if (argc < 2) {
		help(argv[0]);
		return 1;
	}

	string mode = argv[1];

	if (mode == "Alice") {
		if (argc<3) {
			help(argv[0]);
			return 1;
		} else {
			string host(argv[2]);
			string port(argv[3]);
			dh.alice(host, port);
		}
	}

	if (mode == "Bob") {
		if (argc<2) {
			help(argv[0]);
			return 1;
		} else {
			string port(argv[2]);
            dh.serverBob(port);
		}
	}

	if (mode == "Oscar") {
		if (argc<3) {
			help(argv[0]);
			return 1;
		} else {
			string inport(argv[2]);
			string host(argv[3]);
			string port(argv[4]);
            dh.serverOscar(inport, host, port);
		}
	}
	return 0;
}
