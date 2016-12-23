#include "HelpMe.h"
#include <iostream>
using namespace std;

void fill_input() {
	char matr[5][5] =
	{
		{ 1,2,3,4,5 },
		{ 6,7,8,9,10 },
		{ 11,12,13,14,15 },
		{ 16,17,18,19,20 },
		{ 21,22,23,24,25 },
	};

	ofstream output("input.bin", ios::binary);
    std::cout << "Hello, fill!" << std::endl;

	int n = 5, m = 5;
	output.write((char*)&n, sizeof(int));
	output.write((char*)&m, sizeof(int));

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			output.write((char*)&matr[i][j], sizeof(char));

    char matr2[5][5] =
    {
        { 1,2,3,4,5 },
        { 6,7,8,9,10 },
        { 11,12,13,14,15 },
        { 16,17,18,19,20 },
        { 21,22,23,24,25 },
    };
	output.write((char*)&n, sizeof(int));
	output.write((char*)&m, sizeof(int));

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			output.write((char*)&matr2[i][j], sizeof(char));
	output.close();
}
