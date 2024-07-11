#include <cassert>
#include <ctype.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "vigenere-getopt.h"
#include "VigenereCipher.h"

using namespace std;

// #main()
int main(int argc, char **argv) {
    VigenereCipher cipher;
    ifstream in_file;
    ofstream out_file;
    vector<byte> key, in_text, out_text;
    int i, j;
    unsigned char c, c_shift;

    gengetopt_args_info ai;

    if (cmdline_parser(argc, argv, &ai) != 0) {
        exit(1);
    }

    if (argc == 1) {
        cmdline_parser_print_help();
        return 0;
    }

    /*
     * Analyzing the key.
     */
    istringstream is(ai.key_arg);
    if (ClassicCipher::readStream(is, key)==0) {
        cerr << "ERROR: invalid key length: 0)" << endl;
        exit(1);
    }

    // The Vigenere cipher has a correct key.
    // Now, we check whether an input file is given.
    if (ai.inputs_num != 1) {
        cerr << "No input file given!" << endl;
        exit(1);
    } else {
        in_file.open(ai.inputs[0]);
        if (in_file.is_open()) {
            ClassicCipher::readStream(in_file, in_text);
            in_file.close();
        } else {
            cerr << "ERROR: Cannot read file " << ai.inputs[0] << endl;
            exit(1);
        }
    }

    /*
     * Encrypt/decrypt the input text.
     */
    if (ai.decrypt_given) {
        cipher.decrypt(in_text, key, out_text);
        c_shift = 'a';
    }
    else {
        cipher.encrypt(in_text, key, out_text);
        c_shift = 'A';
    }

    /*
     * Output the computed result.
     */
    if (ai.out_given) {
        out_file.open(ai.out_arg);
        if (out_file.is_open()) {
            cipher.writeStream(out_file, out_text, c_shift);
            out_file.close();
        }
        else {
            cerr << "ERROR: Could not open output file " << ai.out_arg << endl;
            exit(1);
        }
    }
    else {
        cipher.writeStream(cout, out_text, c_shift);
    }

    return 0;
}
