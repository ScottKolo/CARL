#include <queue>
#include <vector>
#include <iostream>

#include <boost/graph/max_cardinality_matching.hpp>

#include "CARL.h"

namespace CARL { // Begin namespace CARL

Graph coarsen_heavy_edge(Graph graph)
{
    boost::graph_traits<Graph>::vertices_size_type n_vertices = num_vertices(graph);
    std::vector<boost::graph_traits<Graph>::vertex_descriptor> match(n_vertices);
    edmonds_maximum_cardinality_matching(graph, &match[0]);

    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for(boost::tie(vi,vi_end) = vertices(graph); vi != vi_end; ++vi)
        if (match[*vi] != boost::graph_traits<Graph>::null_vertex() && *vi < match[*vi])
            std::cout << "{" << *vi << ", " << match[*vi] << "}" << std::endl;

    // Matching done, need to coarsen

    return graph;
}

template <typename MateMap, typename VertexIndexMap>
inline void heavy_edge_matching(const Graph& g, MateMap mate, VertexIndexMap vm)
{
    boost::matching  <Graph,
                      MateMap,
                      VertexIndexMap,
                      boost::no_augmenting_path_finder, // Augmenting path finder
                      HeavyEdgeMatchingFinder,   // Initial matching
                      boost::no_matching_verifier>      // Matching verifier
                        (g, mate, vm);
}

template <typename MateMap>
inline void heavy_edge_matching(const Graph& g, MateMap mate)
{
    heavy_edge_matching(g, mate, get(boost::vertex_index,g));
}

template <typename MateMap>
void find_matching(Graph g, MateMap mate)
{

}

} // End namespace CARL