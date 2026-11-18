#include "matrix.h"

#include <exception>
#include <iostream>

/** @brief Демонстрирует сложение, произведение и след на двух матрицах. */
int main() {
    double left[]{1, 2, 3, 4};
    double right[]{5, 6, 7, 8};
    Matrix a{}, b{}, sum{}, product{};
    int status = 0;
    try {
        a = matrix_from_array(left, 2, 2);
        b = matrix_from_array(right, 2, 2);
        sum = matrix_add(a, b);
        product = matrix_multiply(a, b);
        std::cout << "A:\n"; print_matrix(a);
        std::cout << "B:\n"; print_matrix(b);
        std::cout << "A + B:\n"; print_matrix(sum);
        std::cout << "A * B:\n"; print_matrix(product);
        std::cout << "trace(A) = " << matrix_trace(a) << '\n';
        std::cout << "trace(A * B) = " << matrix_trace(product) << '\n';
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        status = 1;
    }
    free_matrix(product);
    free_matrix(sum);
    free_matrix(b);
    free_matrix(a);
    return status;
}
