#include "csv_parser.hpp"

#include <sstream>
#include <utility>
#include <regex>

using namespace std;

namespace csv {
size_t Hasher::operator()(const tuple<string, int>& key) const {
	return hash<string>()(get<0>(key)) * 1'945'637 + hash<int>()(get<1>(key));
}


XlsTable load(istream& input_stream) {
	XlsTable doc;	
	if (input_stream.peek() != EOF) {
		string line;
		string cell;

		getline(input_stream, line);
		stringstream row_stream(move(line));
		getline(row_stream, cell, ',');
		while (getline(row_stream, cell, ','))
		{
			doc.col_names.push_back(move(cell));
		}

		while (!input_stream.eof() && input_stream.peek() != EOF) 
		{
			getline(input_stream, line);
			if (line != "") {
				row_stream = stringstream(line);
				
				getline(row_stream, cell, ',');
				int num_row = atoi(cell.c_str());
				doc.row_names.push_back(num_row);

				for (auto cols_it = doc.col_names.begin();
					getline(row_stream, cell, ',') && cols_it != doc.col_names.end();
					++cols_it) 
				{
					doc.cells[make_tuple(*cols_it, num_row)] = move(cell);
				}
			}
		}
	}

	return doc;
}


int XlsTable::calculate_expression(string expression) {
	regex regular("="
		"([a-zA-Z]+)([0-9]+)"
		"([+\\-*/])"
		"([a-zA-Z]+)([0-9]+)");
	smatch matcher;

	if (regex_match(expression, matcher, regular)) 
	{
		string a = cells[make_tuple(matcher[1].str(), atoi(matcher[2].str().c_str()))];
		string b = cells[make_tuple(matcher[4].str(), atoi(matcher[5].str().c_str()))];
		char op = matcher[3].str()[0];

		int num_a;
		int num_b;

		if (a[0] == '=')
			num_a = calculate_expression(a);
		else
			num_a = atoi(a.c_str());
		
		if (b[0] == '=')
			num_b = calculate_expression(b);
		else
			num_b = atoi(b.c_str());

		switch (op)
		{
		case '+':
			return num_a + num_b;
		case '-':
			return num_a - num_b;
		case '*':
			return num_a * num_b;
		case '/':
			return num_a / num_b;
		}
	}

	throw runtime_error("incorrect expression: " + expression);
}


void XlsTable::parse_expressions() {
	for (auto& [k, v] : cells) 
	{
		if (v[0] == '=') {
			v = to_string(calculate_expression(v));
		}
	}
}

ostream& operator<< (ostream& os, const csv::XlsTable& table)
{
	for (const string& col : table.col_names) {
		os << "," << col;
	}

	for (int row_num : table.row_names) {
		os << "\n" << row_num;
		for (string col : table.col_names) {
			os << "," << table.cells.at(make_tuple(col, row_num));
		}
	}

	return os;
}
}


