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

void QM_method::petrickMethod() {
	map<int, set<int>> number_term_list;
	for (int i = 0; i < minimumSOP.size(); i++) {
		set<int> temp = minimumSOP.at(i).getDecimal();
		for (set<int>::iterator it = temp.begin(); it != temp.end(); it++) {
			number_term_list[*it].insert(i);
		}
	}

	vector<set<int>> last, now;

	for (int i = 0; i < care_number.size(); i++) {
		last = now;
		now.clear();

		if (last.size() == 0) {
			for (set<int>::iterator it = number_term_list[care_number.at(i)].begin(); it != number_term_list[care_number.at(i)].end(); it++) {
				set<int> temp;
				temp.insert(*it);
				now.push_back(temp);
			}
			continue;
		}

		for (set<int>::iterator it = number_term_list[care_number.at(i)].begin(); it != number_term_list[care_number.at(i)].end(); it++) {
			for (int j = 0; j < last.size(); j++) {
				set<int> temp = last.at(j);
				temp.insert(*it);
				now.push_back(temp);
			}
		}
	}

	set<int> shortest = now.at(0);
	for (int i = 1; i < now.size(); i++) {
		if (now.at(i).size() < shortest.size()) {
			shortest = now.at(i);
		}
	}

	vector<Implicant> result;
	for (set<int>::iterator it = shortest.begin(); it != shortest.end(); it++) {
		result.push_back(minimumSOP.at(*it));
	}

	minimumSOP = result;
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

	petrickMethod();

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

	if (!file.eof()) {
		getline(file, input);
		if (input.length() != 0 && input.at(0) == '(') {

			if (input.length() == 1 || input.length() >= 2 && (input[1] > '9' || input[1] < '0')) {
				getline(file, input);
				while (input[0] >= '0' && input[0] <= '9') {
					ss << input;
					ss >> temp;
					ss.str(""); ss.clear();
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
		}
	}
	file.close();

	fstream("output.txt", ios::out);
}

void QM_method::run()
{
	simplify(origin, 1);
}
