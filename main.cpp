#include <matrix_initialised.hpp>
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

using namespace boost::multi_index;


int main() {
//    multyplyMatrix mtrx;
//    mtrx.print_result_thread();
//    mtrx.print_result_async();

    std::complex<int> cmpl1(1, 2);
    std::complex<int> cmpl2(4, 3);
    Graph<std::complex<int>, int, 5, 5> grp0;

    Graph<char, int, 5, 5> grp;
    grp.add_edge('A', 'B', 3);
    grp.add_edge('A', 'C', 7);
    grp.add_edge('A', 'F', 8);
    grp.add_edge('B', 'C', 23);
    grp.add_edge('B', 'D', 5);

    grp.init_vec();
    std::cout << grp << std::endl;

	return 0;
}
