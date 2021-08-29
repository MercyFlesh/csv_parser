#include <iostream>
#include <fstream>

#include "csv_parser.hpp"
#include "cpptest.hpp"

using namespace std;


void test_load() {
	{
		ifstream f("test_csv_cases/correct.csv");
		ASSERT_NOTHING_THROWN(csv::load, f);
		f.close();
	}
	{
		ifstream f("test_csv_cases/correct.csv");
		csv::XlsTable doc = csv::load(f);
		f.close();

		f.open("test_csv_cases/correct.csv");
		stringstream correct_stream;
		correct_stream << f.rdbuf();
		f.close();

		stringstream example_stream;
		example_stream << doc;

		ASSERT_EQUAL(example_stream.str(), correct_stream.str());
	}
}

void test_empty_file() {
	ifstream f("test_csv_cases/empty.csv");
	csv::XlsTable doc = csv::load(f);
	f.close();

	stringstream ss; 
	ss << doc;
	bool flag = ss.str().empty();
	ASSERT(flag);
}

void test_bad_first_cell() {
	ifstream f("test_csv_cases/bad_first_cell.csv");
	ASSERT_THROW(csv::load, runtime_error, f);
	ASSERT_THROW_MESSAGE(csv::load, runtime_error, "first empty field is missing", f);
	f.close();
}

void test_bad_count_cells() {
	ifstream f("test_csv_cases/bad_csv_format.csv");
	ASSERT_THROW_MESSAGE(csv::load, runtime_error, "incorrect csv format, count values in rows not equal", f);
	f.close();
}

void test_bad_row_numbers() {
	{
		ifstream f("test_csv_cases/bad_row_number_2_5.csv");
		ASSERT_THROW_MESSAGE(csv::load, runtime_error, "incorrect row numebr 2.5", f);
		f.close();
	}
	{
		ifstream f("test_csv_cases/bad_row_number_1ABC.csv");
		ASSERT_THROW_MESSAGE(csv::load, runtime_error, "incorrect row numebr 1ABC", f);
		f.close();
	}	
}

void test_bad_cell_format() {
	{
		ifstream f("test_csv_cases/bad_cell_1.csv");
		ASSERT_THROW_MESSAGE(csv::load, runtime_error, "incorrect cell value 1.5", f);
		f.close();
	}
	{
		ifstream f("test_csv_cases/bad_cell_2.csv");
		ASSERT_THROW_MESSAGE(csv::load, runtime_error, "incorrect cell value =abc", f);
		f.close();
	}
}

void test_bad_expression() {
	string bad_expr_1 = "=A1%B2";
	string bad_expr_2 = "A1*B2";
	ifstream f("test_csv_cases/correct.csv");
	csv::XlsTable doc = csv::load(f);
	f.close();

	ASSERT_THROW_MESSAGE(doc.calculate_expression, runtime_error, "incorrect expression =A1%B2", bad_expr_1);
	ASSERT_THROW_MESSAGE(doc.calculate_expression, runtime_error, "incorrect expression A1*B2", bad_expr_2);
}

void test_bad_address_in_expression() {
	string bad_expr_1 = "=A3+B2";
	string bad_expr_2 = "=A1*C2";
	ifstream f("test_csv_cases/correct.csv");
	csv::XlsTable doc = csv::load(f);
	f.close();

	ASSERT_THROW(doc.calculate_expression, out_of_range, bad_expr_1);
	ASSERT_THROW(doc.calculate_expression, out_of_range, bad_expr_2);
}

void test_devision_by_zero() {
	string expr = "=A1/B1";
	ifstream f("test_csv_cases/correct.csv");
	csv::XlsTable doc = csv::load(f);
	f.close();

	ASSERT_THROW(doc.calculate_expression, runtime_error, expr);
	ASSERT_THROW_MESSAGE(doc.calculate_expression, runtime_error, "devision by zero =A1/B1", expr);
}

void test_calculate() {
	ifstream f("test_csv_cases/correct.csv");
	csv::XlsTable doc = csv::load(f);
	f.close();

	int add = doc.calculate_expression("=A1+B2");
	int sub = doc.calculate_expression("=A2-A1");

	ASSERT_EQUAL(add, 5);
	ASSERT_EQUAL(sub, 1);
}

void test_recursion_calculate() {
	ifstream f("test_csv_cases/correct_2.csv");
	csv::XlsTable doc = csv::load(f);
	f.close();

	int a = doc.calculate_expression("=B2+B30");

	ASSERT_EQUAL(a, 29);
}

void test_indexing() {
	ifstream f("test_csv_cases/correct.csv");
	csv::XlsTable doc = csv::load(f);
	f.close();

	ASSERT_EQUAL(doc("A", 1), "1");
	ASSERT_EQUAL(doc("B", 2), "4");
}

void test_correct_example() {
	string example = "test_csv_cases/example.csv";
	string correct_example = "test_csv_cases/example_calculate.csv";
	{
		ifstream f(example);
		ASSERT_NOTHING_THROWN(csv::load, f);
		f.close();
	}
	{
		ifstream f(example);
		csv::XlsTable doc = csv::load(f);
		f.close();

		ASSERT_EQUAL(doc("A", 1), "1");
		doc.calculate_all_expressions();

		ifstream correct_f(correct_example);
		stringstream correct_stream;
		correct_stream << correct_f.rdbuf();
		correct_f.close();
		
		stringstream example_stream;
		example_stream << doc;

		ASSERT_EQUAL(example_stream.str(), correct_stream.str());
	}
}

int main() {
	Test test;
	RUN_TEST(test, test_load);
	RUN_TEST(test, test_empty_file);
	RUN_TEST(test, test_bad_first_cell);
	RUN_TEST(test, test_bad_count_cells);
	RUN_TEST(test, test_bad_row_numbers);
	RUN_TEST(test, test_bad_row_numbers);
	RUN_TEST(test, test_bad_cell_format);
	RUN_TEST(test, test_bad_expression);
	RUN_TEST(test, test_bad_address_in_expression);
	RUN_TEST(test, test_devision_by_zero);
	
	RUN_TEST(test, test_calculate);
	RUN_TEST(test, test_recursion_calculate);
	RUN_TEST(test, test_indexing);
	RUN_TEST(test, test_correct_example);

	return 0;
}
