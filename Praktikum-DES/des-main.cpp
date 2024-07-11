#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include "DESCipher.h"
#include "des-getopt.h"

using namespace std;

// #main()
int main(int argc, char **argv) {
    DESCipher des;
    ifstream in_file;
    ofstream out_file;
    byte *in_text, *out_text;
    int in_len, out_len;
    byte key[8];
    int i, j;
    bool encmode;

    gengetopt_args_info ai;

    if (cmdline_parser(argc, argv, &ai) != 0) {
        exit(1);
    }

    if (argc == 1) {
        cmdline_parser_print_help();
        return 0;
    }

    if (!ai.key_given) {
        cerr << "ERROR: No key given." << endl;
        exit(1);
    }

    /*
     * Check correctness of the key: it must be a lowercase string
     * of length 2
     */
    if (strlen(ai.key_arg) != 16) {
        cerr << "ERROR: Key must be an hexadecimal string of length 16." << endl;
        exit(1);
    }
    else {
        j = 0;
        for (i = 0; i < 16; i += 2) {
            if ((isxdigit(ai.key_arg[i]) == 0) || (isxdigit(ai.key_arg[i+1]) == 0)) {
                cerr << "ERROR: Invalid key (no hex number at position "
                    << i << "/" << (i+1) << ")." << endl;
                exit(1);
            }
            else {
                key[j] = 16 * des.hexToByte(ai.key_arg[i]) + des.hexToByte(ai.key_arg[i + 1]);
                j++;
            }
        }

    }

    /*
     * From here, the array key contains a valid key.
     */

    /*
     *
     */
    if (ai.decrypt_given) {
        encmode = false;
    } else if (ai.encrypt_given) {
        encmode = true;
    } else {
        cerr << "ERROR: no mode (encrypt/decrypt) given." << endl;
        exit(1);
    }


    if (ai.inputs_num != 1) {
        cerr << "No input file given!" << endl;
        exit(1);
    } else {
        in_file.open(ai.inputs[0]);
        if (in_file.is_open()) {
            in_len = des.readStream(in_file, in_text, !encmode);
            in_file.close();
        } else {
            cerr << "ERROR: Cannot read file " << ai.inputs[0] << endl;
            exit(1);
        }
    }

    out_text = new byte[in_len];
    if (encmode == true) {
        des.encrypt(in_text, in_len, key, 8, out_text, in_len);
    }
    else {
        des.decrypt(in_text, in_len, key, 8, out_text, in_len);
    }


    if (ai.out_given) {
        out_file.open(ai.out_arg);
        if (out_file.is_open() == false) {
            cerr << "ERROR: Could not open output file >" << argv[4] << "<" << endl;
            exit(1);
        } 
        else {
            des.writeStream(out_file, out_text, in_len, encmode);
            out_file.close();
        }
    } 
    else {
        des.writeStream(cout, out_text, in_len, encmode);
    } 
    
    delete[] in_text;
    delete[] out_text;

    return 0;


} // main()
