#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <limits>
#include <algorithm>

template<typename Vertex, typename Weight>
class Graph;

template<typename Vertex, typename Weight>
std::ostream& operator << (std::ostream&, Graph<Vertex, Weight> const&);

template<typename Vertex, typename Weight>
class Graph
{
	public:
		using node_type = std::pair<Vertex, Weight>;
		using node_matrix_type = std::vector<node_type>;
		using map_vertex = std::map<Vertex, Weight>;
		using map_weight = std::map<Vertex, Vertex>;
	private:
		std::map<Vertex, node_matrix_type> adjacency_matrix;
		map_vertex neighbor_path;
		map_vertex previos_neighbor;
	public:
		Graph(){}
		~Graph(){}
		constexpr static Weight _inf = std::numeric_limits<Weight>::infinity();
		void add_edge(Vertex const& start, Vertex const& finish, Weight const& weight)
		{
			adjacency_matrix[start].push_back(std::make_pair(finish, weight));
			adjacency_matrix[finish].push_back(std::make_pair(start, weight));
		}
		size_t node_count() const
		{
			return adjacency_matrix.size();
		}	
		std::vector<Vertex> nodes() const
		{
			std::vector<Vertex> v;
			for(auto const& [a, b] : adjacency_matrix) v.push_back(a);
			return v;
		}
		node_matrix_type const& neighbors(Vertex const& v) const
		{
			auto position = adjacency_matrix.find(v);
			if (position == adjacency_matrix.end())
			{
				throw std::runtime_error("not node");
			}
			return position->second;
		}
		void Dijkstra(Vertex const& start)
		{
			neighbor_path.clear();
			for(auto const& v : nodes()) neighbor_path[v] = _inf;
			neighbor_path[start] = 0;
			std::set<std::pair<Weight, Vertex>> node_set;
			node_set.insert(std::make_pair(neighbor_path[start], start));
			for(auto const& [distance, start_node] : node_set)
			{
				for(auto const& [neighbor_x, neighbor_y] : neighbors(start_node)) {
					if((distance + neighbor_y) < (neighbor_path[neighbor_x]))
					{
						node_set.erase(std::make_pair(neighbor_path[neighbor_x], neighbor_x));
						neighbor_path[neighbor_x] = distance + neighbor_y;
						previos_neighbor[neighbor_x] = start_node;
						node_set.insert(std::make_pair(neighbor_path[neighbor_x], neighbor_x));
					}
				}
			}
		}
		void print_on(std::ostream& strm) const
		{
			for(auto const& [x, y] : adjacency_matrix)
			{
				
				for (auto const& [u, v] : y)
				{
					strm << "{" << x << " -> ";
					if(x != u) strm << u << "} = " << v << std::endl;
				}
				strm <<std::endl;
			}
		}
		friend std::ostream& operator << <Vertex, Weight> (std::ostream& ost, Graph<Vertex, Weight> const& grp);
		
};
template<typename Vertex, typename Weight>
std::ostream& operator << (std::ostream& ost, Graph<Vertex, Weight> const& grp)
{
	grp.print_on(ost);
	return ost;
}
std::cout

int main()
{
	Graph<char, int> grp;
	grp.add_edge('A', 'B', 3);
	grp.add_edge('A', 'C', 7);
	grp.add_edge('A', 'F', 8);
	grp.add_edge('B', 'C', 23);
	grp.add_edge('B', 'D', 4);
	char start = 'A';
	grp.Dijkstra(start);
	std::cout << grp << std::endl;
	return 0;
}