#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include <vector>
#include <string>

using Matrix = std::vector<std::vector<int>>;

Matrix transpose_buildMatrix(int y, int x, bool verbose);
Matrix transpose_time(Matrix (*strategy)(const Matrix& A, bool verbose), const Matrix& A, bool verbose, std::string name);
bool is_transpose(const Matrix& A, const Matrix& B);

#endif
