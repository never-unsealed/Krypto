#ifndef RABINDECRYPTOR_H_
#define RABINDECRYPTOR_H_

#include <vector>
#include "integer.h"

using namespace CryptoPP;
using namespace std;

class RabinDecryptor {
private:
	Integer p;
	Integer q;
	Integer padding;
	Integer offset;

public:
	RabinDecryptor(const Integer& p, const Integer& q, const Integer& padding=Integer("987654321"));
	virtual ~RabinDecryptor();

	bool compute(const Integer& y, vector<Integer>& xv);
	bool compute(const Integer& y, Integer& x);

	bool compute2(const Integer& y, Integer& x);
};

#endif /* RABINSYSTEM_H_ */
