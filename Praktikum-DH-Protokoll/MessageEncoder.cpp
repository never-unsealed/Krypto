/*!
 * \file
 * \brief Implementation of the Diffie-Hellman protocol
 */

#include <iostream>
#include <set>
#include <hex.h> // HexEncoder
#include <sha.h>
#include <aes.h>
#include "MessageEncoder.h"

MessageEncoder::MessageEncoder() {
}

void MessageEncoder::append(const byte* buffer, unsigned int size) {
	string result;

	ArraySource(buffer, size, true, new HexEncoder(new StringSink(result)));
	//cout << result << endl;
	data.push_back(result);
}

void MessageEncoder::append(const string& s) {
	string result;
	StringSource(s, true, new HexEncoder(new StringSink(result)));
	data.push_back(result);
}

void MessageEncoder::append(const Integer& x) {
	unsigned n = x.ByteCount();
	byte* buffer = new byte[n];

	x.Encode(buffer, n);
	append(buffer, n);

	delete[] buffer;
}

void MessageEncoder::clear() {
	data.clear();
}

bool MessageEncoder::decode(const string& in_msg) {
	string msg = in_msg;
	string valid_chars="1234567890abcdefABCDEF#";
	set<char> char_set;

	data.clear();
	if (msg.length()==0) {
		return true;
	}

	/*
	 * Create a set containing the valid characters.
	 */
	for (int i=0; i<valid_chars.size(); i++) {
		char_set.insert(valid_chars[i]);
	}

	/*
	 *  Check whether the msg is a bundle of hex strings
	 *  separated by #
	 */
	for (int i = 0; i < msg.size(); i++) {
		if (char_set.count(msg[i])==0) {
			// Invalid character detected!
			return false;
		}
	}

	string block;
	int sep_pos = msg.find('#',0);
	while (sep_pos>=0) {
		data.push_back(msg.substr(0,sep_pos));

		msg = msg.substr(sep_pos+1);
		sep_pos = msg.find('#',0);
	}
	data.push_back(msg);

	return true;
}

string MessageEncoder::encode() const {
	if (data.size()==0) {
		return string("");
	}

	string result=data[0];

	for (int i=1; i<data.size(); i++) {
		result = result + "#" + data[i];
	}

	return result;
}

Integer MessageEncoder::getInteger(unsigned int i)  const {
	if (i>data.size()) {
		return Integer::Zero();
	}

	Integer result;
	unsigned int size = data[i].size()/2;
	byte* buffer = new byte[size];
	StringSource(data[i], true, new HexDecoder(new ArraySink(buffer, size)));
	result.Decode(buffer, size);
	delete[] buffer;
	return result;
}

string MessageEncoder::getString(unsigned int i)  const {
	if (i>data.size()) {
		return "";
	}

	string result;
	StringSource(data[i], true, new HexDecoder(new StringSink(result)));
	return result;
}

unsigned int MessageEncoder::size() const {
	return data.size();
}


string MessageEncoder::sign(const string& key) {
	SHA256 hash;
	string digest;

	string message = key + encode();

	StringSource(message, true,
			new HashFilter(hash, new HexEncoder(new StringSink(digest))));

	return digest;
}

bool MessageEncoder::verify(const string& key, const string& digest) {
	return (digest==sign(key));
}
