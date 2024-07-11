#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "VigenereBreaker.h"
#include "searchshift-getopt.h"

using namespace std;

int main(int argc, char **argv) {
    VigenereBreaker vbreaker;
    ifstream in_file;
    vector<byte> plain_text, cipher_text, key;
    int i, j, columns, shift, len, line;
    byte keychar;
    float threshold = 0.065, ci;
    vector<vector<int> > keyshift;
    bool ok;

    gengetopt_args_info ai;

    if (cmdline_parser(argc, argv, &ai) != 0) {
        exit(1);
    }

    if (ai.inputs_num != 1) {
        cerr << "ERROR: Need exactly one input file." << endl;
        exit(1);
    }

    /*
     * Read maximum number oc columns to be analysed
     */
    columns = ai.columns_arg;
    if (columns <= 0) {
        cerr << "ERROR: Number of columns must be > 0." << endl;
        exit(1);
    }

    /*
     * Read threshold value
     */
    threshold = ai.threshold_arg;
    if ((threshold <= 0.0) or (threshold > 1.0)) {
        cerr << "ERROR: Threshold must be a real number between 0 and 1." << endl;
        exit(1);
    }

    /*
     * Read input file.
     */
    in_file.open(ai.inputs[0]);
    if (in_file.is_open() == false) {
        cerr << "Error: Could not open file >" << argv[1] << "<" << endl;
        exit(1);
    } else {
        vbreaker.readStream(in_file, cipher_text);
        in_file.close();
    }

    /*
   * Create key shift table 
   */
    for (i = 0; i < columns; i++) {
        keyshift.push_back(vector<int>(columns, 0));
    }

    // Analyse the cipher text
    for (i = 0; i < columns; i++) {
        for (j = 0; j < columns; j++) {
            if (i != j) {
                shift = vbreaker.mutualCoinIndex(cipher_text,
                                                 columns, i, j,
                                                 threshold,
                                                 true);
                keyshift[i][j] = shift;
            } else {
                keyshift[i][j] = 0;
            }
        }
    }

    // return 0;
    cout << "Key shift table: " << endl << endl;

    cout << "i/j: ";
    for (i = 0; i < columns; i++) {
        cout << setw(2) << i << "  ";
    }
    cout << endl;

    for (i = 0; i < columns; i++) {
        cout << setw(2) << i << ": ";
        for (j = 0; j < columns; j++) {
            cout << "[" << setw(2) << keyshift[i][j] << "]";
        }
        cout << endl;
    }

    for (line = 0; line < columns; line++) {

        /*
         * Check line for correctness
         */
        ok = true;
        for (i = 0; i < columns; i++) {
            if (keyshift[i][line] < 0) {
                ok = false;
            }
        }

        if (ok == true) {

            cout << endl << "Decrypting with key in line "
                 << line << ":" << endl << endl;
            /*
             * Check the corresponding keys
             */
            for (i = 0; i < 26; i++) {
                cout << "  ";
                // Construct key
                key = vector<byte>(columns, 0);
                // cout << (char)('a'+i);
                for (j = 0; j < columns; j++) {
                    keychar = (i + keyshift[line][j]) % 26;
                    key[j] = keychar;
                    cout << (char) ('a' + keychar);
                }
                cout << ": ";

                plain_text.clear();
                vbreaker.decrypt(cipher_text, key, plain_text);
                len = min(50, (int) plain_text.size());
                for (j = 0; j < len; j++) {
                    cout << (char) ('a' + plain_text[j]);
                }
                cout << endl;
            }
        }
    }

    return 0;

}
