#include <gtest/gtest.h>
#include "../src/mul_array.h"
#include <vector>

// 关键：使用 using namespace std; 或在 vector 前加 std::
// 这里选择 using namespace std; 避免重复写 std::，保持代码简洁
using namespace std;

// 测试1：步长计算和索引转换
TEST(MultiArrayTest, StrideAndIndex)
{
    vector<size_t> shape = {2, 3, 4};
    vector<size_t> strides = compute_strides(shape);
    
    EXPECT_EQ(strides[0], 12);
    EXPECT_EQ(strides[1], 4);
    EXPECT_EQ(strides[2], 1);
    
    vector<size_t> indices = {1, 2, 3};
    size_t single_idx = multi_to_single_index(indices, shape, strides);
    EXPECT_EQ(single_idx, 23);
    
    vector<size_t> invalid_indices = {1, 3, 2};
    EXPECT_THROW(multi_to_single_index(invalid_indices, shape, strides), out_of_range);
}

// 测试2：矩阵加法（元素级）
TEST(MultiArrayTest, ElementWiseAdd)
{
    vector<double> a = {1.0, 2.0, 3.0, 4.0};
    vector<double> b = {5.0, 6.0, 7.0, 8.0};
    vector<double> expected = {6.0, 8.0, 10.0, 12.0};
    
    vector<double> res = add(a, b);
    EXPECT_EQ(res.size(), expected.size());
    for (size_t i = 0; i < res.size(); i++)
        EXPECT_NEAR(res[i], expected[i], 1e-6);
    
    vector<double> c = {1.0, 2.0};
    vector<double> invalid_res = add(a, c);
    EXPECT_TRUE(invalid_res.empty());
}

// 测试3：1维卷积
TEST(MultiArrayTest, Conv1D)
{
    vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    vector<double> kernel = {0.5, 0.5};
    vector<double> expected = {1.5, 2.5, 3.5, 4.5};
    
    vector<double> res = conv1d(data, kernel);
    EXPECT_EQ(res.size(), expected.size());
    for (size_t i = 0; i < res.size(); i++)
        EXPECT_NEAR(res[i], expected[i], 1e-6);
}

// 测试4：编码/解码的维度校验
TEST(MultiArrayTest, EncodeDecodeValidation)
{
    vector<double> multi_data = {1.0, 2.0, 3.0, 4.0};
    vector<int> dims_valid = {2, 2};    // 2×2=4，与数据长度匹配
    vector<int> dims_invalid = {3, 2};  // 3×2=6，与数据长度不匹配（修复笔误）
    
    // 测试合法编码
    EXPECT_NO_THROW(encode(multi_data, dims_valid));
    // 测试非法编码（长度不匹配）
    EXPECT_THROW(encode(multi_data, dims_invalid), invalid_argument);
}

// 测试5：矩阵打印异常
TEST(MultiArrayTest, PrintMatrixException)
{
    vector<int> data = {1,2,3,4};
    vector<size_t> shape = {2,3};  // 2×3=6≠4
    EXPECT_THROW(printMatrix(data, shape), invalid_argument);
}

// 测试入口
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}