#include <gtest/gtest.h>
#include "../src/matrix.h"
#include <vector>

using namespace numcpp;

// 测试1：矩阵构造与基础属性
TEST(MatrixTest, BasicConstructor) {
    // 从维度+数据构造
    Matrix<int> mat({2, 3}, {1, 2, 3, 4, 5, 6});
    EXPECT_EQ(mat.ndim(), 2);          // 维度数：2
    EXPECT_EQ(mat.shape()[0], 2);      // 行数：2
    EXPECT_EQ(mat.shape()[1], 3);      // 列数：3
    EXPECT_EQ(mat.size(), 6);          // 总元素数：6
    EXPECT_EQ(mat(0, 2), 3);           // 访问(0行,2列)元素
    EXPECT_EQ(mat(1, 1), 5);           // 访问(1行,1列)元素

    // 空矩阵构造（异常测试）
    Matrix<double> empty_mat({0});
    EXPECT_THROW(empty_mat.mean(), std::runtime_error); // 空矩阵无平均值
    EXPECT_THROW(empty_mat.max(), std::runtime_error);  // 空矩阵无最大值
}

// 测试2：元素级运算（矩阵-矩阵）
TEST(MatrixTest, ElementWiseOperations) {
    Matrix<double> mat1({2, 2}, {1, 2, 3, 4});
    Matrix<double> mat2({2, 2}, {5, 6, 7, 8});

    // 加法
    auto sum = mat1 + mat2;
    EXPECT_EQ(sum(0,0), 6);
    EXPECT_EQ(sum(1,1), 12);

    // 减法
    auto diff = mat2 - mat1;
    EXPECT_EQ(diff(0,0), 4);
    EXPECT_EQ(diff(1,1), 4);

    // 元素级乘法
    auto elem_mul = mat1 * mat2;
    EXPECT_EQ(elem_mul(0,0), 5);
    EXPECT_EQ(elem_mul(1,1), 32);

    // 元素级除法
    auto elem_div = mat2 / mat1;
    EXPECT_EQ(elem_div(0,0), 5);
    EXPECT_EQ(elem_div(1,1), 2);

    // 维度不匹配（异常测试）
    Matrix<double> mat3({2, 3});
    EXPECT_THROW(mat1 + mat3, std::invalid_argument);
}

// 测试3：标量运算（矩阵-标量）
TEST(MatrixTest, ScalarOperations) {
    Matrix<double> mat({2, 2}, {1, 2, 3, 4});
    double scalar = 2.0;

    // 标量加法
    auto add_scalar = mat + scalar;
    EXPECT_EQ(add_scalar(0,0), 3);
    EXPECT_EQ(add_scalar(1,1), 6);

    // 标量乘法
    auto mul_scalar = mat * scalar;
    EXPECT_EQ(mul_scalar(0,0), 2);
    EXPECT_EQ(mul_scalar(1,1), 8);

    // 标量除法（除数为0，异常测试）
    EXPECT_THROW(mat / 0.0, std::domain_error);
}

// 测试4：矩阵乘法（线性代数）
TEST(MatrixTest, MatrixMultiplication) {
    Matrix<int> mat1({2, 3}, {1, 2, 3, 4, 5, 6}); // 2×3
    Matrix<int> mat2({3, 2}, {7, 8, 9, 10, 11, 12}); // 3×2

    auto product = mat1.matmul(mat2); // 2×3 × 3×2 = 2×2
    EXPECT_EQ(product.shape()[0], 2);
    EXPECT_EQ(product.shape()[1], 2);
    EXPECT_EQ(product(0,0), 58);  // 1×7 + 2×9 + 3×11 = 58
    EXPECT_EQ(product(1,1), 154); // 4×8 + 5×10 + 6×12 = 154

    // 非二维矩阵（异常测试）
    Matrix<int> mat3d({2,2,2});
    EXPECT_THROW(mat1.matmul(mat3d), std::invalid_argument);

    // 维度不匹配（异常测试）
    Matrix<int> mat3({2,2});
    EXPECT_THROW(mat1.matmul(mat3), std::invalid_argument);
}

// 测试5：转置与重塑
TEST(MatrixTest, TransposeAndReshape) {
    Matrix<double> mat({2, 3}, {1,2,3,4,5,6});

    // 转置（2×3 → 3×2）
    auto transposed = mat.transpose();
    EXPECT_EQ(transposed.shape()[0], 3);
    EXPECT_EQ(transposed.shape()[1], 2);
    EXPECT_EQ(transposed(0,1), 4); // 原(1,0)元素
    EXPECT_EQ(transposed(2,0), 3); // 原(0,2)元素

    // 重塑（2×3 → 3×2，元素顺序不变）
    auto reshaped = mat.reshape({3,2});
    EXPECT_EQ(reshaped(0,1), 2);  // 原(0,1)元素
    EXPECT_EQ(reshaped(2,0), 5);  // 原(1,1)元素

    // 重塑元素数不匹配（异常测试）
    EXPECT_THROW(mat.reshape({4,2}), std::invalid_argument);
}

// 测试6：1D卷积
TEST(MatrixTest, Conv1D) {
    Matrix<double> vec({5}, {1.0, 2.0, 3.0, 4.0, 5.0});
    Matrix<double> kernel({2}, {0.5, 0.5});

    auto conv_res = vec.conv1d(kernel);
    EXPECT_EQ(conv_res.shape()[0], 4); // 卷积后长度：5-2+1=4
    EXPECT_EQ(conv_res(0), 1.5);       // (1+2)×0.5
    EXPECT_EQ(conv_res(3), 4.5);       // (4+5)×0.5

    // 非一维矩阵（异常测试）
    Matrix<double> mat2d({2,2});
    EXPECT_THROW(mat2d.conv1d(kernel), std::invalid_argument);
}

// 测试7：统计功能
TEST(MatrixTest, Statistics) {
    Matrix<int> mat({3,3}, {1,2,3,4,5,6,7,8,9});
    EXPECT_EQ(mat.sum(), 45);   // 1+2+...+9=45
    EXPECT_EQ(mat.mean(), 5);   // 45/9=5
    EXPECT_EQ(mat.max(), 9);    // 最大值
    EXPECT_EQ(mat.min(), 1);    // 最小值
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}