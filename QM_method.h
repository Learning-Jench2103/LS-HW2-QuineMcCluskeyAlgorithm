#ifndef QM_METHOD_H
#define QM_METHOD_H
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include "Implicant.h"
using namespace std;

class QM_method
{
private:
	vector<Implicant> origin;
	vector<int> care_number;
	vector<Implicant> minimumSOP;

	void simplify(vector<Implicant> last, int column);
	//vector<Implicant> petrickMethod(vector<Implicant> chosen, map<int, bool> care_chosen, int index);
	void petrickMethod();

	void printResult();

public:
	QM_method(string fileName);
	void run();

};

#endif