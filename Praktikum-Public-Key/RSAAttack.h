/*
 * RSAAttack.h
 *
 *  Created on: 30.12.2013
 *      Author: chkarg
 */

#ifndef RSAATTACK_H_
#define RSAATTACK_H_

#include "integer.h"
#include "RSAOracle.h"

using namespace CryptoPP;

class RSAAttack {
public:
	RSAAttack();
	virtual ~RSAAttack();

	bool factorizeN(const Integer& n, const Integer& phi_n, Integer& p, Integer& q) const;

	bool wienerAttack(const Integer& n, const Integer& e, Integer& p, Integer& q) const;

	Integer halfAttack(const Integer& n, const Integer& e, const Integer& y, RSAOracle& rsa_oracle) const;

};

#endif /* RSAATTACK_H_ */
