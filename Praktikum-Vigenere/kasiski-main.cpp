#include <cassert>
#include <cstdlib>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include "VigenereBreaker.h"
#include "kasiski-getopt.h"

using namespace std;

int main(int argc, char **argv) {
    VigenereBreaker vbreaker;
    ifstream in_file;
    vector<byte> key;
    vector<byte> cipher_text;
    int ngram_len;

    gengetopt_args_info ai;

    if (cmdline_parser(argc, argv, &ai) != 0) {
        exit(1);
    }

    if (argc == 1) {
        cmdline_parser_print_help();
        return 0;
    }

    /*
     * Check whether at least one argument is given.
     */
    if (argc <= 1) {
        cerr << "Usage: " << argv[0]
             << " <in_file> [n-gram length]"
             << endl;
        exit(1);
    }

    ngram_len = ai.len_arg;
    if ((ngram_len<3) || (ngram_len>6)) {
        cerr << "Invalid ngram length: must be between 3 and 6." << endl;
        exit(1);
    }

    if (ai.inputs_num != 1) {
        cerr << "ERROR: No input file given!" << endl;
        exit(1);
    } else {
        /*
         * Read input file.
         */
        in_file.open(ai.inputs[0]);
        if (in_file.is_open()) {
            vbreaker.readStream(in_file, cipher_text);
            in_file.close();
        }
        else {
            cerr << "ERROR: Could not open file " << argv[1] << endl;
            exit(1);
        }

        // Analyze the cipher text
        int max_gcd = vbreaker.kasiskiTest(cipher_text, ngram_len, true);
        cout << endl << "Gcd with maximum number of occurrences: "  << max_gcd << endl;
        return 0;
    }
}
