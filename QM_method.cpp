#include "QM_method.h"
#include <iomanip>

void QM_method::simplify(vector<Implicant> last, int column)
{
	fstream file("output.txt", ios::app);

	vector<Implicant> current;
	vector<Implicant>* group = new vector<Implicant>[Implicant::getVariableAmount() + 1];
	for (int i = 0; i < last.size(); i++) {
		group[last.at(i).count_1()].push_back(last.at(i));
	}
	for (int count_1 = 0; count_1 < Implicant::getVariableAmount(); count_1++) {
		if (group[count_1].size() == 0 || group[count_1 + 1].size() == 0) {
			continue;
		}

		for (int index_1 = 0; index_1 < group[count_1].size(); index_1++) {
			for (int index_2 = 0; index_2 < group[count_1 + 1].size(); index_2++) {
				if (group[count_1].at(index_1).oneDiffer(group[count_1 + 1].at(index_2))) {
					Implicant temp(group[count_1].at(index_1), group[count_1 + 1].at(index_2));
					for (int i = 0; i < current.size(); i++) {
						if (current.at(i) == temp) {
							temp.setRepeated();
							break;
						}
					}
					current.push_back(temp);
				}
			}
		}
	}

	for (int i = 0; i <= Implicant::getVariableAmount(); i++) {
		for (int j = 0; j < group[i].size(); j++) {
			if (!group[i].at(j).isMerged() && group[i].at(j).isCareTerm() && !group[i].at(j).isRepeated()) {
				minimumSOP.push_back(group[i].at(j));
			}
		}
	}

	// write into file //
	file << "Column " << column << endl;
	bool first = true;

	for (int i = 0; i <= Implicant::getVariableAmount(); i++) {
		if (group[i].size() == 0) {
			continue;
		}
		if (first) {
			file << "====================" << endl;
			first = false;
		}
		else {
			file << "--------------------" << endl;
		}
		for (int j = 0; j < group[i].size(); j++) {
			file << group[i].at(j) << endl;
		}
	}
	file << endl;
	file << "====================" << endl;

	delete[] group;

	if (current.size() != 0) {
		simplify(current, column + 1);
	}
	else {
		printResult();
	}

}

vector<Implicant> QM_method::petrickMethod(vector<Implicant> chosen, map<int, bool> care_chosen, int index)
{
	// this combination can't cover all care-number //
	if (index == minimumSOP.size()) {
		chosen.clear();
		return chosen;
	}

	// this term can't cover any care-number which is uncovered //
	set<int> curr_set = minimumSOP.at(index).getDecimal();
	bool coverd = false;
	for (set<int>::iterator it = curr_set.begin(); it != curr_set.end(); it++) {
		if (!care_chosen[*it]) {
			coverd = true;
			care_chosen[*it] = true;
		}
	}
	if (!coverd) {
		chosen.clear();
		return chosen;
	}

	chosen.push_back(minimumSOP.at(index));

	// check if all care-number are coverd with this term //
	bool uncovered = false;
	for (map<int, bool>::iterator it = care_chosen.begin(); it != care_chosen.end(); it++) {
		if (!it->second) {
			uncovered = true;
			break;
		}
	}
	if (!uncovered) {
		return chosen;
	}
	if (index == minimumSOP.size() - 1 && uncovered) {
		chosen.clear();
		return chosen;
	}

	vector<Implicant> shortest;
	vector<Implicant> reply;

	for (int i = index + 1; i < minimumSOP.size(); i++) {
		reply = petrickMethod(chosen, care_chosen, i);
		if (reply.size() != 0 && shortest.size() == 0) {
			shortest = reply;
		}
		else if (reply.size() != 0 && reply.size() < shortest.size()) {
			shortest = reply;
		}
	}

	if (shortest.size() != 0) {
		return shortest;
	}
	else {
		chosen.clear();
		return chosen;
	}

}

