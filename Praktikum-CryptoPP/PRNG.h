/*
 * PRNG.h
 *
 *  Created on: 13.04.2011
 *      Author: chkarg
 */

#ifndef PRNG_H_
#define PRNG_H_

#include "integer.h" // byte

using namespace CryptoPP;

class PRNG {
public:
	virtual bool getBit() = 0;

	virtual byte getByte();

	virtual void getBlock(byte* buffer, unsigned int n);

	virtual Integer getInteger(unsigned int size);

	virtual Integer getInteger(const Integer& m);
};

#endif /* PRNG_H_ */
