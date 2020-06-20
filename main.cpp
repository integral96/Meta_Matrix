#include <array2d.hpp>
#include <function_compose.hpp>
#include <matrix_initialised.hpp>
//#include <wave_equation.hpp>
#include <graph.hpp>

#include <thread>
#include <mutex>

#include <boost/lexical_cast.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>


static auto handler([](int x) {
    return x*x*(5 - x);
});
static auto func([](int x) {
    return x;
});
static auto gung([](int x) {
    return x*x;
});
static auto gungq([](int x) {
    return x*x*(5 - x);
});
static auto Curried([](int x) {
    return [x](int y) {
        return [x, y](int z) {
            return x + y + z;
        };
    };
});
static auto gungs([](int x, int y, int z) {
    return x +y +z;
});

struct AddOne
{
    template<typename T>
    auto operator()(T x) const { return x + 1; }
};

struct SumFunc
{
    template<typename T>
    auto operator()(T x, T y) const { return x + y; }
};

int main() {
    std::cout <<"Данная программа использует шаблонный класс матриц и использует его "
              << "для расчета времени умножения матриц заполненых случайным образом"
                 "применяются методы умножения как классические так и метопрограммирования, "
                 "а так-же сравнивается с классом  boost::numeric::ublas::matrix ."
                 "Так же заполняем графы с помощью этого шаблонного класса матриц и вычисляем норму." << std::endl;
    std::cout <<"Расчет времени умножения матриц в рантайме и времени компиляции: " << std::endl;
    multyplyMatrix mtrx;
    mtrx.print_result_thread();
    mtrx.print_result_async();
//    std::vector<int> vec{2, 2, 2, 2, 2};
    std::cout <<"================================================================ " << std::endl;
    std::cout <<"Расчет Графа от комплексных точек(метод SFINAE): " << std::endl;
    std::complex<char> cmpl1('W', 'Q');
    std::complex<char> cmpl2('Q', 'Z');
    Graph<std::complex<char>, int, 5, 5> grp0;
    grp0.add_edge(cmpl1.real(), cmpl2.real(), 300);
    grp0.add_edge(cmpl2.real(), cmpl1.real(), 700);
    grp0.init_vec();
    std::cout << grp0 << std::endl;


    std::cout <<"================================================================ " << std::endl;
    std::cout <<"Простой Граф: " << std::endl;

    Graph<char, int, 5, 5> grp;
    grp.add_edge('A', 'B', 3);
    grp.add_edge('A', 'C', 7);
    grp.add_edge('A', 'F', 8);
    grp.add_edge('B', 'C', 23);
    grp.add_edge('B', 'D', 5);

    grp.init_vec();
    std::cout << grp << std::endl;

    std::cout <<"================================================================ " << std::endl;
    std::cout <<"Расчет нормы матрицы: " << std::endl;

    array2d<int, 8, 8, boost::container::vector> B_8;
    B_8.fill(10);

    std::cout <<"one norm(B_8) = " << one_norm(B_8) << std::endl;
	return 0;
}
