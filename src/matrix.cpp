#include "matrix.h"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace {
/** @brief Проверяет размеры и наличие указателя на каждую строку. */
bool valid(Matrix m) {
    return m.rows > 0 && m.cols > 0 && m.data &&
           std::all_of(m.data, m.data + m.rows,
                       [](const double* row) { return row != nullptr; });
}
}  // namespace

Matrix create_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0)
        throw std::invalid_argument("Matrix dimensions must be positive");
    Matrix m{new double*[rows]{}, rows, cols};
    try {
        for (int i = 0; i < rows; ++i)
            m.data[i] = new double[cols]{};
    } catch (...) {
        free_matrix(m);
        throw;
    }
    return m;
}

void free_matrix(Matrix m) noexcept {
    if (!m.data) return;
    for (int i = 0; i < m.rows; ++i)
        delete[] m.data[i];
    delete[] m.data;
}

Matrix matrix_from_array(double* data, int rows, int cols) {
    if (!data) throw std::invalid_argument("Null source array");
    Matrix m = create_matrix(rows, cols);
    for (int i = 0; i < rows; ++i)
        std::copy_n(data + static_cast<std::size_t>(i) * cols,
                    cols, m.data[i]);
    return m;
}

Matrix matrix_add(Matrix a, Matrix b) {
    if (!valid(a) || !valid(b) || a.rows != b.rows || a.cols != b.cols)
        throw std::invalid_argument("Addition requires equal shapes");
    Matrix sum = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; ++i)
        for (int j = 0; j < a.cols; ++j)
            sum.data[i][j] = a.data[i][j] + b.data[i][j];
    return sum;
}

Matrix matrix_multiply(Matrix a, Matrix b) {
    if (!valid(a) || !valid(b) || a.cols != b.rows)
        throw std::invalid_argument("Product requires A.cols == B.rows");
    Matrix product = create_matrix(a.rows, b.cols);
    for (int i = 0; i < a.rows; ++i)
        for (int k = 0; k < a.cols; ++k)
            for (int j = 0; j < b.cols; ++j)
                product.data[i][j] += a.data[i][k] * b.data[k][j];
    return product;
}

double matrix_trace(Matrix m) {
    if (!valid(m) || m.rows != m.cols)
        throw std::invalid_argument("Trace requires a square matrix");
    double trace = 0.0;
    for (int i = 0; i < m.rows; ++i)
        trace += m.data[i][i];
    return trace;
}

void print_matrix(Matrix m) {
    if (!valid(m)) {
        std::cout << "[Empty matrix]\n";
        return;
    }
    const auto flags = std::cout.flags();
    const auto precision = std::cout.precision();
    std::cout << std::fixed << std::setprecision(2);
    constexpr int max_rows = 10, max_cols = 8;
    for (int i = 0; i < m.rows; ++i) {
        if (i == max_rows / 2 && m.rows > max_rows) {
            std::cout << "[...]\n";
            i = m.rows - max_rows / 2;
        }
        std::cout << '[';
        for (int j = 0; j < m.cols; ++j) {
            if (j == max_cols / 2 && m.cols > max_cols) {
                std::cout << " ...";
                j = m.cols - max_cols / 2;
            }
            std::cout << std::setw(8) << m.data[i][j];
        }
        std::cout << "]\n";
    }
    if (m.rows > max_rows || m.cols > max_cols)
        std::cout << "Matrix " << m.rows << " x " << m.cols << '\n';
    std::cout.flags(flags);
    std::cout.precision(precision);
}
