#include <iostream>
#include <map>
#include <boost/unordered_set.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include <boost/functional/hash.hpp>
#include <cassert>
#include <limits>
#include <algorithm>
#include <complex>

#include <array2d.hpp>
#include <matrix_mult.hpp>

template <size_t N = 3, size_t M = 3>
    using my_matrix = array2d<size_t, N, M, std::vector>;

template<typename Vertex, typename Weight>
struct node_type
{
    using type = std::pair<Vertex, std::pair<Vertex, Weight> >;
};
template<typename Vertex, typename Weight>
struct node_type<std::complex<Vertex>, std::pair<std::complex<Vertex>, Weight> >
{
    using type = std::pair<std::complex<Vertex>, std::pair<std::complex<Vertex>, Weight> >;
};

template<typename Vertex, typename Weight>
bool operator == (const node_type<Vertex, Weight>& left, const node_type<Vertex, Weight>& right) {
    return (left.first == right.first && left.second.first == right.second.first
            && left.second.second == right.second.second);
}
template<typename Vertex, typename Weight>
size_t hash_value (const node_type<Vertex, Weight>& pair) {
    size_t hash = 0;
        boost::hash_combine(hash, boost::hash_value(pair.first));
        boost::hash_combine(hash, boost::hash_value(pair.second.first));
        boost::hash_combine(hash, boost::hash_value(pair.second.second));
    return hash;
}

template<typename Vertex, typename Weight>
struct Adjacency
{
    using type = Vertex;
    Vertex start;
    Vertex finish;
    Weight weight;
};
template<typename Vertex, typename Weight>
struct Adjacency<std::complex<Vertex>, Weight>
{
    using type = std::complex<Vertex>;
    std::complex<Vertex> start;
    std::complex<Vertex> finish;
    Weight weight;
};

template<typename Vertex, typename Weight, size_t N, size_t M>
class Graph;

template<typename Vertex, typename Weight, size_t N, size_t M>
std::ostream& operator << (std::ostream&, Graph<Vertex, Weight, N, M> const&);

template<typename Vertex, typename Weight, size_t N, size_t M>
class Graph
{

    private:
        typename node_type<Vertex, Weight>::type tmp;
        boost::unordered_set<node_type<Vertex, Weight>> m_graph;
        array2d<Weight, N, M, boost::container::vector> adjacency_matrix;
        boost::bimap<Vertex, Weight> neighbor_path;
        boost::bimap<Vertex, Vertex> previos_neighbor;

    public:
        constexpr static Weight _inf = std::numeric_limits<Weight>::infinity();
        using by_start_ver = typename boost::multi_index::ordered_non_unique<boost::multi_index::member<Adjacency<Vertex, Weight>,
                        Vertex, &Adjacency<Vertex, Weight>::start>>;

        using by_finish_ver = typename boost::multi_index::ordered_non_unique<boost::multi_index::member<Adjacency<Vertex, Weight>,
                        Vertex, &Adjacency<Vertex, Weight>::finish>>;
        using graph_t = typename boost::multi_index::multi_index_container<Adjacency<Vertex, Weight>,
                        boost::multi_index::indexed_by<by_start_ver, by_finish_ver>>;
        boost::container::vector<Adjacency<Vertex, Weight> > vec_rtt;
                    graph_t graph_index;
    public:
        Graph() {}
        ~Graph(){}

        void add_edge(Vertex const& start, Vertex const& finish, const Weight& weight)
        {
                vec_rtt.push_back({start, start, 0});
                vec_rtt.push_back({finish, finish, 0});

                vec_rtt.push_back({start, finish, weight});

                vec_rtt.push_back({finish, start, weight});
        }


        void init_vec() {
            for(const auto& x : vec_rtt) {
                graph_index.insert(x);
            }
            for(auto& y : (graph_index.template get<1>()))
            {
                if(y.start == y.finish) {
                    adjacency_matrix.push_t(0);
                }
                else adjacency_matrix.push_t(y.weight);
            }
//                adjacency_matrix.sortir();
        }

        void print_on(std::ostream& strm) const
        {
            for(auto& y : (graph_index.template get<1>()))
            {
                if(y.start != y.finish) strm << y.start << " => " << y.finish << ", weight = " << y.weight << std::endl;
            }
            strm << adjacency_matrix << std::endl;
        }

        friend std::ostream& operator << <Vertex, Weight, N, M> (std::ostream& ost, Graph<Vertex, Weight, N, M> const& grp);

};

template<typename Vertex, typename Weight, size_t N, size_t M>
std::ostream& operator << (std::ostream& ost, Graph<Vertex, Weight, N, M> const& grp)
{
    grp.print_on(ost);
	return ost;
}
