#ifndef _PublicKeyAlgorithms_h_
#define _PublicKeyAlgorithms_h_
#include <vector>
#include "integer.h"

using namespace std;
using namespace CryptoPP;

class PublicKeyAlgorithmBox {
public:

	unsigned int computeConvergents(const Integer& a, const Integer& b,
			vector<Integer>& c, vector<Integer>& d);

	Integer euklid(const Integer& a, const Integer& b, vector<Integer>& q);

	bool EEA(const Integer& a, const Integer& b, Integer& d,
			Integer& x, Integer& y);

	Integer modularExponentation(const Integer& a, const Integer& b,
			const Integer& n);

	bool multInverse(const Integer& a, const Integer& n, Integer& a_inv);

	bool witness(const Integer& a, const Integer& n);

	bool millerRabinTest(Integer& n, unsigned int s);

	Integer randomInteger(const Integer& n);

	unsigned int randomPrime(Integer &p, unsigned int bitlen, unsigned int s);

	unsigned int randomRabinPrime(Integer &p, unsigned int bitlen,
			unsigned int s);

	bool modPrimeSqrt(const Integer& y, const Integer& p, vector<Integer>& v);

	bool sqrt(const Integer& x, Integer& s) const;

	void generateRSAParams(Integer& p, Integer& q, Integer& e, Integer& d,
			unsigned int bitlen = 256, unsigned int s = 30);

    string vectorToString(const vector<Integer>& v);
};

#endif
