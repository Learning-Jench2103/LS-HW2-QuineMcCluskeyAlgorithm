#include "Implicant.h"
#include <cmath>
using namespace std;

int Implicant::variable_amount = 10;

void Implicant::findDontCare()
{
	for (int i = 0; i < variable_amount; i++) {
		if (boolean.at(i) == -1) {
			dont_care_positoin.push_back(i);
		}
	}
	return;
}

void Implicant::computeDecimal(int taken, int result, int now, int round, int care_sigma)
{
	if (care_sigma == -1) {
		care_sigma = 0;
		for (int i = 0; i < variable_amount; i++) {
			if (boolean.at(i) == 1) {
				care_sigma += pow(2, variable_amount - 1 - i);
			}
		}
		result = care_sigma;
	}
	if (round = taken) {
		decimalNum.insert(result);
		return;
	}
	if (taken - round > dont_care_positoin.size() - 1 - now) {
		return;
	}
	else {
		result += boolean.at(dont_care_positoin.at(now));
		for (int i = now + 1; i < dont_care_positoin.size(); i++) {
			computeDecimal(taken, result, i, round + 1, care_sigma);
		}
	}
}

Implicant::Implicant(int decimal, bool care_implicant)
{
	boolean.resize(variable_amount);

	for (int i = 0; i < variable_amount; i++) {
		if (decimal > pow(2, variable_amount - i - 1)) {
			boolean.at(i) = 1;
			decimal -= pow(2, variable_amount - i - 1);
		}
		else {
			boolean.at(i) = 0;
		}
	}

	Implicant::care_implicant = care_implicant;

	findDontCare();
	for (int i = 0; i <= dont_care_positoin.size(); i++) {
		computeDecimal(i);
	}
	for (set<Implicant*>::iterator it = object_list.begin(); it != object_list.end(); it++) {
		if ((*this) == *(*it)) {
			repeated = true;
		}
	}
	object_list.insert(this);
}

Implicant::Implicant(const string term, bool care_implicant)
{
	int var = 0;
	for (int i = 0; i < term.length(); i++) {
		if (term[i] < 'a' || term[i] > 'z') {
			continue;
		}
		if ((term[i] >= 'a' && term[i] <= 'z') && term[i] - 'a' + 1 > var) {
			var = term[i] - 'a' + 1;
		}
	}
	for (int i = 0; i < var; i++) {
		int posi = term.find((char)('a' + i), 0);
		if (posi == -1) {
			boolean.at(i) = -1;
			continue;
		}
		if (posi == term.length() - 1 || (posi < term.length() - 1 && term[posi + 1] != '\'')) {
			boolean.at(i) = 1;
		}
		else {
			boolean.at(i) = 0;
		}
	}

	Implicant::care_implicant = care_implicant;
	findDontCare();
	for (int i = 0; i <= dont_care_positoin.size(); i++) {
		computeDecimal(i);
	}
	for (set<Implicant*>::iterator it = object_list.begin(); it != object_list.end(); it++) {
		if ((*this) == *(*it)) {
			repeated = true;
		}
	}
	object_list.insert(this);
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
	for (int i = 0; i <= dont_care_positoin.size(); i++) {
		computeDecimal(i);
	}
	for (set<Implicant*>::iterator it = object_list.begin(); it != object_list.end(); it++) {
		if ((*this) == *(*it)) {
			repeated = true;
		}
	}
	object_list.insert(this);
}

Implicant::~Implicant()
{
	object_list.erase(this);
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

bool Implicant::isRepeated() const
{
	return repeated;
}

bool Implicant::operator==(const Implicant& a) const
{
	return (boolean == a.boolean && decimalNum == a.decimalNum);
}

bool Implicant::setVariableAmount(int variable_amount)
{
	if (variable_amount < Implicant::variable_amount) {
		return false;
	}
	if (variable_amount == Implicant::variable_amount) {
		return true;
	}
	int old_amount = Implicant::variable_amount;
	Implicant::variable_amount = variable_amount;
	for (set<Implicant*>::iterator it = object_list.begin(); it != object_list.end(); it++) {
		for (int i = old_amount; i < variable_amount; i++) {
			(*it)->boolean.push_back(-1);
		}
		(*it)->findDontCare();
		for (int i = 0; i <= (*it)->dont_care_positoin.size(); i++) {
			(*it)->computeDecimal(i);
		}
	}

	return true;
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
