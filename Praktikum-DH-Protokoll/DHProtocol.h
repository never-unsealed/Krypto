/*
 * DHProtocol.h
 */

#ifndef DHPROTOCOL_H_
#define DHPROTOCOL_H_

#include <iostream>
#include <integer.h>
#include <asio.hpp>

using asio::ip::tcp;
using namespace std;
using namespace CryptoPP;

class DHProtocol {
protected:
	/*
	 *  Module of the Diffie-Hellman key exchange (must be prime)
	 */
	Integer p;
	/*
	 * Prime q such that p = 2*q + 1
	 */
	Integer q;
	/*
	 * Generator of Z^*_p
	 */
	Integer alpha;

	bool bob(tcp::iostream &alice_strm);

	bool oscar(tcp::iostream &alice_strm, const string& host, const string& port);

public:
	DHProtocol(const string& param_file);
	DHProtocol(Integer& p, Integer& q, Integer& alpha);

	void alice(const string& host, const string& port);

    void serverBob(const string& port);
	void serverOscar(const string& inport, const string& host, const string& port);
};

#endif /* DHPROTOCOL_H_ */
