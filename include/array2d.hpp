#pragma once

#include <boost/container/stable_vector.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_constructible.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

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

///meta mult

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
    static constexpr size_t next_index1 = Index1;
    static constexpr size_t next_index2 = Index2 + 1;
};
template<size_t Index1, size_t MaxIndex1, size_t MaxIndex2>
struct loopback<Index1, MaxIndex1, MaxIndex2, MaxIndex2>
{
    static constexpr size_t next_index1 = Index1 + 1;
    static constexpr size_t next_index2 = 0;
};
///////////////////////////
template<size_t Index1, size_t MaxIndex1, size_t Index2, size_t MaxIndex2>
struct mult_block
{
    using loop = loopback<Index1, MaxIndex1, Index2, MaxIndex2>;
    using next = mult_block<loop::next_index1, MaxIndex1, loop::next_index2, MaxIndex2>;

    template<typename T, typename Matrix>
    void operator()(T& tmp, const Matrix& A, const Matrix& B, size_t i, size_t k, size_t j) {
        tmp.value += A(i + Index1, k) * B(k, j + Index2);
        next()(tmp.sub, A, B, i, k, j);
    }

    template<typename T, typename Matrix>
    void update(const T& tmp, Matrix& C, size_t i, size_t j) {
        C(i + Index1, j + Index2) = tmp.value;
        next().update(tmp.sub, C, i, j);
    }
};

template<size_t Index1, size_t MaxIndex1, size_t MaxIndex2>
struct mult_block<Index1, MaxIndex1, MaxIndex2, MaxIndex2>
{
    using next = mult_block<Index1 + 1, MaxIndex1, 0, MaxIndex2>;

    template<typename T, typename Matrix>
    void operator()(T& tmp, const Matrix& A, const Matrix& B, size_t i, size_t k, size_t j) {
        tmp.value += A(i + Index1, k) * B(k, j + MaxIndex2);
        next()(tmp.sub, A, B, i, k, j);
    }

    template<typename T, typename Matrix>
    void update(const T& tmp, Matrix& C, size_t i, size_t j) {
        C(i + Index1, j + MaxIndex2) = tmp.value;
        next().update(tmp.sub, C, i, j);
    }
};

template<size_t MaxIndex1, size_t MaxIndex2>
struct mult_block<MaxIndex1, MaxIndex1, MaxIndex2, MaxIndex2>
{
    template<typename T, typename Matrix>
    void operator()(T& tmp, const Matrix& A, const Matrix& B, size_t i, size_t k, size_t j) {
        tmp.value += A(i + MaxIndex1, k) * B(k, j + MaxIndex2);
    }

    template<typename T, typename Matrix>
    void update(const T& tmp, Matrix& C, size_t i, size_t j) {
        C(i + MaxIndex1, j + MaxIndex2) = tmp.value;
    }
};

template<size_t N, size_t M, typename Matrix>
void mult_meta(const Matrix& A, const Matrix& B, Matrix& C) {
    assert(A.size(1) == B.size(2));
    using value_type = typename Matrix::value_type;
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
