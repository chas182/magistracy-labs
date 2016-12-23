#include <cstdint>
#include <iostream>
#include <fstream>
using namespace std;


void multiply(const char* a, const char* b, char* c, int a_n, int a_m, int b_n, int b_m);

char* read_block(fstream &input, 
                 const int block_n, const int block_m,
                 const int n, const int m, 
                 const int offset_rows, const int offset_cols, 
                 const int offset);

void write_block(fstream &output, const char* buffer,
                 const int block_n, const int block_m,
                 const int n, const int m,
                 const int offset_rows, const int offset_cols, 
                 const int offset);

int main() {
    fstream input("input.bin", fstream::in | fstream::binary);
    fstream output("output.bin", fstream::out | fstream::binary);

    int B = 300;
    int size = 0;

    input.read((char *)&size, sizeof(size));
    output.write((char *)&size, sizeof(size));
    output.write((char *)&size, sizeof(size));

    if (size < B) B = size;

    for (int i = 0; i < size;  i += B) {
        int a_n = min(B, size - i);
        for (int j = 0; j < size;  j += B) {
            int b_m = min(B, size - j);
            char* c_block = new char[a_n * b_m];
            fill(c_block, c_block + a_n * b_m, 0);

            for (int k = 0; k < size;  k += B) {
                int a_m = min(B, size - k);
                int b_n = a_m;
                char* a_block = read_block(input, a_n, a_m, size, size, i, k, 8);
                char* b_block = read_block(input, b_n, b_m, size, size, k, j, 16 + size * size);
                multiply(a_block, b_block, c_block, a_n, a_m, b_n, b_m);

                delete a_block;
                delete b_block;
            }

            write_block(output, c_block, a_n, b_m, size, size, i, j, 8);
            delete c_block;
        }
    }

    input.close();
    output.close();
    return 0;
}

void multiply(const char* a, const char* b, char* c, int a_n, int a_m, int b_n, int b_m) {
    for (int i = 0; i < a_n; ++i)
        for (int j = 0; j < b_m; ++j)
            for (int k = 0; k < a_m; ++k)
                c[i * b_m + j] += a[i * a_m + k] * b[k * b_m + j];
}

char* read_block(fstream &input,
                 const int block_n, const int block_m,
                 const int n, const int m,
                 const int offset_rows, const int offset_cols,
                 const int offset) {
    char * buffer = new char [block_n * block_m];
    if (block_m == m)
        input.read(buffer, block_n * block_m);
    else {
        for (int i = 0; i < block_n; i++) {
            int seek = (offset_rows + i) * m + offset_cols + offset;
            input.seekg(seek, ios_base::beg);
            input.read(buffer + i * block_m, block_m);
        }
    }
    return buffer;
}

void write_block(fstream &output, const char* buffer,
                 const int block_n, const int block_m,
                 const int n, const int m,
                 const int offset_rows, const int offset_cols,
                 const int offset) {
    if (block_m == m)
        output.write(buffer, block_m * block_n);
    else {
        for (int i = 0; i < block_n; i++) {
            int seek = (offset_rows + i) * m + offset_cols + offset;
            output.seekp(seek, ios_base::beg);
            output.write(buffer + i * block_m, block_m);
        }
    }
}
