#ifndef RSAENCRYPTOR_H_
#define RSAENCRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

class RSAEncryptor {
private:
	Integer n;
	Integer e;

public:
	RSAEncryptor(const Integer& n, const Integer& e);
	virtual ~RSAEncryptor();

	bool compute(const Integer& x, Integer& y) const;
};

#endif /* RSAENCRYPTOR_H_ */
