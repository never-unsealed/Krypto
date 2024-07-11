#ifndef _BlumBlumShubGenerator_h_
#define _BlumBlumShubGenerator_h_

#include "integer.h" // Integer
#include "PRNG.h"

using namespace CryptoPP;

class BlumBlumShubGenerator : public PRNG {
private:
	Integer n;
	Integer s;

public:
	BlumBlumShubGenerator(const Integer& n, const Integer& seed);

	virtual bool getBit();

};

#endif
