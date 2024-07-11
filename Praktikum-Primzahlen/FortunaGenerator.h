/*
 * FortunaGenerator.h
 */

#ifndef FORTUNAGENERATOR_H_
#define FORTUNAGENERATOR_H_

#include <sys/time.h>
#include <stdint.h>
#include <time.h>
#include "PRNG.h"

class FortunaGenerator: public PRNG {
private:
	// Key
	byte key[32];
	// Counter
	byte cntr[16];
	// Flag (true => prng seeded and ready to use)
	bool seeded;
	// time stamp of last reseed
	uint32_t last_reseed;

protected:

	uint32_t getTimeStamp() const;

	void generateBlocks(byte* buffer, unsigned int n);

public:
	FortunaGenerator();

	bool incCntr();

	void reseed(byte* seed, unsigned int size);

	virtual bool getBit();

	virtual byte getByte();

	virtual void getBlock(byte* buffer, unsigned int n);

	virtual void getBlockV0(byte* buffer, unsigned int n);
};

#endif /* FORTUNAGENERATOR_H_ */
