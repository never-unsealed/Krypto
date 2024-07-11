#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include "AESCipher.h"
#include "aes-getopt.h"

using namespace std;

// #main()
int main(int argc, char **argv) {
    AESCipher aes;
    ifstream in_file;
    ofstream out_file;
    vector<byte> in_text, out_text, key;
    int i, j;

    bool mode, flag;
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



    if (AESCipher::hexStringToVector(ai.key_arg, key)==false) {
        cerr << "ERROR: key is not a hex string!" << endl;
    }

    if (aes.setKey(key) == false) {
        cerr << "ERROR: Invalid key" << endl;
        exit(1);
    }

    /*
      *
      */
    if (ai.decrypt_given) {
        mode = AESCipher::Decryption;
    } else if (ai.encrypt_given) {
        mode = AESCipher::Encryption;
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
            if (mode==AESCipher::Encryption) {
                flag = AESCipher::readStream(in_file, in_text, false);
                in_text.push_back(0);
                while (in_text.size() % 16 != 0) {
                    in_text.push_back(1);
                }
            } else {
                flag = AESCipher::readStream(in_file, in_text, true);
            }
            in_file.close();
        } else {
            cerr << "ERROR: Cannot read file " << ai.inputs[0] << endl;
            exit(1);
        }
    }


    if (mode==AESCipher::Encryption) {
        aes.encrypt(in_text, out_text);
    }
    else {
        aes.decrypt(in_text, out_text);
    }

    if (ai.out_given) {
        out_file.open(ai.out_arg);
        if (out_file.is_open() == false) {
            cerr << "ERROR: Could not open output file >" << argv[4] << "<" << endl;
            exit(1);
        }
        else {
            if (mode==AESCipher::Encryption) {
                aes.writeStream(out_file, out_text, true);
            } else {
                aes.writeStream(out_file, out_text, mode);
            }

            out_file.close();
        }
    }
    else {
        if (mode==AESCipher::Encryption) {
            aes.writeStream(cout, out_text, true);
        } else {
            aes.writeStream(cout, out_text, false);
        }
        cout << endl;
    }

    return 0;


}
