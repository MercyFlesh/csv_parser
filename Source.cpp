#include <iostream>
#include <fstream>

#include "csv_parser.hpp"

using namespace std;


int main(int argc, char* argv[]) 
{
	if (argc < 2)
		return 1;

	ifstream f;
	f.exceptions(ifstream::failbit|ifstream::badbit);

	try 
	{
		f.open(argv[1]);
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
