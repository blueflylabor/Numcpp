#ifndef NUMCPP_MATRIX_H
#define NUMCPP_MATRIX_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <initializer_list>

namespace numcpp {

template <typename T>
class Matrix {
private:
    std::vector<size_t> shape_;       // 矩阵维度
    std::vector<size_t> strides_;     // 步长
    std::vector<T> data_;             // 数据存储（一维）

    // 计算步长
    void compute_strides() {
        size_t dims = shape_.size();
        strides_.resize(dims, 1);
        for (int i = dims - 2; i >= 0; --i) {
            strides_[i] = strides_[i + 1] * shape_[i + 1];
        }
    }

    // 计算总元素数
    size_t total_elements() const {
        size_t total = 1;
        for (size_t dim : shape_) {
            total *= dim;
        }
        return total;
    }

    // 将多维索引转换为一维索引
    size_t index(const std::vector<size_t>& indices) const {
        if (indices.size() != shape_.size()) {
            throw std::invalid_argument("索引数量与维度不匹配");
        }
        
        size_t idx = 0;
        for (size_t i = 0; i < indices.size(); ++i) {
            if (indices[i] >= shape_[i]) {
                throw std::out_of_range("索引越界");
            }
            idx += indices[i] * strides_[i];
        }
        return idx;
    }

    // 递归打印辅助函数
    void print_recursive(std::vector<size_t> indices, size_t dim, int indent) const {
        if (dim == shape_.size() - 1) {
            std::cout << "[";
            for (size_t i = 0; i < shape_[dim]; ++i) {
                indices.push_back(i);
                std::cout << std::setw(6) << data_[index(indices)];
                if (i != shape_[dim] - 1) {
                    std::cout << ", ";
                }
                indices.pop_back();
            }
            std::cout << "]";
            return;
        }
        
        std::cout << "[" << std::endl;
        for (size_t i = 0; i < shape_[dim]; ++i) {
            std::cout << std::string(indent + 4, ' ');
            indices.push_back(i);
            print_recursive(indices, dim + 1, indent + 4);
            indices.pop_back();
            if (i != shape_[dim] - 1) {
                std::cout << ",";
            }
            std::cout << std::endl;
        }
        std::cout << std::string(indent, ' ') << "]";
    }

    // 设置指定索引处的值
    void set(const std::vector<size_t>& indices, T value) {
        data_[index(indices)] = value;
    }

    // 递增索引（用于迭代）
    bool increment_indices(std::vector<size_t>& indices) const {
        for (int i = indices.size() - 1; i >= 0; --i) {
            indices[i]++;
            if (indices[i] < shape_[i]) {
                return true;
            }
            indices[i] = 0;
        }
        return false;
    }

public:
    // 构造函数
    Matrix() = default;

    // 从维度创建矩阵（默认初始化）
    explicit Matrix(const std::vector<size_t>& shape) 
        : shape_(shape) {
        compute_strides();
        data_.resize(total_elements());
    }

    // 从维度和初始值创建矩阵
    Matrix(const std::vector<size_t>& shape, const std::vector<T>& data)
        : shape_(shape) {
        if (data.size() != total_elements()) {
            throw std::invalid_argument("数据大小与维度不匹配");
        }
        compute_strides();
        data_ = data;
    }

    // 从初始化列表创建矩阵
    Matrix(std::initializer_list<size_t> shape, std::initializer_list<T> data)
        : shape_(shape.begin(), shape.end()) {
        if (data.size() != total_elements()) {
            throw std::invalid_argument("数据大小与维度不匹配");
        }
        compute_strides();
        data_ = std::vector<T>(data.begin(), data.end());
    }

    // 复制/移动构造与赋值（默认）
    Matrix(const Matrix& other) = default;
    Matrix(Matrix&& other) noexcept = default;
    Matrix& operator=(const Matrix& other) = default;
    Matrix& operator=(Matrix&& other) noexcept = default;

    // 析构函数
    ~Matrix() = default;

    // 基础属性获取
    const std::vector<size_t>& shape() const { return shape_; }
    size_t ndim() const { return shape_.size(); }
    size_t size() const { return data_.size(); }
    const T* data() const { return data_.data(); }
    T* data() { return data_.data(); }

    // 多维索引访问
    template <typename... Args>
    const T& operator()(Args... args) const {
        static_assert(sizeof...(args) > 0, "至少需要一个索引");
        std::vector<size_t> indices = {static_cast<size_t>(args)...};
        return data_[index(indices)];
    }
    template <typename... Args>
    T& operator()(Args... args) {
        static_assert(sizeof...(args) > 0, "至少需要一个索引");
        std::vector<size_t> indices = {static_cast<size_t>(args)...};
        return data_[index(indices)];
    }

    // 核心功能：转置
    Matrix<T> transpose() const {
        std::vector<size_t> new_shape = shape_;
        std::reverse(new_shape.begin(), new_shape.end());
        Matrix<T> result(new_shape);

        std::vector<size_t> indices(shape_.size(), 0);
        std::vector<size_t> transposed_indices(shape_.size(), 0);

        do {
            size_t original_idx = index(indices);
            for (size_t i = 0; i < shape_.size(); ++i) {
                transposed_indices[i] = indices[shape_.size() - 1 - i];
            }
            result.set(transposed_indices, data_[original_idx]);
        } while (increment_indices(indices));

        return result;
    }

