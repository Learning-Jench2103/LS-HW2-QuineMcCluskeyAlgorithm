#include "Implicant.h"
#include <cmath>
using namespace std;

int Implicant::variable_amount = 10;

void Implicant::findDontCare()
{
	for (int i = 0; i < variable_amount; i++) {
		if (boolean.at(i) == -1) {	// store don't care term as -1
			dont_care_positoin.push_back(i);
		}
	}
	return;
}

Implicant::Implicant(int decimal, bool care_implicant)
{
	boolean.resize(variable_amount);
	decimalNum.insert(decimal);
	for (int i = 0; i < variable_amount; i++) {
		if (decimal >= pow(2, variable_amount - i - 1)) {
			boolean.at(i) = 1;
			decimal -= pow(2, variable_amount - i - 1);
		}
		else {
			boolean.at(i) = 0;
		}
	}

	Implicant::care_implicant = care_implicant;
	findDontCare();
}

Implicant::Implicant(Implicant& a, Implicant& b)
{
	boolean = a.boolean;
	for (int i = 0; i < a.boolean.size(); i++) {
		if (a.boolean.at(i) != b.boolean.at(i)) {
			boolean.at(i) = -1;
		}
	}

	a.merged = true; b.merged = true;
	Implicant::care_implicant = (a.care_implicant || b.care_implicant);
	findDontCare();
	decimalNum.insert(a.decimalNum.begin(), a.decimalNum.end());
	decimalNum.insert(b.decimalNum.begin(), b.decimalNum.end());
}

Implicant::Implicant(const Implicant& a)
{
	boolean = a.boolean;
	decimalNum = a.decimalNum;
	care_implicant = a.care_implicant;
	merged = a.merged;
	repeated = a.repeated;
	dont_care_positoin = a.dont_care_positoin;
}

bool Implicant::oneDiffer(const Implicant& a) const
{
	if (a.boolean.size() != boolean.size()) {
		return false;
	}
	int count = 0;
	for (int i = 0; i < boolean.size(); i++) {
		if (boolean.at(i) != a.boolean.at(i)) {
			count++;
			if (count > 1) {
				return false;
			}
		}
	}
	return count == 1;
}

set<int> Implicant::getDecimal() const
{
	return decimalNum;
}

string Implicant::getBooleanEquation() const
{
	bool output = false;
	string result;
	for (int i = 0; i < Implicant::getVariableAmount(); i++) {
		if (boolean.at(i) == 1) {
			result += (char)('A' + i);
			result += ' ';
			output = true;
		}
		else if (boolean.at(i) == 0) {
			result += (char)('A' + i);
			result += '\'';
			output = true;
		}
	}
	if (!output) {
		result += "1";
	}
	return result;
}

int Implicant::care() const
{
	int care = 0;
	for (int i = 0; i < variable_amount; i++) {
		if (boolean.at(i) != -1) {
			care++;
		}
	}
	return care;
}

bool Implicant::contain(Implicant& a) const
{
	for (set<int>::iterator it = a.decimalNum.begin(); it != a.decimalNum.end(); it++) {
		if (decimalNum.find(*it) == decimalNum.end()) {
			return false;
		}
	}
	return true;
}

bool Implicant::isCareTerm() const
{
	return care_implicant;
}

bool Implicant::isMerged() const
{
	return merged;
}

void Implicant::setRepeated(void)
{
	repeated = true;
}

bool Implicant::isRepeated() const
{
	return repeated;
}

bool Implicant::operator==(const Implicant& a) const
{
	return (boolean == a.boolean && decimalNum == a.decimalNum);
}

int Implicant::count_1() const
{
	int count = 0;
	for (int i = 0; i < boolean.size(); i++) {
		if (boolean.at(i) == 1) {
			count++;
		}
	}
	return count;
}

void Implicant::setVariableAmount(int variable_amount)
{
	Implicant::variable_amount = variable_amount;
}

int Implicant::getVariableAmount()
{
	return variable_amount;
}

ostream& operator<<(ostream& output, Implicant& implicant)
{
	if (implicant.repeated == true) {
		output << 'x';
	}
	else if (implicant.merged == true) {
		output << 'v';
	}
	else if (implicant.care_implicant == false) {
		output << 'd';
	}
	else {
		output << ' ';
	}

	output << ' ';

	for (int i = 0; i < Implicant::variable_amount; i++) {
		if (implicant.boolean.at(i) == -1) {
			output << '-';
		}
		else {
			output << implicant.boolean.at(i);
		}
	}

	output << " : ";

	int countDecimal = 0;
	for (set<int>::iterator it = implicant.decimalNum.begin(); it != implicant.decimalNum.end(); it++) {
		output << *it;
		++countDecimal;
		if (countDecimal < implicant.decimalNum.size()) {
			output << ", ";
		}
	}

	return output;
}
