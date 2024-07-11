#ifndef UTILS_TOOLBOX_H_
#define UTILS_TOOLBOX_H_

#include <iostream>
#include <string>

using namespace std;

class Toolbox {
public:
	Toolbox();
	virtual ~Toolbox();

	static string prune(const string& s);

	static string format(const string& s, unsigned short columns = 8,
			unsigned short blocklen = 5, bool uppercase=false);

	static string readFromStream(istream &strm);

	static bool readFromFile(const string& filename, string& output);

	static bool writeToFile(const string& filename, const string& s);

	static string toLower(const string& s);

	static string toUpper(const string& s);

};

#endif /* UTILS_TOOLBOX_H_ */
