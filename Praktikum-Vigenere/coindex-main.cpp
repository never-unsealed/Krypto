#include <cassert>
#include <cstdlib>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include "VigenereBreaker.h"
#include "coindex-getopt.h"

using namespace std;

int main(int argc, char **argv) {
    VigenereBreaker vbreaker;
    ifstream in_file;
    vector<byte> cipher_text;
    int i, max_cols;
    float threshold = 0.065;

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
    max_cols = ai.max_columns_arg;
    if (max_cols <= 0) {
        cerr << "ERROR: Maximum number of columns must be > 0." << endl;
        exit(1);
    }

    /*
     * Read threshold value
     */
    threshold = ai.threshold_arg;
    if ((threshold <= 0.0) || (threshold>1.0)) {
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
    }
    else {
        vbreaker.readStream(in_file, cipher_text);
        in_file.close();
    }

    // Analyse the cipher text
    for (i = 1; i <= max_cols; i++) {
        vbreaker.coincidenceTest(cipher_text, i, threshold, true);
    }

    return 0;

}
