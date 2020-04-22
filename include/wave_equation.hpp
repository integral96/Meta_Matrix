#pragma once

#include <iostream>
#include <vector>

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
        std::cout << matx_u << std::endl;
    }
};
