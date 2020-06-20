#pragma once

#include <boost/container/stable_vector.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_constructible.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/type_traits.hpp>

#include <initializer_list>
#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>

template <typename T>
using EnableIfFunc = typename boost::enable_if<boost::is_convertible<T, boost::container::stable_vector<size_t>>>::type;

template<typename T, size_t R, size_t C, template<typename Elem> class Cont = std::initializer_list>
class array2d;

template<typename T, size_t R, size_t C, template<typename Elem> class Cont>
std::ostream& operator << (std::ostream&, const array2d<T, R, C, Cont>&);

template<typename T, size_t R, size_t C, template<typename Elem> class Cont>
class array2d 
{
public:
    using value_type = T;
    using size_type  = size_t;
    static const size_t value_R = R;
    static const size_t value_C = C;
private:
    using iterator = boost::container::stable_vector_iterator<T*, true>;
    using const_iterator = const boost::container::stable_vector_iterator<T*, true>;
    boost::container::stable_vector<T> arr;
public:
    array2d();
    ~array2d(){}
    explicit array2d(const Cont<T>&);
    explicit array2d(const array2d<T, R, C, Cont>&);

    template<typename Func, typename = EnableIfFunc<Func>>
    explicit array2d(Func&&);

    explicit array2d(array2d<T, R, C, Cont>&&);

    void init_list(Cont<T>&) ;

    constexpr T* data() noexcept { return arr.internal_data(); }
    constexpr const T* data() const noexcept { return arr.internal_data(); }
    constexpr T& at(const size_t r, const size_t c) { return arr.at(r*C + c); }
    constexpr const T& at(const size_t r, const size_t c) const { return arr.at(r*C + c); }
    constexpr T& operator()(const size_t r, const size_t c) { return arr[r*C + c]; }
    constexpr const T& operator()(const size_t r, const size_t c) const { return arr[r*C + c]; }
    constexpr bool empty() const noexcept { return R == 0 || C == 0; }
    array2d<T, R, C, Cont>& operator = (const array2d<T, R, C, Cont>& other) ;
public:
    constexpr size_t size(const size_t rank) const;
    void fill(T&& val);
    void push_t(const T& val);
    void sortir();
    void swap(array2d<T, R, C, Cont>& other) noexcept;
public:
    const_iterator cbegin() const { return arr.cbegin(); }
    const_iterator cend() const { return arr.cend(); }
    iterator begin() { return arr.begin(); }
    iterator end() { return arr.end(); }
    friend std::ostream& operator << <T, R, C, Cont> (std::ostream&, const array2d<T, R, C, Cont>&);
    array2d<T, R, C, Cont>& operator * (const array2d<T, R, C, Cont>& A);
    array2d<T, R, C, Cont>& operator * (const std::vector<T>& A);
    array2d<T, R, C, Cont>& multy(const array2d<T, R, C, Cont>&, const array2d<T, R, C, Cont>&);
};

template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>::array2d() : arr(R*C) {
}

template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>::array2d(const Cont<T>& l)  {
    arr.insert(arr.begin(), l.begin(), l.end());
}

template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>::array2d(const array2d<T, R, C, Cont>& other) : arr(other.arr)  {
    
}

template<typename T, size_t R, size_t C, template<typename> class Cont>
template<typename Func, typename>
array2d<T, R, C, Cont>::array2d(Func&& other): arr(std::forward<Func>(other)) {

}
template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>::array2d(array2d<T, R, C, Cont>&& other) : arr(std::move(other.arr))  {
    
}

template<typename T, size_t R, size_t C, template<typename> class Cont>
void array2d<T, R, C, Cont>::init_list(Cont<T>& other) {
    arr.insert(arr.begin(), other.begin(), other.end());
} 

template<typename T, size_t R, size_t C, template<typename> class Cont>
constexpr size_t array2d<T, R, C, Cont>::size(const size_t rank) const {
    if(rank == 1) return R;
    else if(rank == 2) return C;
    else throw std::out_of_range("out of range be be!");
}
template<typename T, size_t R, size_t C, template<typename> class Cont>
void array2d<T, R, C, Cont>::fill(T&& val) {
    std::fill(arr.begin(), arr.end(), std::forward<T>(val));
}
template<typename T, size_t R, size_t C, template<typename> class Cont>
void array2d<T, R, C, Cont>::push_t(const T& val) {
    arr.insert(arr.begin(), val);
}
template<typename T, size_t R, size_t C, template<typename> class Cont>
void array2d<T, R, C, Cont>::sortir() {
    for (size_t i = 0; i < size(1) - 1; i++) {
        for (size_t j = i + 1; j < size(2); j++)
            {
                std::swap(arr.at(i*size(1) + j), arr.at(j*size(2) + i));
            }
    }
//    std::make_heap(arr.begin(), arr.end());
}
template<typename T, size_t R, size_t C, template<typename> class Cont>
void array2d<T, R, C, Cont>::swap(array2d& other) noexcept {
    arr.swap(other.arr);
}
template<typename T, size_t R, size_t C, template<typename> class Cont>
std::ostream& operator << (std::ostream& oss, const array2d<T, R, C, Cont>& matrix) {
    for (size_t i = 0; i < matrix.size(1); i++) {
        for (size_t j = 0; j < matrix.size(2); j++)
            {
                oss << std::setw(6) << std::setprecision(3) << matrix.at(i, j) << "\t";
            }
            oss << std::endl;
    }
    return oss;
}
template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>& array2d<T, R, C, Cont>::operator = (const array2d<T, R, C, Cont>& other) {
        this->arr = other.arr;
    return *this;
}
/*!
 *  Перегрузка оператора умножения на матрицу.
 */
