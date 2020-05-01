#include <array2d.hpp>
#include <function_compose.hpp>
//#include <matrix_initialised.hpp>
#include <wave_equation.hpp>
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
//    multyplyMatrix mtrx;
//    mtrx.print_result_thread();
//    mtrx.print_result_async();
//    std::vector<int> vec{2, 2, 2, 2, 2};

//    std::complex<char> cmpl1('W', 'Q');
//    std::complex<char> cmpl2('Q', 'Z');
//    Graph<std::complex<char>, int, 5, 5> grp0;
//    grp0.add_edge(cmpl1.real(), cmpl2.real(), 300);
//    grp0.add_edge(cmpl2.real(), cmpl1.real(), 700);
//    grp0.init_vec();
//    std::cout << grp0 << std::endl;

    Hyperbolic_equation<double, 5, 5, decltype (handler)> ret(16., .0, .0);
    ret.equation(handler);
//    equation_meta<int, 5, 5, decltype (handler)> equationn;
//    equationn.equation_resolv(0, 0, handler);

//    Graph<char, int, 5, 5> grp;
//    grp.add_edge('A', 'B', 3);
//    grp.add_edge('A', 'C', 7);
//    grp.add_edge('A', 'F', 8);
//    grp.add_edge('B', 'C', 23);
//    grp.add_edge('B', 'D', 5);

//    grp.init_vec();
//    std::cout << grp << std::endl;

//    array2d<size_t, 5, 5, std::vector>   A_5;
//    A_5.fill(2);
//    std::cout << trans(A_5) << std::endl;
    auto comp = Compose(func, gung, gungq);
    std::cout << call(comp, 1) << std::endl;
    std::cout << call(gungs, 1, 2, 3) << std::endl;
    auto val = Compose([](int const a) { return std::to_string(a); }, [](int const a) { return a*a; }, gung, gungq)(4);
    std::cout << val << std::endl;
    auto p = Curried(1)(2)(3);
    std::cout << p << std::endl;
    std::cout << PartialFunctionAdd3(1, 2, 3) << std::endl;
    std::cout << PartialFunctionAdd3(1, 2)(3) << std::endl;
    std::cout << PartialFunctionAdd3(1)(2)(3) << std::endl;
    boost::function<void(int)> lambprint([=](int i) {  std::cout << call(comp, i) << std::endl; });
    lambprint(1);
    boost::function<void(int, int, int)> gangsprint([=](int x, int y, int z) {  std::cout << call(gungs, x, y, z) << std::endl; });
    gangsprint(1, 2, 3);
    boost::function<int(int, int, int)> sprint([=](int x, int y, int z) {  return  call(gungs, x, y, z); });
    std::cout << sprint(1, 2, 3) << std::endl;;

    const PipableClosure<AddOne> fnclosure {};
    int value = 1 | fnclosure | fnclosure;
    std::cout << value << std::endl;
    const PipableClosureBinary<SumFunc> sumclosure = {};
    int value1 = 1 | sumclosure(2) | sumclosure(3) | fnclosure;
    std::cout << value1 << std::endl;
	return 0;
}
