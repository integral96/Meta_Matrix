#pragma once

#include <boost/container/stable_vector.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_constructible.hpp>
#include <boost/shared_ptr.hpp>
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
    explicit array2d(const array2d&);

    template<typename Func, typename = EnableIfFunc<Func>>
    explicit array2d(Func&&);

    explicit array2d(array2d&&);

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
    void swap(array2d& other) noexcept;
public:
    const_iterator cbegin() const { return arr.cbegin(); }
    const_iterator cend() const { return arr.cend(); }
    iterator begin() { return arr.begin(); }
    iterator end() { return arr.end(); }
    friend std::ostream& operator << <T, R, C, Cont> (std::ostream&, const array2d<T, R, C, Cont>&);
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
array2d<T, R, C, Cont>::array2d(const array2d& other) : arr(other.arr)  {
    
}

template<typename T, size_t R, size_t C, template<typename> class Cont>
template<typename Func, typename>
array2d<T, R, C, Cont>::array2d(Func&& other): arr(std::forward<Func>(other)) {

}
template<typename T, size_t R, size_t C, template<typename> class Cont>
array2d<T, R, C, Cont>::array2d(array2d&& other) : arr(std::move(other.arr))  {
    
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
