// src/mul_array.cpp
#include "mul_array.h"  // 包含头文件，获取函数声明
#include <vector>
using namespace std;

// ---------------------- 非模板函数的实现（模板函数已在头文件实现）----------------------
// 计算任意维度数组的步长
vector<size_t> compute_strides(const vector<size_t> &shape)
{
    size_t dims = shape.size();
    vector<size_t> strides(dims, 1);
    for (int i = dims - 2; i >= 0; --i)
    {
        strides[i] = strides[i + 1] * shape[i + 1];
    }
    return strides;
}

// 将多维索引转换为一维索引
size_t multi_to_single_index(
    const vector<size_t> &indices,
    const vector<size_t> &shape,
    const vector<size_t> &strides)
{
    if (indices.size() != shape.size())
        throw invalid_argument("索引数量与维度不匹配");
    size_t index = 0;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        if (indices[i] >= shape[i])
            throw out_of_range("索引越界");
        index += indices[i] * strides[i];
    }
    return index;
}

// 编码：多维→一维（修复dims参数未使用：添加维度校验）
vector<double> encode(const vector<double> &multi_dim_data, const vector<int> &dims)
{
    // 校验：多维数据长度是否与dims计算的总元素数一致（实际功能，消除警告）
    size_t total = 1;
    for (int dim : dims)
        total *= dim;
    if (multi_dim_data.size() != total)
        throw invalid_argument("编码：多维数据长度与维度不匹配");
    
    vector<double> one_dim;
    one_dim.reserve(multi_dim_data.size());
    for (double val : multi_dim_data)
        one_dim.push_back(val);
    return one_dim;
}

// 解码：一维→多维（修复dims参数未使用：添加维度校验）
vector<double> decode(const vector<double> &one_dim_data, const vector<int> &dims)
{
    // 校验：一维数据长度是否与dims计算的总元素数一致（消除警告）
    size_t total = 1;
    for (int dim : dims)
        total *= dim;
    if (one_dim_data.size() != total)
        throw invalid_argument("解码：一维数据长度与维度不匹配");
    
    vector<double> multi_dim_data;
    multi_dim_data.reserve(one_dim_data.size());
    for (double val : one_dim_data)
        multi_dim_data.push_back(val);
    return multi_dim_data;
}

// 矩阵加法（元素级）
vector<double> add(const vector<double> &a_one_dim, const vector<double> &b_one_dim)
{
    vector<double> res;
    if (a_one_dim.size() != b_one_dim.size())
        return res;
    res.reserve(a_one_dim.size());
    for (size_t i = 0; i < a_one_dim.size(); i++)
        res.push_back(a_one_dim[i] + b_one_dim[i]);
    return res;
}

// 矩阵乘法（元素级）
vector<double> multiply(const vector<double> &a_one_dim, const vector<double> &b_one_dim)
{
    vector<double> res;
    if (a_one_dim.size() != b_one_dim.size())
        return res;
    res.reserve(a_one_dim.size());
    for (size_t i = 0; i < a_one_dim.size(); i++)
        res.push_back(a_one_dim[i] * b_one_dim[i]);
    return res;
}

// 1维卷积
vector<double> conv1d(const vector<double> &one_dim_data, const vector<double> &kernel)
{
    vector<double> res;
    size_t data_len = one_dim_data.size();
    size_t kernel_len = kernel.size();
    for (size_t i = 0; i <= data_len - kernel_len; i++)
    {
        double sum = 0;
        for (size_t j = 0; j < kernel_len; j++)
            sum += one_dim_data[i + j] * kernel[j];
        res.push_back(sum);
    }
    return res;
}

// 计算总元素数
size_t calculateTotalElements(const vector<size_t> &shape)
{
    size_t total = 1;
    for (size_t dim : shape)
        total *= dim;
    return total;
}
