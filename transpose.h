#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include <vector>

using Matrix = std::vector<std::vector<int>>;

Matrix transpose_sequential(const Matrix& A, bool verbose);
void transpose_worker(const Matrix& A, Matrix& B, int start_row, int end_row);
Matrix transpose_multithreaded(const Matrix& A, int threads, bool);
bool is_transpose(const Matrix& A, const Matrix& B);
Matrix transpose_cache_oblivious(const Matrix& A);
void transpose_recursive(const Matrix& A, Matrix& B, int x, int y, int size);

#endif
