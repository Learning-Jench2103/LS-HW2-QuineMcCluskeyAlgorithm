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

	static int variable_amount;

public:
	Implicant(int decimal, bool care_implicant = true);
	Implicant(Implicant& a, Implicant& b);	// pass two implicants that different at the same bit and construct a new implicant
	Implicant(const Implicant& a);
	bool oneDiffer(const Implicant& a) const;
	set<int> getDecimal() const;
	string getBooleanEquation() const;
	int care() const;
	bool contain(Implicant& a) const;	// if the implicant contains a ?
	bool isCareTerm() const;
	bool isMerged() const;
	void setRepeated(void);
	bool isRepeated() const;
	bool operator==(const Implicant& a) const;
	int count_1() const;	// count how many bit's value is 1

	static void setVariableAmount(int variable_amount);
	static int getVariableAmount();

};

#endif
