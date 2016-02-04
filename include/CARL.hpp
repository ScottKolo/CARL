#ifndef CARL_HPP
#define CARL_HPP

#include <tuple>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/graph_traits.hpp>

namespace CARL
{  // Begin namespace CARL

typedef double EdgeWeight;

typedef boost::property<boost::edge_weight_t, EdgeWeight> EdgeWeightProperty;

typedef boost::adjacency_list<boost::vecS,        // Vertex list data structure
                              boost::vecS,        // Adjacency data structure
                              boost::undirectedS, // Directionality
                              boost::no_property, // Vertex properties
                              EdgeWeightProperty> // Edge properties
                                Graph;

typedef boost::graph_traits<Graph>::vertices_size_type Vertex;

Graph coarsen_heavy_edge(Graph graph);

template <typename MateMap, typename VertexIndexMap>
inline void
heavy_edge_matching(const Graph& g, MateMap mate, VertexIndexMap vm);

template <typename MateMap>
inline void
heavy_edge_matching(const Graph& g, MateMap mate);

template <typename GraphType, typename MateMap>
struct HeavyEdgeMatchingFinder
{
    typedef typename
        boost::graph_traits<GraphType>::vertex_descriptor Vertex;
    typedef typename
        boost::graph_traits<GraphType>::vertex_iterator VertexIterator;
    typedef typename
        boost::graph_traits<GraphType>::edge_descriptor Edge;
    typedef typename
        boost::graph_traits<GraphType>::out_edge_iterator EdgeIterator;
    typedef typename
        boost::graph_traits<GraphType>::adjacency_iterator AdjacencyIterator;
    typedef typename
            boost::property_map<GraphType, boost::edge_weight_t>::const_type 
            EdgeWeightMap;

    // Implementation of heavy edge matching
    static void find_matching(const GraphType& graph, MateMap match)
    {
        VertexIterator vi, vi_end;
        EdgeIterator ei, ei_end;
        Vertex null_vertex = boost::graph_traits<GraphType>::null_vertex();

        EdgeWeightMap edgeWeightMap = get(boost::edge_weight, graph);

        // Iterate over all vertices in the graph
        for (std::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
        {
            // Check if match exists for this vertex already
            if (boost::get(match, *vi) != null_vertex)
            {
                // If match already exists, skip to the next vertex
                continue;
            }

            EdgeWeight max_weight = std::numeric_limits<EdgeWeight>::min();
            EdgeWeight weight;
            Edge candidate_edge;

            for (std::tie(ei, ei_end) = out_edges(*vi, graph); ei != ei_end;
                                                                           ++ei)
            {
                if (match[(target(*ei, graph))] != null_vertex)
                {
                    // If match already exists for the adjacent vertex, skip to
                    // the next vertex
                    continue;
                }
                else
                {
                    // Both vertices (*vi and *adji) are unmatched!
                    // Check if this edge has a higher weight than the
                    // previous candidate.
                    weight = boost::get(boost::edge_weight, graph, *ei);
                    if (weight > max_weight)
                    {
                        max_weight = weight;
                        candidate_edge = *ei;
                    }
                }
            }

            // Choose maximum weight edge and match
            if (max_weight > std::numeric_limits<EdgeWeight>::min())
            {
                // We found a match!
                match[*vi] = target(candidate_edge, graph);
                match[target(candidate_edge, graph)] = *vi;
            }
        }
    }
};

template <typename MateMap, typename VertexIndexMap>
inline void
heavy_edge_matching(const Graph& g, MateMap mate, VertexIndexMap vm)
{
    boost::matching<Graph, MateMap, VertexIndexMap,
                    boost::no_augmenting_path_finder,  // Augmenting path finder
                    HeavyEdgeMatchingFinder,           // Initial matching
                    boost::no_matching_verifier>       // Matching verifier
        (g, mate, vm);
}

template <typename MateMap>
inline void
heavy_edge_matching(const Graph& g, MateMap mate)
{
    heavy_edge_matching(g, mate, get(boost::vertex_index, g));
}

}  // End namespace CARL

#endif  // CARL_HPP