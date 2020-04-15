#include "Implicant.h"
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

void Implicant::computeDecimal()
{
	int careNum = 0;
	vector<int> dontCare;

	switch (variables) {
	case 4:
		for (int i = 0; i < 4; i++) {
			if (boolean[i] == -1) {
				dontCare.push_back(i);
				continue;
			}
			if (boolean[i] == 1) {
				careNum += pow(2, 3 - i);
			}
		}

		decimalNum.insert(careNum);

		if (dontCare.size() == 1) {
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)));
		}
		else if (dontCare.size() == 2) {
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(1)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)) + pow(2, 3 - dontCare.at(1)));
		}
		else if (dontCare.size() == 3) {
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(1)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(2)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)) + pow(2, 3 - dontCare.at(1)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)) + pow(2, 3 - dontCare.at(2)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(1)) + pow(2, 3 - dontCare.at(2)));
			decimalNum.insert(careNum + pow(2, 3 - dontCare.at(0)) + pow(2, 3 - dontCare.at(1)) + pow(2, 3 - dontCare.at(2)));
		}

		break;

	case 3:
		for (int i = 0; i < 3; i++) {
			if (boolean[i] == -1) {
				dontCare.push_back(i);
				continue;
			}
			if (boolean[i] == 1) {
				careNum += pow(2, 2 - i);
			}
		}

		decimalNum.insert(careNum);

		if (dontCare.size() == 1) {
			decimalNum.insert(careNum + pow(2, 2 - dontCare.at(0)));
		}
		else if (dontCare.size() == 2) {
			decimalNum.insert(careNum + pow(2, 2 - dontCare.at(0)));
			decimalNum.insert(careNum + pow(2, 2 - dontCare.at(1)));
			decimalNum.insert(careNum + pow(2, 2 - dontCare.at(0)) + pow(2, 2 - dontCare.at(1)));
		}

		break;

	case 2:
		for (int i = 0; i < 2; i++) {
			if (boolean[i] == -1) {
				dontCare.push_back(i);
				continue;
			}
			if (boolean[i] == 1) {
				careNum += pow(2, 1 - i);
			}
		}

		decimalNum.insert(careNum);

		if (dontCare.size() == 1) {
			decimalNum.insert(careNum + pow(2, 1 - dontCare.at(0)));
		}

		break;
	}
}

Implicant::Implicant(int value, int v):variables(v)
{
	switch (variables) {
	case 4:
		if (value >= 8) {
			boolean[0] = 1;
			value -= 8;
		}
		else {
			boolean[0] = 0;
		}
		if (value >= 4) {
			boolean[1] = 1;
			value -= 4;
		}
		else {
			boolean[1] = 0;
		}
		if (value >= 2) {
			boolean[2] = 1;
			value -= 2;
		}
		else {
			boolean[2] = 0;
		}
		if (value == 1) {
			boolean[3] = 1;
		}
		else {
			boolean[3] = 0;
		}

		break;
	case 3:
		if (value >= 4) {
			boolean[0] = 1;
			value -= 4;
		}
		else {
			boolean[0] = 0;
		}
		if (value >= 2) {
			boolean[1] = 1;
			value -= 2;
		}
		else {
			boolean[1] = 0;
		}
		if (value == 1) {
			boolean[2] = 1;
		}
		else {
			boolean[2] = 0;
		}
		boolean[3] = -1;

		break;
	case 2:
		if (value >= 2) {
			boolean[0] = 1;
			value -= 2;
		}
		else {
			boolean[0] = 0;
		}
		if (value == 1) {
			boolean[1] = 1;
		}
		else {
			boolean[1] = 0;
		}
		boolean[2] = -1;
		boolean[3] = -1;

		break;
	}

	computeDecimal();
}

