// src/mul_array.h
#ifndef MUL_ARRAY_H
#define MUL_ARRAY_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>

// 计算任意维度数组的步长
std::vector<size_t> compute_strides(const std::vector<size_t> &shape);

// 将多维索引转换为一维索引
size_t multi_to_single_index(
    const std::vector<size_t> &indices,
    const std::vector<size_t> &shape,
    const std::vector<size_t> &strides);

// 编码：多维→一维（修复dims参数未使用的问题，实际场景可用于校验维度）
std::vector<double> encode(const std::vector<double> &multi_dim_data, const std::vector<int> &dims);

// 解码：一维→多维（同理，修复dims参数未使用）
std::vector<double> decode(const std::vector<double> &one_dim_data, const std::vector<int> &dims);

// 矩阵加法（元素级）
std::vector<double> add(const std::vector<double> &a_one_dim, const std::vector<double> &b_one_dim);

// 矩阵乘法（元素级）
std::vector<double> multiply(const std::vector<double> &a_one_dim, const std::vector<double> &b_one_dim);

// 1维卷积
std::vector<double> conv1d(const std::vector<double> &one_dim_data, const std::vector<double> &kernel);

// 计算总元素数
size_t calculateTotalElements(const std::vector<size_t> &shape);

// 递归打印辅助函数（模板函数，必须在头文件实现）
template <typename T>
void printRecursive(
    const std::vector<T> &data,
    const std::vector<size_t> &shape,
    std::vector<size_t> &indices,
    size_t dimIndex,
    const std::vector<size_t> &strides,
    int indent = 0)
{
    if (dimIndex == shape.size() - 1)
    {
        for (size_t i = 0; i < shape[dimIndex]; ++i)
        {
            indices[dimIndex] = i;
            size_t index = 0;
            for (size_t j = 0; j < indices.size(); ++j)
                index += indices[j] * strides[j];
            std::cout << std::setw(6) << data[index];
        }
        return;
    }
    for (size_t i = 0; i < shape[dimIndex]; ++i)
    {
        if (dimIndex > 0)
            std::cout << std::string(indent, ' ');
        if (shape.size() > 2 && dimIndex == 0)
            std::cout << "第" << i << "层: ";
        std::cout << "[";
        indices[dimIndex] = i;
        printRecursive(data, shape, indices, dimIndex + 1, strides, indent + 4);
        std::cout << "]";
        std::cout << "\n";
    }
}

// 主打印函数（模板函数，必须在头文件实现）
template <typename T>
void printMatrix(
    const std::vector<T> &data,
    const std::vector<size_t> &shape)
{
    size_t totalElements = calculateTotalElements(shape);
    if (data.size() != totalElements)
        throw std::invalid_argument("数据长度与维度信息不匹配");
    if (shape.empty())
    {
        std::cout << "空矩阵" << std::endl;
        return;
    }
    std::vector<size_t> strides(shape.size(), 1);
    for (int i = shape.size() - 2; i >= 0; --i)
        strides[i] = strides[i + 1] * shape[i + 1];
    
    std::cout << "矩阵维度: ";
    for (size_t i = 0; i < shape.size(); ++i)
    {
        std::cout << shape[i];
        if (i != shape.size() - 1)
            std::cout << "×";
    }
    std::cout << "\n矩阵内容:\n";
    std::vector<size_t> indices(shape.size(), 0);
    printRecursive(data, shape, indices, 0, strides);
    std::cout << std::endl;
}

#endif // MUL_ARRAY_H