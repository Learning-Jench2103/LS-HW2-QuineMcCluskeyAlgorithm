#ifndef IMPLICANT_H
#define IMPLICANT_H

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

using namespace std;

class Implicant {
	friend ostream& operator<<(ostream&, Implicant&);
private:
	vector<int> boolean;
	set<int> decimalNum;
	bool care_implicant;
	bool merged = false;
	bool repeated = false;
	vector<int>dont_care_positoin;

	void findDontCare();

	void computeDecimal(int taken, int result = 0, int now = 0, int round = 1, int care_sigma = -1);

	static int variable_amount;
	static set<Implicant*> object_list;

public:
	Implicant(int decimal, bool care_implicant = true);
	Implicant(const string term, bool care_implicant);
	Implicant(Implicant& a, Implicant& b);
	~Implicant();
	bool oneDiffer(const Implicant& a) const;
	set<int> getDecimal() const;
	int care() const;
	bool contain(Implicant& a) const;
	bool isRepeated() const;
	bool operator==(const Implicant& a) const;

	static bool setVariableAmount(int variable_amount);
	static int getVariableAmount();

};

#endif
