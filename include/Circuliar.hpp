#pragma once
#include <vector>
#include <typeinfo>

#include <boost/container/vector.hpp>

#include <array2d.hpp>

template<typename T>
struct is_const
{
    static const bool value = false;
};

template<typename T>
struct is_const<const T>
{
    static const bool value = true;
};

template<bool Condition, typename ThenType, typename ElseType>
struct conditional
{
    using type = ThenType;
};

template<typename ThenType, typename ElseType>
struct conditional<false, ThenType, ElseType>
{
    using type = ElseType;
};



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
private:
    using vref_type = typename conditional<is_const<Matrix>::value, const value_type&, value_type&>::type;
public:
    explicit transposed_view(Matrix& A): ref(A) {}

    vref_type operator()(size_type r, size_type c) { return ref(c, r); }
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
