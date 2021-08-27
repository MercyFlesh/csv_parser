#include <sstream>
#include <utility>
#include <regex>

#include "csv_parser.hpp"

using namespace std;


namespace csv {
size_t Hasher::operator()(const tuple<string, int>& key) const 
{
	return hash<string>()(get<0>(key)) * 1'945'637 + hash<int>()(get<1>(key));
}

XlsTable load(istream& input_stream) 
{
	XlsTable doc;	
	if (input_stream.peek() != EOF) 
	{
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
			if (line != "") 
			{
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

int XlsTable::calculate_expression(const string& expression, optional<tuple<std::string, int>> current_cell = nullopt) 
{
	regex regular("="
		"([a-zA-Z]+)([0-9]+)"
		"([+\\-*/])"
		"([a-zA-Z]+)([0-9]+)");
	smatch matcher;

	if (regex_match(expression, matcher, regular)) 
	{
		tuple<std::string, int> a_cell_key = make_tuple(matcher[1].str(), atoi(matcher[2].str().c_str()));
		tuple<std::string, int> b_cell_key = make_tuple(matcher[4].str(), atoi(matcher[5].str().c_str()));
		
		string a = cells[a_cell_key];
		string b = cells[b_cell_key];
		char op = matcher[3].str()[0];

		int num_a;
		int num_b;
		int result;

		if (a[0] == '=')
			num_a = calculate_expression(a, a_cell_key);
		else
			num_a = atoi(a.c_str());
		
		if (b[0] == '=')
			num_b = calculate_expression(b, b_cell_key);
		else
			num_b = atoi(b.c_str());

		switch (op)
		{
		case '+':
			result = num_a + num_b;
			break;
		case '-':
			result = num_a - num_b;
			break;
		case '*':
			return num_a * num_b;
			break;
		case '/':
			return num_a / num_b;
			break;
		default:
			throw runtime_error("incorrect expression: " + expression);
		}

		if (current_cell.has_value())
			cells[*current_cell] = to_string(result);

		return result;
	}

	throw runtime_error("incorrect expression: " + expression);
}

void XlsTable::calculate_all_expressions() 
{
	for (auto& [k, v] : cells) 
	{
		if (v[0] == '=') {
			v = to_string(calculate_expression(v));
		}
	}
}

ostream& operator<< (ostream& os, const csv::XlsTable& table)
{
	for (const string& col : table.col_names) 
	{
		os << "," << col;
	}

	for (int row_num : table.row_names) 
	{
		os << "\n" << row_num;
		for (const string& col : table.col_names) {
			os << "," << table.cells.at(make_tuple(col, row_num));
		}
	}

	return os;
}

vector<string> XlsTable::get_row(int row_num) const
{
	vector<string> row_cells;
	row_cells.reserve(col_names.size());
	for (const string& col : col_names) 
	{
		row_cells.emplace_back(cells.at(make_tuple(col, row_num)));
	}

	return row_cells;
}

string& XlsTable::operator()(const string& col_name, int row_num) 
{
	return cells[make_tuple(col_name, row_num)];
}
}
