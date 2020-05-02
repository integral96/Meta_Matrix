#pragma once

#include <iostream>

#include <Circuliar.hpp>

#include <boost/container/vector.hpp>

#include <array2d.hpp>

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
        std::cout << trans(matx_u) << typeid (trans(matx_u)).name() << std::endl;
    }
};

///meta mult

///////////////////////////
template<size_t Index1, size_t MaxIndex1, size_t Index2, size_t MaxIndex2>
struct equation_loopback
{
    static constexpr size_t next_index1 = Index1;
    static constexpr size_t next_index2 = Index2 + 1;
};
template<size_t Index1, size_t MaxIndex1, size_t MaxIndex2>
struct equation_loopback<Index1, MaxIndex1, MaxIndex2, MaxIndex2>
{
    static constexpr size_t next_index1 = Index1 + 1;
    static constexpr size_t next_index2 = 0;
};
///////////////////////////
template<size_t Index1, size_t MaxIndex1, size_t Index2, size_t MaxIndex2>
struct equation_block
{
    using loop = equation_loopback<Index1, MaxIndex1, Index2, MaxIndex2>;
    using next = equation_block<loop::next_index1, MaxIndex1, loop::next_index2, MaxIndex2>;

    template<typename T, typename Matrix, typename Callable>
    void operator()(T& ust, T& uet, Matrix& A, size_t i, size_t j, Callable&& callback) {
            A(0, j + Index2) = ust;
            A(MaxIndex1 + 1, j + Index2) = uet;
            A(i + Index1, MaxIndex2 + 1) = std::invoke(std::forward<Callable>(callback), i);
            A(i + Index1, 0) = std::invoke(std::forward<Callable>(callback), i);
            next()(ust, uet, A, i, j, callback);
    }

    template<typename T, typename Matrix>
    void update(Matrix& A, size_t i, size_t j) {
        A(i + Index1, j + Index2 + 1) = A(i + Index1 - 1, j + Index2) + A(i + Index1 + 1, j + Index2) - A(i + Index1, j + Index2 - 1);
        next().update(A, i, j);
    }
};

template<size_t Index1, size_t MaxIndex1, size_t MaxIndex2>
struct equation_block<Index1, MaxIndex1, MaxIndex2, MaxIndex2>
{
    using next = equation_block<Index1 + 1, MaxIndex1, 0, MaxIndex2>;

    template<typename T, typename Matrix, typename Callable>
    void operator()(T& ust, T& uet, Matrix& A, size_t i, size_t j, Callable&& callback) {
            A(0, j + MaxIndex2) = ust;
            A(MaxIndex1 + 1, j + MaxIndex2) = uet;
            A(i + Index1, MaxIndex2 + 1) = std::invoke(std::forward<Callable>(callback), i);
            A(i + Index1, 0) = std::invoke(std::forward<Callable>(callback), i);
            next()(ust, uet, A, i, j, callback);
    }

    template<typename T, typename Matrix>
    void update(Matrix& A, size_t i, size_t j) {
        A(i + Index1, j + MaxIndex2 + 1) = A(i + Index1 - 1, j + MaxIndex2) + A(i + Index1 + 1, j + MaxIndex2) - A(i + Index1, j + MaxIndex2 - 1);
        next().update(A, i, j);
    }
};

template<size_t MaxIndex1, size_t MaxIndex2>
struct equation_block<MaxIndex1, MaxIndex1, MaxIndex2, MaxIndex2>
{
    template<typename T, typename Matrix, typename Callable>
    void operator()(T& ust, T& uet, Matrix& A, size_t i, size_t j, Callable&& callback) {
            A(0, j + MaxIndex2) = ust;
            A(MaxIndex1 + 1, j + MaxIndex2) = uet;
            A(i + MaxIndex1, MaxIndex2 + 1) = std::invoke(std::forward<Callable>(callback), i);
            A(i + MaxIndex1, 0) = std::invoke(std::forward<Callable>(callback), i);
    }

    template<typename T, typename Matrix>
    void update(Matrix& A, size_t i, size_t j) {
        A(i + MaxIndex1, j + MaxIndex2 + 1) = A(i + MaxIndex1 - 1, j + MaxIndex2) + A(i + MaxIndex1 + 1, j + MaxIndex2) - A(i + MaxIndex1, j + MaxIndex2 - 1);
    }
};

template<typename T, size_t N, size_t M, typename Callable>
class equation_meta {
private:
    using value_type = T;
    array2d<T, N + 2, M + 2, boost::container::vector> A;
public:
    equation_meta() {
        A.fill(0);
    }
    void equation_resolv(value_type ust, value_type uet, Callable callback) {

        size_t Rang = A.size(1);
        equation_block<0, N, 0, M> block;
        value_type tmp1(ust);
        value_type tmp2(uet);
        for (size_t i = 0; i < Rang; i ++/*= N*/)
        {
            for (size_t j = 0; j < Rang; j ++/*= M*/)
            {
                block(tmp1, tmp2, A, i, j, callback);
//                block.update(A, i, j);
            }
        }
    }
};
