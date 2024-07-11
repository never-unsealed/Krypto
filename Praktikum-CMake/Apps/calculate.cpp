#include <iostream>
#include "Sinus.h"
#include "Square.h"

using namespace std;

int main() {
	float x=3.5;
	Square square;
	Sinus sinus;

	cout << "sin(" << x << ") = " << sinus.compute(x) << endl;
	cout << "(" << x << ")^2 = " << square.compute(x) << endl;
	return 0;
}