void QM_method::printResult()
{
	fstream file("output.txt", ios::app);

	file << "Result" << endl;
	for (int i = 0; i < Implicant::getVariableAmount() * 2 + 3 + care_number.size() * 6 + 6; i++) {
		file << "=";
	}
	file << endl;

	for (int i = 0; i < Implicant::getVariableAmount() * 2 + 2; i++) {
		file << " ";
	}
	file << "|";
	file << setw(6) << care_number.at(0);
	for (int i = 1; i < care_number.size(); i++) {
		file << "," << setw(5) << care_number.at(i);
	}
	file << endl;

	for (int i = 0; i < Implicant::getVariableAmount() * 2 + 2; i++) {
		file << "-";
	}
	file << "|";
	for (int i = 0; i < care_number.size() * 6 + 6; i++) {
		file << "-";
	}
	file << endl;

	for (int i = 0; i < minimumSOP.size(); i++) {
		string booleanEq = minimumSOP.at(i).getBooleanEquation();
		file << booleanEq;
		for (int j = 0; j < Implicant::getVariableAmount() * 2 + 2 - booleanEq.length(); j++) {
			file << " ";
		}
		file << "|";
		set<int> decimal = minimumSOP.at(i).getDecimal();
		for (int j = 0; j < care_number.size(); j++) {
			if (decimal.find(care_number.at(j)) != decimal.end()) {
				file << "     x";
			}
			else {
				file << "      ";
			}
		}

		file << endl;
	}

	vector<Implicant> miniResult;
	map<int, bool> care_chosen;
	for (int i = 0; i < care_number.size(); i++) {
		care_chosen[care_number.at(i)] = false;
	}

	for (int i = 0; i < minimumSOP.size(); i++) {
		vector<Implicant> temp;
		temp = petrickMethod(temp, care_chosen, i);
		if (miniResult.size() == 0 && temp.size() != 0) {
			miniResult = temp;
		}
		else if (temp.size() < miniResult.size() && temp.size() != 0) {
			miniResult = temp;
		}
	}

	for (int i = 0; i < miniResult.size(); i++) {
		bool again = false;
		for (int j = 0; j < minimumSOP.size(); j++) {
			if (minimumSOP.at(j) == miniResult.at(i)) {
				again = true;
			}
		}
		if (!again) {
			minimumSOP.push_back(miniResult.at(i));
		}
	}

	for (int i = 0; i < Implicant::getVariableAmount() * 2 + 2; i++) {
		file << "-";
	}
	file << "|";
	for (int i = 0; i < care_number.size() * 6 + 6; i++) {
		file << "-";
	}
	file << endl;

	file << "F(A";
	for (int i = 1; i < Implicant::getVariableAmount(); i++) {
		file << "," << (char)('A' + i);
	}
	file << ") = " << minimumSOP.at(0).getBooleanEquation();
	for (int i = 1; i < minimumSOP.size(); i++) {
		if (!minimumSOP.at(i).isRepeated()) {
			file << " + " << minimumSOP.at(i).getBooleanEquation();
		}
	}

}

QM_method::QM_method(string fileName)
{
	fstream file(fileName, ios::in);
	string input;
	getline(file, input);
	stringstream ss;
	ss << string(input.begin() + input.find('<') + 1, input.begin() + input.find('>'));
	int temp;
	ss >> temp;
	Implicant::setVariableAmount(temp);
	ss.str(""); ss.clear();

	getline(file, input);
	getline(file, input);

	while (input[0] >= '0' && input[0] <= '9') {
		ss << input;
		ss >> temp;
		ss.str(""); ss.clear();
		care_number.push_back(temp);
		origin.push_back(Implicant(temp, true));
		getline(file, input);
	}

	// input = "]"


	getline(file, input);

	if (input.length() == 1 || input.length() >= 2 && (input[1] > '9' || input[1] < '0')) {
		getline(file, input);
		while (input[0] >= '0' && input[0] <= '9') {
			ss << input;
			ss >> temp;
			ss.str(""); ss.clear();
			care_number.push_back(temp);
			origin.push_back(Implicant(temp, false));
			getline(file, input);
		}
	}
	else {
		ss << string(input.begin() + input.find('(') + 1, input.begin() + input.find(')'));
		ss >> temp;
		ss.str(""); ss.clear();
		origin.push_back(Implicant(temp, false));
	}

	file.close();

	fstream("output.txt", ios::out);
}

void QM_method::run()
{
	simplify(origin, 1);
}