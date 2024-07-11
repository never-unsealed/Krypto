#include <iostream>
#include "polybius-getopt.h"
#include "PolybiusCipher.h"
#include "Toolbox.h"

using namespace std;

int main(int argc, char *argv[]) {
    string plain_text;

    gengetopt_args_info ai;

    if (cmdline_parser(argc, argv, &ai) != 0) {
        exit(1);
    }

    if (argc == 1) {
        cmdline_parser_print_help();
        return 0;
    }

    /*
     * Generation of a random key.
     */
    if (ai.randomkey_given) {
        cout << PolybiusCipher::randomKey() << endl;
        return 0;
    }

    /*
     * Dump key table.
     */
    if (ai.table_given) {
        if (ai.key_given) {
            PolybiusCipher pc;

            if (pc.setKey(ai.key_arg)) {
                pc.printKey();
                return 0;
            } else {
                cerr << "Invalid key." << endl;
                return 1;
            }
        } else {
            cerr << "Need a key to dump a key table." << endl;
            return 1;
        }
    }

    /*
     * Encryption or decryption.
     */
    if (ai.encrypt_given || ai.decrypt_given) {
        PolybiusCipher pc;
        string data;

        // Try to set the key given at the command line.
        if (ai.key_given) {
            if (!pc.setKey(ai.key_arg)) {
                cerr << "Invalid key." << endl;
                exit(1);
            }
        } else {
            cerr << "Need a key for encrytion or decryption." << endl;
            exit(1);
        }

        // Polybius cipher has a correct key.
        // Now, we check whether an input file is given.
        if (ai.inputs_num != 1) {
            cerr << "No input file given!" << endl;
            exit(1);
        } else {
            if (Toolbox::readFromFile(ai.inputs[0], data)) {

            } else {
                cerr << "Error opening file " << ai.inputs[0] << endl;
                exit(1);
            }
        }

        /*
         * Now, a key and an input file is given.
         */
        string result;
        string output;
        if (ai.encrypt_given) {
            result = pc.encrypt(data);
            output = Toolbox::format(result, 20, 2);
        } else {
            result = pc.decrypt(data);
            output = Toolbox::format(result, 1, 50);
        }

        if (ai.out_given) {
            if (!Toolbox::writeToFile(ai.out_arg, output)) {
                cerr << "Error writing to file: " << ai.out_arg << endl;
                exit(1);
            }
        } else {
            cout << output;
        }

        exit(0);
    }

    return 0;
}
