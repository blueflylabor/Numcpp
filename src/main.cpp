// src/main.cpp
#include "mul_array.h"
using namespace std;

int main()
{
    // 3维矩阵加法示例
    vector<int> dims = {3, 2, 4};
    vector<double> mat_a = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    vector<double> mat_b = {24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    
    try
    {
        vector<double> one_dim_a = encode(mat_a, dims);
        vector<double> one_dim_b = encode(mat_b, dims);
        vector<double> one_dim_res = add(one_dim_a, one_dim_b);
        vector<double> mat_res = decode(one_dim_res, dims);
        printMatrix(mat_res, {3,2,4});
        
        // 其他打印示例
        vector<int> vec1D = {1,2,3,4,5};
        printMatrix(vec1D, {5});
        
        vector<int> vec2D = {1,2,3,4,5,6};
        printMatrix(vec2D, {2,3});
    }
    catch (const exception &e)
    {
        cerr << "错误: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}