/*
 * MillerRabinTest.h
 */

#ifndef MILLERRABINTEST_H_
#define MILLERRABINTEST_H_

#include "integer.h"
#include "PRNG.h"

using namespace CryptoPP;

class MillerRabinTest {
public:

	Integer boundedExponentation(const Integer& b, const Integer& e,
			const Integer& bound) const;

	static Integer modularExponentation(const Integer& a, const Integer& b,
			const Integer& n);

	static bool witness(const Integer& a, const Integer& n);

	bool witness2(const Integer& a, const Integer& n) const;

	static bool isStrongWitness(const Integer& a, const Integer& n);

	bool isPerfectPower(const Integer& n, Integer& b, Integer& e) const;

	Integer searchBase(const Integer& n, const Integer& e) const;

    static Integer exp(const Integer& b, const Integer& e);

	static bool searchSqrt(const Integer& n, Integer& r);

	static bool searchRoot(const Integer& n, unsigned int k, Integer& r);

    static bool isIntegerPower(const Integer& n, Integer& b, unsigned int& e);

    static bool extendedEuklid(const Integer& a, const Integer& b, Integer& d,
			Integer& x, Integer& y);

	static bool isPrime(PRNG* rng, const Integer& n, unsigned int s);

};

#endif /* MILLERRABINTEST_H_ */