Implicant::Implicant(string input, int v)
	:variables(v)
{
	const int length = input.length();
	int char_now;

	// a //
	char_now = input.find('a', 0);
	if (char_now != -1) {
		if (char_now + 1 < length) {
			if (input.at(char_now + 1) == '\'') {
				boolean[0] = 0;
			}
			else {
				boolean[0] = 1;
			}
		}
		else {
			boolean[0] = 1;
		}
	}
	else {
		boolean[0] = -1;
	}

	// b //
	char_now = input.find('b', 0);
	if (char_now != -1) {
		if (char_now + 1 < length) {
			if (input.at(char_now + 1) == '\'') {
				boolean[1] = 0;
			}
			else {
				boolean[1] = 1;
			}
		}
		else {
			boolean[1] = 1;
		}
	}
	else {
		boolean[1] = -1;
	}

	// c //
	char_now = input.find('c', 0);
	if (char_now != -1) {
		if (char_now + 1 < length) {
			if (input.at(char_now + 1) == '\'') {
				boolean[2] = 0;
			}
			else {
				boolean[2] = 1;
			}
		}
		else {
			boolean[2] = 1;
		}
	}
	else {
		boolean[2] = -1;
	}

	// d //
	char_now = input.find('d', 0);
	if (char_now != -1) {
		if (char_now + 1 < length) {
			if (input.at(char_now + 1) == '\'') {
				boolean[3] = 0;
			}
			else {
				boolean[3] = 1;
			}
		}
		else {
			boolean[3] = 1;
		}
	}
	else {
		boolean[3] = -1;
	}

	computeDecimal();
}

Implicant::Implicant(Implicant& a, Implicant& b, int v)
	:variables(v)
{
	vector<int> diff;
	for (int i = 0; i < 4; i++) {
		if (a.boolean[i] != b.boolean[i]) {
			diff.push_back(i);
		}
	}

	// for debug //
	if (diff.size() != 1) {
		cout << "Implicant(Implicant&, Implicant&) ERROR!!!" << endl;
	}

	for (int i = 0; i < 4; i++) {
		if (i == diff.at(0)) {
			boolean[i] = -1;
			continue;
		}
		boolean[i] = a.boolean[i];
	}

	computeDecimal();
}



bool Implicant::oneDifferent(const Implicant& a)
{
	int count = 0;
	for (int i = 0; i < 4; i++) {
		if (boolean[i] != a.boolean[i]) {
			count++;
		}
	}

	if (count == 1) {
		return true;
	}
	else {
		return false;
	}
}

set<int> Implicant::getDecimal() const
{
	return decimalNum;
}

int Implicant::care() const
{
	int count = 0;
	for (int i = 0; i < 4; i++) {
		if (boolean[i] == -1) {
			count++;
		}
	}
	return 4 - count;
}

bool Implicant::contain(Implicant& a)
{
	if (care() > a.care()) {
		return false;
	}
	for (set<int>::iterator it = a.decimalNum.begin(); it != a.decimalNum.end(); it++) {
		if (decimalNum.find(*it) == decimalNum.end()) {
			return false;
		}
	}
	return true;
}

bool Implicant::operator==(Implicant& a)
{
	for (int i = 0; i < 4; i++) {
		if (boolean[i] != a.boolean[i]) {
			return false;
		}
	}
	if (variables != a.variables) {
		return false;
	}
	if (decimalNum != a.decimalNum) {
		return false;
	}
	return true;

}

int Implicant::getVariables()
{
	return variables;
}

/*
Implicant& Implicant::operator=(Implicant& a)
{
	for (int i = 0; i < 4; i++) {
		boolean[i] = a.boolean[i];
	}
	variables = a.variables;
	decimalNum = a.decimalNum;
	return *this;
}
*/

ostream& operator<<(ostream& output, Implicant& a)
{
	for (int i = 0; i < 4; i++) {
		switch (a.boolean[i]) {
		case 1:
			output << (char)('a' + i);
			break;
		case 0:
			output << (char)('a' + i) << '\'';
			break;
		}
	}

	return output;
}
