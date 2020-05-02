#pragma once
#include <vector>
#include <typeinfo>
#include <type_traits>

#include <boost/container/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/type_traits/is_same.hpp>

#include <array2d.hpp>

template <typename Matrix>
class transposed_view;

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

/*!
 * Проверка на комплесное число
 *
 */
template<typename T>
struct Magnitude
{
    using type = T;
};
template<typename T>
struct Magnitude<std::complex<T>>
{
    using type = T;
};
template<typename T>
struct Magnitude<std::vector<T>>
{
    using type = T;
};
template<typename T>
struct Magnitude<boost::numeric::ublas::matrix<T>>
{
    using type = T;
};
template<typename T, size_t N, size_t M>
struct Magnitude<array2d<T, N, M, boost::container::vector>>
{
    using type = T;
};
/*!
 * Проверка на матрицу
 *
 */
template<typename T>
struct is_matrix : boost::mpl::false_ {};

template<typename T>
struct is_matrix<const T> : is_matrix<T> {};

template<typename T>
struct is_matrix<boost::numeric::ublas::matrix<T>> : boost::mpl::true_ {};

template<typename T, size_t N, size_t M>
struct is_matrix<array2d<T, N, M, boost::container::vector>> : boost::mpl::true_ {};

template<typename Matrix>
struct is_matrix<transposed_view<Matrix>> : is_matrix<Matrix> {};

template<typename T>
struct is_blast_matrix : boost::mpl::false_ {};

template<typename T>
struct is_blast_matrix<boost::numeric::ublas::matrix<T>> : boost::mpl::true_ {};

/*!
 * Проверка на вектор
 *
 */
template<typename T>
struct is_vector : boost::mpl::false_ {};

template<typename T>
struct is_vector<const T> : is_vector<T> {};

template<typename T>
struct is_vector<std::vector<T>> : boost::mpl::true_ {};

template<typename T>
struct is_vector<boost::container::vector<T>> : boost::mpl::true_ {};
/*!
 *
 *
 */
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
    static_assert (is_matrix<Matrix>::value, "Must be matrix");

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

/*!
 * Норма матрицы
 *
 */

template<typename T>
boost::enable_if_t<is_matrix<T>::value, typename Magnitude<T>::type>
inline one_norm(const T& A) {
    typename Magnitude<T>::type max{0};
    if constexpr (!is_blast_matrix<T>::value) {
        for(size_t i = 0; i < A.size(1); ++i) {
            typename Magnitude<T>::type sum{0};
            for(size_t j = 0; j < A.size(2); ++j) {
                sum += std::abs(A(i, j));
            }
            max = max < sum ? sum : max;
        }
    } else {
        for(size_t i = 0; i < A.size1(); ++i) {
            typename Magnitude<T>::type sum{0};
            for(size_t j = 0; j < A.size2(); ++j) {
                sum += std::abs(A(i, j));
            }
            max = max < sum ? sum : max;
        }
    }
    return max;
}

template<typename T>
boost::enable_if_t<is_vector<T>::value, typename Magnitude<T>::type>
inline one_norm(const T& A) {
    typename Magnitude<T>::type sum{0};
    for(size_t j = 0; j < A.size(); ++j) {
        sum += std::abs(A[j]);
    }
    return sum;
}
///Сумма вариативная

template <typename ...Args> struct sum_type {};
template <typename T> struct sum_type<T> {
    using type = T;
};
template <typename T, typename ...Args> struct sum_type<T, Args...> {
    using type = decltype (T() + typename sum_type<Args...>::type());
};
template <typename ...Args>
using sum_type_t = typename sum_type<Args...>::type;
