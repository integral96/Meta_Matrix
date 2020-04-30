#include <array2d.hpp>
#include <matrix_initialised.hpp>
#include <wave_equation.hpp>
#include <graph.hpp>
#include <Circuliar.hpp>
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

int main() {
    multyplyMatrix mtrx;
    mtrx.print_result_thread();
//    mtrx.print_result_async();
    std::vector<int> vec{2, 2, 2, 2, 2};

    std::complex<char> cmpl1('W', 'Q');
    std::complex<char> cmpl2('Q', 'Z');
    Graph<std::complex<char>, int, 5, 5> grp0;
    grp0.add_edge(cmpl1.real(), cmpl2.real(), 300);
    grp0.add_edge(cmpl2.real(), cmpl1.real(), 700);
    grp0.init_vec();
    std::cout << grp0 << std::endl;

    Hyperbolic_equation<double, 5, 5, decltype (handler)> ret(16., .0, .0);
    ret.equation(handler);

    Graph<char, int, 5, 5> grp;
    grp.add_edge('A', 'B', 3);
    grp.add_edge('A', 'C', 7);
    grp.add_edge('A', 'F', 8);
    grp.add_edge('B', 'C', 23);
    grp.add_edge('B', 'D', 5);

    grp.init_vec();
    std::cout << grp << std::endl;

    array2d<size_t, 5, 5, std::vector>   A_5;
    A_5.fill(2);
    std::cout << trans(A_5) << std::endl;

	return 0;
}
