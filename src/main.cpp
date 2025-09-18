#include "matrix.h"
#include <iostream>

int main() {
    try {
        // 1. 创建矩阵并初始化
        numcpp::Matrix<double> mat1({2, 3}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
        numcpp::Matrix<double> mat2({2, 3}, {7.0, 8.0, 9.0, 10.0, 11.0, 12.0});
        mat1.print("mat1");
        mat2.print("mat2");

        // 2. 元素级运算
        auto mat_sum = mat1 + mat2;       // 矩阵加法
        auto mat_diff = mat2 - mat1;      // 矩阵减法
        auto mat_elem_mul = mat1 * mat2;  // 元素级乘法
        auto mat_elem_div = mat2 / mat1;  // 元素级除法

        mat_sum.print("mat1 + mat2");
        mat_diff.print("mat2 - mat1");
        mat_elem_mul.print("mat1 * mat2 (元素级)");
        mat_elem_div.print("mat2 / mat1 (元素级)");

        // 3. 标量运算
        auto mat_scalar_mul = mat1 * 2.5;  // 矩阵 × 标量
        auto mat_scalar_add = mat1 + 10.0; // 矩阵 + 标量
        mat_scalar_mul.print("mat1 * 2.5");
        mat_scalar_add.print("mat1 + 10.0");

        // 4. 线性代数矩阵乘法（需满足：前矩阵列数 = 后矩阵行数）
        auto mat1_trans = mat1.transpose(); // mat1 转置（2×3 → 3×2）
        auto mat_matmul = mat1.matmul(mat1_trans); // 2×3 × 3×2 = 2×2
        mat1_trans.print("mat1 转置");
        mat_matmul.print("mat1 × mat1^T (矩阵乘法)");

        // 5. 重塑与转置
        auto mat_reshaped = mat1.reshape({3, 2}); // 2×3 → 3×2
        mat_reshaped.print("mat1 重塑为 3×2");

        // 6. 1D卷积（仅支持一维矩阵）
        numcpp::Matrix<double> vec1d({5}, {1.0, 2.0, 3.0, 4.0, 5.0});
        numcpp::Matrix<double> kernel({2}, {0.5, 0.5});
        auto conv_result = vec1d.conv1d(kernel);
        vec1d.print("一维向量");
        kernel.print("卷积核");
        conv_result.print("卷积结果");

        // 7. 统计功能
        std::cout << "\nmat1 统计信息：" << std::endl;
        std::cout << "  元素总和: " << mat1.sum() << std::endl;
        std::cout << "  平均值:   " << mat1.mean() << std::endl;
        std::cout << "  最大值:   " << mat1.max() << std::endl;
        std::cout << "  最小值:   " << mat1.min() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "\n错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}