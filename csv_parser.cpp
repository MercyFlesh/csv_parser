#include "csv_parser.hpp"

#include <sstream>
#include <utility>
#include <regex>

using namespace std;

namespace csv {
size_t Hasher::operator()(const tuple<string, int>& key) const {
	return hash<string>()(get<0>(key)) * 1'945'637 + hash<int>()(get<1>(key));
}


Document load(istream& input_stream) {
	Document doc;	
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
}