template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>& array2d<T, R, C, Cont>::operator * (const array2d<T, R, C, Cont>& A) {
    size_t Rang1 = A.size(1);
    size_t Rang2 = A.size(2);
    array2d<T, R, C, Cont> MTRX(*this);
    boost::shared_array<T> tmp_arr(new T[Rang1*Rang2]);
    for (size_t i = 0; i < Rang1; ++i)
        for (size_t j = 0; j < Rang2; ++j)
            for (size_t k = 0; k < Rang1; ++k)
                tmp_arr[i*Rang2 + j] += A.at(i, k)*(MTRX.at(k, j));
    for (size_t i = 0; i < Rang1; ++i)
        for (size_t j = 0; j < Rang2; ++j) {
            this->at(i, j) = tmp_arr[i*Rang2 + j];
            tmp_arr[i*Rang2 + j] = 0;
        }

    return *this;
}

/*!
 *  Перегрузка оператора умножения на вектор.
 */
template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>& array2d<T, R, C, Cont>::operator * (const std::vector<T>& Vec) {
    assert(Vec.size() == R || Vec.size() == C);
    array2d<T, R, C, Cont> MTRX(*this);
    boost::shared_array<T> tmp_arr(new T[Vec.size()]);
    for (size_t i = 0; i < Vec.size(); ++i)
        for (size_t j = 0; j < Vec.size(); ++j)
                tmp_arr[i] += Vec[i]*(MTRX.at(i, j));
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j) {
            this->at(i, j) = tmp_arr[i];
            tmp_arr[i] = 0;
        }

    return *this;
}
/*!
 *  умножение двух матриц, классический способ.
 */
template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>& array2d<T, R, C, Cont>::multy(const array2d<T, R, C, Cont>& A, const array2d<T, R, C, Cont>& B) {
    assert(A.size(1) == B.size(2));
    size_t Rang = A.size(1);
    array2d<T, R, C, Cont> C_MTX;
    for (size_t i = 0; i < Rang; ++i)
    {
        for (size_t j = 0; j < Rang; ++j)
        {
            T tmp{};
            for (size_t k = 0; k < Rang; ++k)
            {
                tmp += A.at(i, k)*B.at(k, j);
            }
            this->arr[i*C + j] = tmp;
        }
    }
    return *this;
}

/*!
 *  Умножение матриц методом метапрограммирования. Метод 1
 */

template<size_t K, typename T>
struct multi_tmp
{
    using sub_type = multi_tmp<K - 1, T>;

    multi_tmp(const T& val) : value(val), sub(val) {}

    T value;
    sub_type sub;
};

template<typename T>
struct multi_tmp<0, T>
{
    multi_tmp(const T& val) {}
};
///////////////////////////
template<size_t Index1, size_t MaxIndex1, size_t Index2, size_t MaxIndex2>
struct loopback
{
    static constexpr size_t next_I = Index1;
    static constexpr size_t next_J = Index2 + 1;
};
template<size_t Index1, size_t MaxIndex1, size_t MaxIndex2>
struct loopback<Index1, MaxIndex1, MaxIndex2, MaxIndex2>
{
    static constexpr size_t next_I = Index1 + 1;
    static constexpr size_t next_J = 0;
};
////////////////////
///////////////////////////
template<size_t I, size_t N, size_t J, size_t M>
struct mult_block
{
    typedef  loopback<I, N, J, M> loop;
    typedef  mult_block<loop::next_I, N, loop::next_J, M> next;

    template<typename T, typename Matrix>
    void operator()(T& tmp, const Matrix& A, const Matrix& B, size_t i, size_t k, size_t j) {
        tmp.value += A(i + I, k) * B(k, j + J);
        next()(tmp.sub, A, B, i, k, j);
    }

    template<typename T, typename Matrix>
    void update(const T& tmp, Matrix& C, size_t i, size_t j) {
        C(i + I, j + J) = tmp.value;
        next().update(tmp.sub, C, i, j);
    }
};