    // 核心功能：重塑
    Matrix<T> reshape(const std::vector<size_t>& new_shape) const {
        size_t new_total = 1;
        for (size_t dim : new_shape) new_total *= dim;
        if (new_total != total_elements()) {
            throw std::invalid_argument("重塑后的总元素数必须保持不变");
        }
        return Matrix<T>(new_shape, data_);
    }

    // 核心功能：1D卷积
    Matrix<T> conv1d(const Matrix<T>& kernel) const {
        if (ndim() != 1 || kernel.ndim() != 1) {
            throw std::invalid_argument("1D卷积仅支持一维矩阵和一维卷积核");
        }
        size_t data_len = shape_[0];
        size_t kernel_len = kernel.shape_[0];
        std::vector<size_t> new_shape = {data_len - kernel_len + 1};
        Matrix<T> result(new_shape);

        for (size_t i = 0; i < new_shape[0]; ++i) {
            T sum = 0;
            for (size_t j = 0; j < kernel_len; ++j) {
                sum += data_[i + j] * kernel.data_[j];
            }
            result(i) = sum;
        }
        return result;
    }

    // 元素级运算（矩阵-矩阵）
    Matrix<T> operator+(const Matrix<T>& other) const {
        if (shape_ != other.shape_) throw std::invalid_argument("矩阵维度必须匹配");
        Matrix<T> res(shape_);
        for (size_t i = 0; i < data_.size(); ++i) res.data_[i] = data_[i] + other.data_[i];
        return res;
    }
    Matrix<T> operator-(const Matrix<T>& other) const {
        if (shape_ != other.shape_) throw std::invalid_argument("矩阵维度必须匹配");
        Matrix<T> res(shape_);
        for (size_t i = 0; i < data_.size(); ++i) res.data_[i] = data_[i] - other.data_[i];
        return res;
    }
    Matrix<T> operator*(const Matrix<T>& other) const {
        if (shape_ != other.shape_) throw std::invalid_argument("矩阵维度必须匹配");
        Matrix<T> res(shape_);
        for (size_t i = 0; i < data_.size(); ++i) res.data_[i] = data_[i] * other.data_[i];
        return res;
    }
    Matrix<T> operator/(const Matrix<T>& other) const {
        if (shape_ != other.shape_) throw std::invalid_argument("矩阵维度必须匹配");
        Matrix<T> res(shape_);
        for (size_t i = 0; i < data_.size(); ++i) {
            if (other.data_[i] == 0) throw std::domain_error("元素不能为零，无法除法");
            res.data_[i] = data_[i] / other.data_[i];
        }
        return res;
    }

    // 线性代数矩阵乘法
    Matrix<T> matmul(const Matrix<T>& other) const {
        if (ndim() != 2 || other.ndim() != 2) throw std::invalid_argument("仅支持二维矩阵乘法");
        if (shape_[1] != other.shape_[0]) throw std::invalid_argument("前矩阵列数需等于后矩阵行数");
        std::vector<size_t> new_shape = {shape_[0], other.shape_[1]};
        Matrix<T> res(new_shape);

        for (size_t i = 0; i < new_shape[0]; ++i) {
            for (size_t j = 0; j < new_shape[1]; ++j) {
                T sum = 0;
                for (size_t k = 0; k < shape_[1]; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                res(i, j) = sum;
            }
        }
        return res;
    }

    // 标量运算
    Matrix<T> operator+(T scalar) const {
        Matrix<T> res(shape_);
        for (size_t i = 0; i < data_.size(); ++i) res.data_[i] = data_[i] + scalar;
        return res;
    }
    Matrix<T> operator-(T scalar) const {
        Matrix<T> res(shape_);
        for (size_t i = 0; i < data_.size(); ++i) res.data_[i] = data_[i] - scalar;
        return res;
    }
    Matrix<T> operator*(T scalar) const {
        Matrix<T> res(shape_);
        for (size_t i = 0; i < data_.size(); ++i) res.data_[i] = data_[i] * scalar;
        return res;
    }
    Matrix<T> operator/(T scalar) const {
        if (scalar == 0) throw std::domain_error("除数不能为零");
        Matrix<T> res(shape_);
        for (size_t i = 0; i < data_.size(); ++i) res.data_[i] = data_[i] / scalar;
        return res;
    }

    // 统计功能
    T sum() const { return std::accumulate(data_.begin(), data_.end(), T(0)); }
    T mean() const {
        if (data_.empty()) throw std::runtime_error("空矩阵无法计算平均值");
        return sum() / static_cast<T>(data_.size());
    }
    T max() const {
        if (data_.empty()) throw std::runtime_error("空矩阵无法找最大值");
        return *std::max_element(data_.begin(), data_.end());
    }
    T min() const {
        if (data_.empty()) throw std::runtime_error("空矩阵无法找最小值");
        return *std::min_element(data_.begin(), data_.end());
    }

    // 打印功能
    void print(const std::string& name = "") const {
        if (!name.empty()) std::cout << name << " = ";
        std::cout << "Matrix(shape=(";
        for (size_t i = 0; i < shape_.size(); ++i) {
            std::cout << shape_[i];
            if (i != shape_.size() - 1) std::cout << ", ";
        }
        std::cout << ")):\n";
        print_recursive({}, 0, 0);
        std::cout << std::endl;
    }
};

} // namespace numcpp

#endif // NUMCPP_MATRIX_H