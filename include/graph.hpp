#include <iostream>
#include <map>
#include <boost/unordered_set.hpp>
#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include <boost/functional/hash.hpp>
#include <cassert>
#include <limits>
#include <algorithm>

#include <array2d.hpp>
#include <matrix_mult.hpp>

template <size_t N = 3, size_t M = 3>
    using my_matrix = array2d<size_t, N, M, std::vector>;

template<typename Vertex, typename Weight>
using node_type = std::pair<Vertex, std::pair<Vertex, Weight> >;

template<typename Vertex, typename Weight>
bool operator == (const node_type<Vertex, Weight>& left, const node_type<Vertex, Weight>& right) {
    return (left.first == right.first && left.second.first == right.second.first
            && left.second.second == right.second.second);
}

namespace boost {
    template<typename Vertex, typename Weight>
    struct hash<node_type<Vertex, Weight>> {
        size_t operator() (const node_type<Vertex, Weight>& pair) const {
            size_t hash = 0;
                boost::hash_combine(hash, boost::hash_value(pair.first));
                boost::hash_combine(hash, boost::hash_value(pair.second.first));
                boost::hash_combine(hash, boost::hash_value(pair.second.second));
            return hash;
        }
    };
}

template<typename Vertex, typename Weight, size_t N, size_t M>
class Graph;

template<typename Vertex, typename Weight, size_t N, size_t M>
std::ostream& operator << (std::ostream&, Graph<Vertex, Weight, N, M> const&);

template<typename Vertex, typename Weight, size_t N, size_t M>
class Graph
{
    public:
        constexpr static Weight _inf = std::numeric_limits<Weight>::infinity();
    private:
        boost::container::vector<Weight> node_matrix;
        boost::unordered_set<node_type<Vertex, Weight>> m_graph;
        array2d<Weight, N, M, boost::container::vector> adjacency_matrix;
        boost::bimap<Vertex, Weight> neighbor_path;
        boost::bimap<Vertex, Vertex> previos_neighbor;
    public:
        Graph(){}
        ~Graph(){}

        void add_edge(Vertex const& start, Vertex const& finish, const Weight& weight)
        {
            if(start == finish)
            {
                m_graph.insert({start, std::make_pair(finish, 0)});
                m_graph.insert({finish, std::make_pair(start, 0)});
                node_matrix.push_back(0);
//                node_matrix.push_back(0);
            }
            else if(start != finish && weight != 0)
            {
                m_graph.insert({start, std::make_pair(finish, weight)});
                m_graph.insert({finish, std::make_pair(start, weight)});
                node_matrix.push_back(weight);
//                node_matrix.push_back(weight);
            }
            else {
                m_graph.insert({start, std::make_pair(finish, _inf)});
                m_graph.insert({finish, std::make_pair(start, _inf)});
                node_matrix.push_back(_inf);
//                node_matrix.push_back(_inf);
            }
        }
        void init_adj_matr() {
            for(size_t i = 0; i < adjacency_matrix.size(1); ++i) {
                for(size_t j = 0; j < adjacency_matrix.size(2); ++j) {
                    if(i == j ) adjacency_matrix.at(i,j) = 0;
                    else if(i != j && node_matrix.at(i) != 0) adjacency_matrix.at(i,j) = node_matrix.at(i);
                    else adjacency_matrix.at(i,j) = _inf;
                }
            }
//            adjacency_matrix.init_list(node_matrix);
        }

        void print_on(std::ostream& strm) const
        {
            strm << this->adjacency_matrix <<std::endl;
        }

        friend std::ostream& operator << <Vertex, Weight, N, M> (std::ostream& ost, Graph<Vertex, Weight, N, M> const& grp);
		
};
template<typename Vertex, typename Weight, size_t N, size_t M>
std::ostream& operator << (std::ostream& ost, Graph<Vertex, Weight, N, M> const& grp)
{
    grp.print_on(ost);
	return ost;
}