template<size_t I, size_t N, size_t M>
struct mult_block<I, N, M, M>
{
    typedef  mult_block<I + 1, N, 0, M> next;

    template<typename T, typename Matrix>
    void operator()(T& tmp, const Matrix& A, const Matrix& B, size_t i, size_t k, size_t j) {
        tmp.value += A(i + I, k) * B(k, j + M);
        next()(tmp.sub, A, B, i, k, j);
    }

    template<typename T, typename Matrix>
    void update(const T& tmp, Matrix& C, size_t i, size_t j) {
        C(i + I, j + M) = tmp.value;
        next().update(tmp.sub, C, i, j);
    }
};

template<size_t N, size_t M>
struct mult_block<N, N, M, M>
{
    template<typename T, typename Matrix>
    void operator()(T& tmp, const Matrix& A, const Matrix& B, size_t i, size_t k, size_t j) {
        tmp.value += A(i + N, k) * B(k, j + M);
    }

    template<typename T, typename Matrix>
    void update(const T& tmp, Matrix& C, size_t i, size_t j) {
        C(i + N, j + M) = tmp.value;
    }
};

template<size_t N, size_t M, typename Matrix>
void mult_meta(const Matrix& A, const Matrix& B, Matrix& C) {
    assert(A.size(1) == B.size(2));
    typedef typename Matrix::value_type value_type;
    size_t Rang = A.size(1);
    mult_block<0, N - 1, 0, M - 1> block;
    for (size_t i = 0; i < Rang; i += N)
    {
        for (size_t j = 0; j < Rang; j += M)
        {
            multi_tmp<N*M, value_type> tmp(value_type{});
            for (size_t k = 0; k < Rang; ++k)
            {
                block(tmp, A, B, i, k, j);
            }
            block.update(tmp, C, i, j);
        }
    }
}
/*!
 *  Умножение матриц методом метапрограммирования. Метод 2
 */

template <size_t N, size_t I, class Closure>
typename boost::enable_if_t<(I == N)> is_meta_loop2(Closure& closure) {}

template <size_t N, size_t I, class Closure>
typename boost::enable_if_t<(I < N)> is_meta_loop2(Closure& closure) {
    closure.template apply<I>();
    is_meta_loop2<N, I + 1>(closure);
}
template <size_t N, class Closure>
void meta_loop2(Closure& closure) {
    is_meta_loop2<N, 0>(closure);
}

/*!
 * struct abstract_sum
 */
template <size_t N, size_t M, typename Matrix_cls>
struct abstr_sum_matrix
{
    typedef typename Matrix_cls::value_type value_type;
    Matrix_cls matrix_cls;
    value_type result;
    abstr_sum_matrix(Matrix_cls& mtrx) : matrix_cls(mtrx), result(value_type()) {}

    template<size_t K>
    void apply() {
        result += matrix_cls.template value<K>();
    }
};
template <size_t N, size_t M, class Matrix_cls>
typename Matrix_cls::value_type abstr_sum(Matrix_cls& matrix_cls) {
    abstr_sum_matrix<N, M, Matrix_cls> my_closure(matrix_cls);
    meta_loop2<N>(my_closure);
    return my_closure.result;
}

template <size_t N, size_t M, typename Matrix>
struct matrix_prod_closure
{
    typedef typename Matrix::value_type value_type;
private:
    Matrix A, B;
public:
    matrix_prod_closure(const Matrix& A, const Matrix& B) : A(A), B(B) {}
    template<size_t K>
    value_type value() const {
        return A(N, K) * B(K, M);
    }
};

template <typename Matrix>
struct result_matrx {
    typedef typename Matrix::value_type value_type;
    Matrix A, B;
    result_matrx(Matrix& A, Matrix& B): A(A), B(B) {}
    template <size_t N, size_t M>
    value_type value() const {
        matrix_prod_closure<N, M, Matrix> closure(A, B);
        return abstr_sum<N, M>(closure);
    }
};

template <size_t i, typename Matrix>
struct A_m
{
private:
    Matrix C, A, B;

public:
    A_m(Matrix& C, const Matrix& A, const Matrix& B): C(C), A(A), B(B){}

    template <size_t j>
    void apply() {
        result_matrx<Matrix> a(A, B);
        C(i, j) = a.template value<i, j>();
    }
};
template <typename Matrix>
struct CALC_A
{
private:
    Matrix C, A, B;
public:
    CALC_A(Matrix& C, const Matrix& A, const Matrix& B): C(C), A(A), B(B){}

    template <size_t i>
    void apply() {
        A_m<i, Matrix> closure(C, A, B);
        meta_loop2<i + 1>(closure);
    }
};
template <size_t M, typename Matrix>
void calc_matrix(Matrix& C, const Matrix& A, const Matrix& B) {
    CALC_A<Matrix> closure(C, A, B);
    meta_loop2<M + 1>(closure);
}
