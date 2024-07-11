#ifndef RABINATTACK_H_
#define RABINATTACK_H_

#include "integer.h"
#include "RabinDecryptor.h"

using namespace CryptoPP;

class RabinAttack {
public:
	RabinAttack();
	virtual ~RabinAttack();

	int factorize(const Integer& n, Integer& f, int max_tries, RabinDecryptor& rabin_dec);
};

#endif /* RABINATTACK_H_ */
