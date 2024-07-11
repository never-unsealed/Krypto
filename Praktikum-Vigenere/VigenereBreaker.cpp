#include <cassert>
#include <map>
#include <iomanip>
#include <set>
#include <string>
#include <vector>
#include <cstring>
#include "VigenereBreaker.h"

void VigenereBreaker::analyse(const vector<byte> &cipher_text) {
    int kasiski_len;

    // Check length of cipher text
    if (cipher_text.size() == 0) return;

    // Perform Kasiski test
    kasiski_len = kasiskiTest(cipher_text);

    cout << "Kasiski test: " << kasiski_len << endl;

}

int VigenereBreaker::gcd(int a, int b) const {
    int r;

    do {
        r = a % b;
        a = b;
        b = r;
    } while (r != 0);

    return a;

}

int VigenereBreaker::gcd(const vector<int> &v) const {
    int r, i;

    assert(v.size() > 0);

    if (v.size() == 1) {
        return v[0];
    }
    else {

        r = gcd(v[0], v[1]);
        i = 2;
        while ((r > 1) && (i < v.size())) {
            r = gcd(r, v[i]);
            i++;
        }
        return r;
    }
}

/**
 * @brief Fuehrt den Kasiski-Text auf einen angegeben Ciphertext aus.
 *
 * Die kasiskiTest-Methode analysiert einen Ciphertext,
 * indem er diesen in Ngramme unterteilt und unter Betrachtung
 * von deren relativen Abständen die Schluessellaenge bestimmt. 
 * @param cipher_text Der zu analysierende Ciphertext.
 * @param ngram_len Die Laenge eines Ngrams, anhand dessen der Ciphertext analysiert werden soll.
 * @param verbose Aktiviert zusaetzliche Konsolenausgaben.
 * @return Der groesste gemeinsame Teiler aus den relativen Abstaenden der Ngramme.
 */
int VigenereBreaker::kasiskiTest
        (
                const vector<byte> &cipher_text,
                int ngram_len,
                bool verbose
        ) {
    /*************************************************************************
     * Aufgabe 4.
     *************************************************************************/
    int max_gcd = -1, prev = 0;
    std::string ciphertextStr, currNgram;
    std::map<std::string, std::vector<int>> ngramMap;
    std::map<int, int> gcdMap;
    std::vector<int> currPos, gcdTmp;

    //1. Byte-Array in String umwandeln.
    for (unsigned int i = 0; i < cipher_text.size(); i++)
        ciphertextStr += (cipher_text[i] + 65);

    //2. Ngramme aus dem Text erstellen und in Map speichern.
    for (unsigned int i = 0; i <= ciphertextStr.size() - ngram_len; i++)
    {
        currNgram = ciphertextStr.substr(i, ngram_len);
        currPos.clear();
        currPos.push_back(i);

        ngramMap[currNgram].push_back(i);
    }

    //3. GCD aus den relativen Abständen der Ngramme berechnen und in Map speichern
    //Ausgabe des jeweiligen Ngramms und dessen Positionen
    for (auto const& current : ngramMap)
    {
        if (current.second.size() < 3)
            continue;

        gcdTmp.clear();

        for (unsigned int i = 1; i < current.second.size(); i++)
            gcdTmp.push_back(current.second[i] - current.second[0]);

        gcdMap[VigenereBreaker::gcd(gcdTmp)]++;

        if (verbose)
        {
            std::cout << current.first << ": ";

            for (unsigned int i = 0; i < current.second.size(); i++)
                std::cout << std::to_string(current.second[i]) << " ";

            std::cout << "-> gcd: " << VigenereBreaker::gcd(gcdTmp) << std::endl;
        }
    }

    if(verbose)
        std::cout << "\n" << "Result:" << std::endl;

    //4. Ausgabe und Berechung des am häufigsten auftretenden GCD
    for (auto const& current : gcdMap)
    {
        if(verbose)
            std::cout << 
                "gcd " <<setw(2) << right  << current.first << ": number of occurrences = " << current.second << std::endl;
    
        if (prev > current.second)
            continue;

        prev = current.second;
        max_gcd = current.first;
    }
    return max_gcd;
}

/**
 * @brief Berechnet den Koinzidenzindex fuer einen angegebenen Ciphertext aus.
 *
 * Die Methode berechnet den Koinzidenzindex von einem gegebenen Ciphertext,
 * d.h. die Wahrscheinlichkeit, dass zwei zufällig aus dem Text ausgewählte
 * Buchstaben identisch sind
 * @param text Der zu analysierende Ciphertext.
 * @return Der Koinzidenzindex des Textes als Float.
 */
float VigenereBreaker::coincidenceIndex(const vector<byte> &text) {
    /*************************************************************************
     * Aufgabe 6.
     *************************************************************************/
    std::map<byte, unsigned int> Haeufigkeiten;
    char currentElement;
    unsigned int counter = 0;
    float ci = 0;
    
    for (auto currentElement : text)
    {
        counter = 0;
        for (unsigned int t = 0; t < text.size(); t++)
        {
            if (currentElement == text[t])
            {
                counter++;
            }
        }
        Haeufigkeiten.insert({ currentElement, counter });
    }
    for (auto Elem : Haeufigkeiten)
    {
        ci += Elem.second * ((Elem.second) - 1);
    }

    return (ci / (text.size() * ((text.size()) - 1)));
}

