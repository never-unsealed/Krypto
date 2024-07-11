/*
 * MessageEncoder.h
 */

#ifndef MESSAGEENCODER_H_
#define MESSAGEENCODER_H_

#include<vector>
#include<string>
#include<hex.h>
#include<integer.h>

using namespace std;
using namespace CryptoPP;

class MessageEncoder {
protected:
	/**
	 * Vector containing the list of encoded values.
	 */
	vector<string> data;

	/**
	 * Append a byte array of length size to the data vector
	 *
	 * @param buffer pointer tu the byte array
	 * @param size length of the byte array
	 */
	void append(const byte* buffer, unsigned int size);

public:

	/**
	 * Konstruktor.
	 */
	MessageEncoder();

	/**
	 * Append a string to the data vector
	 *
	 * @param s string to be added
	 */
	void append(const string& s);

	/**
	 * Append an integer to the data vector
	 *
	 * The integer is encoded using the appropriate Encode()
	 * member function.
	 *
	 * @param x integer to be added
	 */
	void append(const Integer& x);

	/**
	 * Clear the data vector. This is, reset the instance.
	 */
	void clear();

	/**
	 * Decode a given message.
	 *
	 * @param msg data block to be decoded
	 *
	 * @returm true, if decoding was successful, false, otherwise.
	 */
	bool decode(const string& msg);

	/**
	 * Encode the current elements of the data vector
	 */
	string encode() const;

	/**
	 * Get the i-th element, decoded as an integer.
	 *
	 * @return integer decoded from the i-th element in data
	 */
	Integer getInteger(unsigned int i) const;

	/**
	 * Get the i-th element, decoded as a string.
	 *
	 * @return string decoded from the i-th element in data
	 */
	string getString(unsigned int i) const;

	/**
	 * Get the number of elements in data.
	 *
	 * @return size of the data vector
	 */
	unsigned int size() const;

	/**
	 * Compute a SHA-256 over the elements in data.
	 *
	 * @param key secret key for MAC computation
	 *
	 * @return MAC encoded as an hexadecimal string
	 */
	string sign(const string& key);

	/**
	 * Verify
	 */
	bool verify(const string& key, const string& digest);
};

#endif /* MESSAGEENCODER_H_ */
