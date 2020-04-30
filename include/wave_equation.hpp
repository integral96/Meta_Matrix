#pragma once

#include <iostream>
#include <vector>

#include <boost/container/vector.hpp>

#include <array2d.hpp>

template <typename Matrix>
class transposed_view;
template<typename Matrix>
std::ostream& operator << (std::ostream&, const transposed_view<Matrix>&);

template <typename Matrix>
class transposed_view
{
private:
    Matrix& ref;
public:
    using value_type = typename Matrix::value_type;
    using size_type  = typename Matrix::size_type;
    explicit transposed_view(Matrix& A): ref(A) {}

    value_type& operator()(size_type r, size_type c) { return ref(c, r); }
    const value_type& operator()(size_type r, size_type c) const { return ref(c, r); }
    friend std::ostream& operator << <Matrix> (std::ostream&, const transposed_view<Matrix>&);
};
template<typename Matrix>
std::ostream& operator << (std::ostream& oss, const transposed_view<Matrix>& matrix) {
    for (size_t i = 0; i < matrix.ref.size(1); i++) {
        for (size_t j = 0; j < matrix.ref.size(2); j++)
            {
                oss << std::setw(6) << std::setprecision(3) << matrix.ref.at(j, i) << "\t";
            }
            oss << std::endl;
    }
    return oss;
}

template<typename Matrix>
inline transposed_view<Matrix> trans(Matrix& A) {
    return transposed_view<Matrix>(A);
}

template <typename T, size_t N, size_t M, typename Func>
class Hyperbolic_equation
{
private:
    array2d<T, N + 1, M + 1, boost::container::vector> matx_u;
    T csqr, ust, uet;
public:
    Hyperbolic_equation(T csqr, T ust, T uet) : csqr(csqr), ust(ust), uet(uet) { }
    void equation(Func func) {
        for(size_t j = 0; j < M; ++j) {
            matx_u(0, j) = ust;
            matx_u(N, j) = uet;
        }
        for(size_t i = 0; i < N - 1; ++i) {
            matx_u(i, 1) = func(i);
            matx_u(i, 0) = func(i);
        }
        for(size_t j = 1; j <= M - 1; ++j) {
            for(size_t i = 1; i <= N - 1; ++i) {
                matx_u(i, j + 1) = matx_u(i - 1, j) + matx_u(i + 1, j) - matx_u(i, j - 1);
            }
        }
        std::cout << transposed_view<array2d<T, N + 1, M + 1, boost::container::vector>>(matx_u) << std::endl;
    }
};
