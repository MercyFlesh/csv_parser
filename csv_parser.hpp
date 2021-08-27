#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>


namespace csv 
{
struct Hasher 
{
	std::size_t operator() (const std::tuple<std::string, int>& key) const;
};

class Document
{
public:
	Document() = default;
	~Document() = default;

	int calculate(std::string operation);
	void example_parse();
	friend Document load(std::istream& input_stream);

private:
	std::unordered_map<std::tuple<std::string, int>, std::string, Hasher> cells;
	std::vector<std::string> col_names;
	std::vector<int> row_names;
};

Document load(std::istream& input_stream);
}
