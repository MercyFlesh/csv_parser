#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>
#include <optional>


namespace csv 
{
struct Hasher 
{
	std::size_t operator() (const std::tuple<std::string, int>& key) const;
};

class XlsTable
{
public:
	XlsTable() = default;
	~XlsTable() = default;

	int calculate_expression(const std::string& expression, std::optional<std::tuple<std::string, int>> current_cell);
	void calculate_all_expressions();

	std::vector<std::string> get_row(int row_num) const;
	std::string& operator()(const std::string& col_name, int row_num);

	friend XlsTable load(std::istream& input_stream);
	friend std::ostream& operator<<(std::ostream& os, const XlsTable& table);

private:
	std::unordered_map<std::tuple<std::string, int>, std::string, Hasher> cells;
	std::vector<std::string> col_names;
	std::vector<int> row_names;
};

XlsTable load(std::istream& input_stream);
std::ostream& operator<<(std::ostream& os, const csv::XlsTable& table);
}
