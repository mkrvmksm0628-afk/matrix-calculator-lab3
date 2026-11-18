#include "matrix.h"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

class MatrixTest : public ::testing::Test {
protected:
    Matrix a{}, b{}, result{};
    void TearDown() override {
        free_matrix(result);
        free_matrix(b);
        free_matrix(a);
    }
};

TEST_F(MatrixTest, CreateZeros) {
    a = create_matrix(3, 4);
    ASSERT_EQ(a.rows, 3);
    ASSERT_EQ(a.cols, 4);
    for (int i = 0; i < a.rows; ++i)
        for (int j = 0; j < a.cols; ++j)
            EXPECT_DOUBLE_EQ(a.data[i][j], 0);
}

TEST_F(MatrixTest, FromArrayCopiesRows) {
    double values[]{1, 2, 3, 4, 5, 6};
    a = matrix_from_array(values, 2, 3);
    values[0] = 99;
    EXPECT_DOUBLE_EQ(a.data[0][0], 1);
    EXPECT_DOUBLE_EQ(a.data[1][0], 4);
    EXPECT_DOUBLE_EQ(a.data[1][2], 6);
}

TEST_F(MatrixTest, Add) {
    double x[]{1, 2, 3, 4}, y[]{5, 6, 7, 8};
    a = matrix_from_array(x, 2, 2);
    b = matrix_from_array(y, 2, 2);
    result = matrix_add(a, b);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            EXPECT_DOUBLE_EQ(result.data[i][j], x[2*i+j] + y[2*i+j]);
    result.data[0][0] = 100;
    EXPECT_DOUBLE_EQ(a.data[0][0], 1);
}

TEST_F(MatrixTest, RectangularProduct) {
    double x[]{1, 2, 3, 4, 5, 6}, y[]{7, 8, 9, 10, 11, 12};
    a = matrix_from_array(x, 2, 3);
    b = matrix_from_array(y, 3, 2);
    result = matrix_multiply(a, b);
    ASSERT_EQ(result.rows, 2);
    ASSERT_EQ(result.cols, 2);
    EXPECT_DOUBLE_EQ(result.data[0][0], 58);
    EXPECT_DOUBLE_EQ(result.data[0][1], 64);
    EXPECT_DOUBLE_EQ(result.data[1][0], 139);
    EXPECT_DOUBLE_EQ(result.data[1][1], 154);
}

TEST_F(MatrixTest, DotProduct) {
    double x[]{1, 2, 3}, y[]{4, 5, 6};
    a = matrix_from_array(x, 1, 3);
    b = matrix_from_array(y, 3, 1);
    result = matrix_multiply(a, b);
    ASSERT_EQ(result.rows, 1);
    ASSERT_EQ(result.cols, 1);
    EXPECT_DOUBLE_EQ(result.data[0][0], 32);
}

TEST_F(MatrixTest, TraceUsesOnlyDiagonal) {
    double x[]{-2.5, 90, 70, 40, 1.25, 80, 60, 30, 5};
    a = matrix_from_array(x, 3, 3);
    EXPECT_DOUBLE_EQ(matrix_trace(a), 3.75);
}

TEST_F(MatrixTest, TraceSingleElement) {
    double x[]{-4};
    a = matrix_from_array(x, 1, 1);
    EXPECT_DOUBLE_EQ(matrix_trace(a), -4);
}

TEST_F(MatrixTest, TraceRejectsRectangle) {
    a = create_matrix(2, 3);
    EXPECT_THROW(matrix_trace(a), std::invalid_argument);
}

TEST_F(MatrixTest, RejectInvalidSizes) {
    EXPECT_THROW(create_matrix(0, 3), std::invalid_argument);
    EXPECT_THROW(create_matrix(3, -1), std::invalid_argument);
}

TEST_F(MatrixTest, RejectNullArray) {
    EXPECT_THROW(matrix_from_array(nullptr, 2, 3), std::invalid_argument);
}

TEST_F(MatrixTest, RejectIncompatibleShapes) {
    a = create_matrix(2, 3);
    b = create_matrix(2, 2);
    EXPECT_THROW(matrix_add(a, b), std::invalid_argument);
    EXPECT_THROW(matrix_multiply(a, b), std::invalid_argument);
}

TEST_F(MatrixTest, RejectMissingStorage) {
    double* missing[]{nullptr};
    Matrix incomplete{missing, 1, 1};
    EXPECT_THROW(matrix_trace(incomplete), std::invalid_argument);
    EXPECT_THROW(matrix_add(incomplete, incomplete), std::invalid_argument);
    EXPECT_THROW(matrix_multiply(a, b), std::invalid_argument);
}

TEST_F(MatrixTest, FreeEmptyTwice) {
    EXPECT_NO_THROW(free_matrix(a));
    EXPECT_NO_THROW(free_matrix(a));
}

TEST_F(MatrixTest, PrintFormatting) {
    double x[]{1.5, -2};
    a = matrix_from_array(x, 1, 2);
    std::ostringstream output;
    auto* previous = std::cout.rdbuf(output.rdbuf());
    print_matrix(a);
    print_matrix(b);
    std::cout.rdbuf(previous);
    EXPECT_EQ(output.str(), "[    1.50   -2.00]\n[Empty matrix]\n");
}

TEST_F(MatrixTest, PrintLargeMatrix) {
    a = create_matrix(11, 12);
    a.data[5][5] = 123.25;
    a.data[10][11] = 456.50;
    std::ostringstream output;
    auto* previous = std::cout.rdbuf(output.rdbuf());
    print_matrix(a);
    std::cout.rdbuf(previous);
    EXPECT_NE(output.str().find("[...]"), std::string::npos);
    EXPECT_NE(output.str().find("Matrix 11 x 12"), std::string::npos);
    EXPECT_EQ(output.str().find("123.25"), std::string::npos);
    EXPECT_NE(output.str().find("456.50"), std::string::npos);
}
