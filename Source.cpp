#include <iostream>
#include <fstream>

#include "csv_parser.hpp"

using namespace std;


int main(int argc, char* argv[]) 
{
	string file_path = "C:\\Users\\artem\\source\\repos\\cpp\\csv_parser\\example.csv";
	ifstream f;
	f.exceptions(ifstream::failbit|ifstream::badbit);

	try 
	{
		f.open(file_path);
		csv::XlsTable doc = csv::load(f);
		
		doc.calculate_all_expressions();
		cout << doc << endl;
	} 
	catch (const ifstream::failure& ex) 
	{
		cout << ex.what() << endl;
	} 
	catch (const exception& ex) 
	{
		cout << ex.what() << endl;
	}

	f.close();
	return 0;
}
