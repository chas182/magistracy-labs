#include <fstream>

using namespace std;

char* transpose(const char* array, const int n, const int m);

char* read_block(ifstream &input, 
	const int block_n, const int block_m, 
	const int n, const int m, 
	const int offset_rows, const int offset_cols);

void write_block(ofstream &output, const char*buffer, 
	const int block_n, const int block_m,
	const int n, const int m,
	const int offset_rows, const int offset_cols);

int main() {
	ifstream input("input.bin", ifstream::in | ifstream::binary);
	ofstream output("output.bin", ofstream::out | ofstream::binary);

	int n = 0, m = 0;
	int block_n = 300, block_m = 300;
	int B = block_n * block_m;

	input.read((char*)&n, sizeof(n));
	input.read((char*)&m, sizeof(m));

	if (n * m < B) { block_n = n; block_m = m; } 
    else {
		if (n < block_n) { block_n = n; block_m = min(block_m, m); }
		if (m < block_m) { block_m = m; if (block_n != n) { block_n = min(block_n, n); } }
	}

	output.write((char*)&m, sizeof(m));
	output.write((char*)&n, sizeof(n));

	for (int i = 0; i < n / (double)block_n; i++) {
		int offset_rows = i * block_n;
		for (int j = 0; j < m / (double)block_m; j++) {
			int offset_cols = j * block_m;
			int adjusted_block_rows = (offset_rows + block_n) < n ? block_n : n - offset_rows;
			int adjusted_block_cols = (offset_cols + block_m) < m ? block_m : m - offset_cols;
			char* buffer = read_block(input, adjusted_block_rows, adjusted_block_cols,
				n, m, offset_rows, offset_cols);
			char* transposed = transpose(buffer, adjusted_block_rows, adjusted_block_cols);
			write_block(output, transposed, adjusted_block_cols, adjusted_block_rows, m, n, offset_cols, offset_rows);
			delete buffer;
			delete transposed;
		}
	}

	input.close();
	output.close();
	return 0;
}

char* transpose(const char* array, const int n, const int m) {
	char* result = new char[n * m];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			result[n * j + i] = array[i * m + j];
		}
	}
	return result;
}

char* read_block(ifstream &input, const int block_n, const int block_m,
	const int n, const int m, const int offset_rows, const int offset_cols) {
	char* buffer = new char[block_n * block_m];
	if (block_m == m)
		input.read(buffer, block_n * block_m);
	else {
		for (int i = 0; i < block_n; i++) {
			int seek = (offset_rows + i) * m + offset_cols + 8;
			input.seekg(seek, ios_base::beg);
			input.read(buffer + i * block_m, block_m);
		}
	}

	return buffer;
}

void write_block(ofstream &output, const char*buffer, const int block_n, const int block_m,
	const int n, const int m,
	const int offset_rows, const int offset_cols) {
	if (block_m == m)
		output.write(buffer, block_m * block_n);
	else {
		for (int i = 0; i < block_n; i++) {
			int seek = (offset_rows + i) * m + offset_cols + 8;
			output.seekp(seek, ios_base::beg);
			output.write(buffer + i * block_m, block_m);
		}
	}
}