/**
 * @brief Fuehrt die Koinzidenzindex-Methode fuer einen angegebenen Ciphertext aus.
 *
 * Die Methode unterteilt den Text in Spalten und berechnet jeweils deren 
 * Koinzidenzindex. Diese werden gegen den Schwellwert verglichen.
 * @param cipher_text Der zu analysierende Ciphertext.
 * @param cols Laenge des Schluessels, mit dem der Text geschuetzt wird.
 * @param threshold Schwellwert fuer den Koinzidenz-Test.
 * @param verbose Aktiviert zusaetzliche Konsolenausgaben.
 * @return true wenn alle Koinzidenzindexe ueber dem threshold liegen, ansonsten false.
 */
bool VigenereBreaker::coincidenceTest
        (
                const vector<byte> &cipher_text,
                int cols,
                float threshold,
                bool verbose
        ) {
    /*************************************************************************
     * Aufgabe 7.
     *************************************************************************/
    bool result = true;

    vector<float> indices;
    vector<byte> currentRow;

    // 1. Assemble rows

    for (unsigned int i = 0; i < cols; i++)
    {
        currentRow.clear();

        for (unsigned int x = i; x < cipher_text.size(); x += cols)
        {
            currentRow.push_back(cipher_text[x]);
        }

        // 2. Indizes für jede Spalte berechnen

        indices.push_back(coincidenceIndex(currentRow));
    }

    if (verbose)
    {
        cout << setw(2) << right << cols;
        cout << " columns:";
    }
    for (unsigned int i = 0; i < indices.size(); i++)
    {
        if (verbose)
            cout << " " << fixed <<setprecision(8)<< indices[i];

        // 3. Liegt der Wert unter dem Threshold?

        if (indices[i] <= threshold)
            result = false;
    }

    if (verbose)
        cout << endl;

    return result;
}

/**
 * @brief Analysiert einen angegebenen Ciphertext mittels des Gegenseitiger Koinzidenzindex.
 *
 * Berechnung des gegenseitigen Koinzidenzindex zweier Geheimtextspalten.
 * @param cipher_text Der zu analysierende Ciphertext.
 * @param cols Spaltenanzahl.
 * @param col_i Erste Spalte, die aus dem Ciphertext ausgelesen werden soll.
 * @param col_j Zweite Spalte, die aus dem Ciphertext ausgelesen werden soll.
 * @param threshold Schwellwert fuer den Test.
 * @param verbose Aktiviert zusaetzliche Konsolenausgaben.
 * @return Die Verschiebung mit maximalem Index, falls dieser ueber dem Schwellwert liegt, ansonsten -1.
 */
int VigenereBreaker::mutualCoinIndex
(
    const vector<byte> &cipher_text,
    int cols,
    int col_i,
    int col_j,
    float threshold,
    bool verbose) {
    /*************************************************************************
     * Aufgabe 9.
     *************************************************************************/

    vector<byte> rowFirst, rowSecond;
    map<byte, float> distance;
    float tmpSum = 0, maxProb = 0, currentReturnIndex = 0;
    byte freqFirst[26], freqSec[26];

    memset(freqFirst, 0, 26);
    memset(freqSec, 0, 26);

    if (verbose)
    {
        cout << setw(3) << right << col_i << ",";
        cout << setw(3) << right << col_j << ":";
    }

    // 1. Die Spalten auslesen

    for (unsigned int i = col_i; i < cipher_text.size(); i += cols)
        rowFirst.push_back(cipher_text[i]);

    for (unsigned int i = col_j; i < cipher_text.size(); i += cols)
        rowSecond.push_back(cipher_text[i]);

    // 2. Die H ufigkeit der Buchstabeln in den Spalten abspeichern.

    for (unsigned int i = 0; i < rowFirst.size(); i++)
        freqFirst[rowFirst[i]]++;

    for (unsigned int i = 0; i < rowSecond.size(); i++)
        freqSec[rowSecond[i]]++;

    // 3. Anwendung der Formel auf die H ufigkeit
    float size1 = rowFirst.size();
    float size2 = rowSecond.size();
    float toatlSize = size1 * size2;

    for (int o = 0; o < 26; o++)
    {
        tmpSum = 0;

        for (int i = 0; i < 26; i++)
            tmpSum += freqFirst[i] * freqSec[(i + o) % 26];

        tmpSum = tmpSum / toatlSize;

        o ? distance[26 - o] = tmpSum : distance[o] = tmpSum;
    }

    // 4. Suche den maximalen

    for (auto const &current : distance)
    {
        if (current.second < maxProb)
            continue;

        maxProb = current.second;

        if (current.first == 0) {
            currentReturnIndex = 0;
        }
        else {
            currentReturnIndex = 26 - current.first;
        }

    }

    if (verbose)
    {
        for (int i = 0; i < 26; i++) {
            if (i % 7 == 0 && i != 0)
                cout << "\n" << setw(8) << "" << setw(4) << right << i << ": " << fixed << setprecision(3) << distance[i];
            else
            {
                cout << setw(4) << right << i << ": " << fixed << setprecision(3) << distance[i];
            }

            if (i == 25)
            {
                cout << "\n";
            }
        }

        if (maxProb > threshold)
        {
            cout << setw(8) << right << "max:" << setw(4) << right << setprecision(0) << (currentReturnIndex == 0 ? currentReturnIndex : 26 - currentReturnIndex) << ": " << setprecision(3) << maxProb << " => key[" << col_j << "] = key[" << col_i << "] + " << setprecision(0) << currentReturnIndex << endl;
        }
        else
        {
            cout << setw(11) << right << "max: < " << threshold << endl;
        }
        cout << endl;
    }

    return maxProb > threshold ? currentReturnIndex : -1;
}
