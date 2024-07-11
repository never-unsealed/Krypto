#ifndef RSADECRYPTOR_H_
#define RSADECRYPTOR_H_

#include "integer.h"

using namespace CryptoPP;

class RSADecryptor {
private:
	Integer p;
	Integer q;
	Integer d;
	Integer n;

public:
	RSADecryptor(const Integer& p, const Integer& q, const Integer& d);
	virtual ~RSADecryptor();

	bool compute(const Integer& y, Integer& x) const;

	bool crt(const Integer& y, Integer& x) const;

	bool garner(const Integer& y, Integer& x) const;
};

#endif /* RSADECRYPTOR_H_ */
