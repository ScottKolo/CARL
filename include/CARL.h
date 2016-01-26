#ifndef CARL_H
#define CARL_H

#include <boost/graph/adjacency_list.hpp> 

namespace CARL { // Begin namespace CARL

typedef boost::property<boost::edge_weight_t, unsigned long>
        EdgeWeightProperty;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, EdgeWeightProperty>
        Graph;

Graph coarsen_heavy_edge(Graph graph);

template <typename MateMap, typename VertexIndexMap>
inline void heavy_edge_matching(const Graph& g, MateMap mate, VertexIndexMap vm);

template <typename MateMap>
inline void heavy_edge_matching(const Graph& g, MateMap mate);

template <typename MateMap>
class HeavyEdgeMatchingFinder
{
public:
    void find_matching(Graph g, MateMap mate);
};

} // End namespace CARL

#endif // CARL_H