#ifndef RSAORACLE_H_
#define RSAORACLE_H_

#include "integer.h"

using namespace std;
using namespace CryptoPP;

class RSAOracle {
private:
	Integer n;
	Integer d;

public:
	RSAOracle(Integer& p, Integer& q, Integer& d);
	virtual ~RSAOracle();

	bool half(const Integer& y) const;
	bool parity(const Integer& y) const;

};

#endif /* RSAORACLE_H_ */
