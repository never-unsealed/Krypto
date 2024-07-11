#ifndef RABINENCRYPTOR_H_
#define RABINENCRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

class RabinEncryptor {
private:
	Integer n;
	Integer offset;
	Integer padding;

public:
	RabinEncryptor(const Integer& n, const Integer& padding=Integer("987654321"));
	virtual ~RabinEncryptor();

	bool compute(const Integer& x, Integer& y);

	bool compute2(const Integer&x, Integer& y);
};

#endif /* RABINENCRYPTOR_H_ */
