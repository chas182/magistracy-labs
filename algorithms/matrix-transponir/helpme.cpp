#include "HelpMe.h"
using namespace std;

void fill_input()
{
	// 3 4 1 2 3 4 5 6 7 8 9 0 0 0
	// 3 4 
	// 1 2 3 4
	// 5 6 7 8
	// 9 0 0 0
	char matr[3][4] =
	{
		{ 1, 2, 3, 4 },
		{ 5, 6, 7, 8 },
		{ 9, 0, 0, 0 }
	};
	ofstream output("input.bin", ios::binary);

	int n = 3, m = 4;
	output.write((char*)&n, sizeof(int));
	output.write((char*)&m, sizeof(int));

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			output.write((char*)&matr[i][j], sizeof(char));
		}
	}

	output.close();
}
