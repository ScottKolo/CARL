#include <queue>
#include <vector>
#include <iostream>

#include "CARL.hpp"

namespace CARL { // Begin namespace CARL

Graph coarsen_heavy_edge(Graph graph)
{
    boost::graph_traits<Graph>::vertices_size_type n_vertices = 
        num_vertices(graph);
    std::vector<boost::graph_traits<Graph>::vertex_descriptor> 
        match(n_vertices, boost::graph_traits<Graph>::null_vertex());
    // TODO: Should not have to initialize vector to null_vertex
    // TODO: May be able to use iterator_property_map for speedup

    std::vector<Graph> graphs;
    graphs.push_back(graph);

    heavy_edge_matching(graph, &match[0]);

    /*
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for(boost::tie(vi,vi_end) = vertices(graph); vi != vi_end; ++vi)
    {
        if (match[*vi] != 0 && *vi < match[*vi])
        {
            std::cout << "{" << *vi << ", " << match[*vi] << "}" << std::endl;
        }
    }
    */
    //std::cout << "TEST!" << std::endl;
    // Matching done, need to coarsen
    graphs.push_back(coarsen(graphs[0], match));
    //std::cout << "TEST END!" << std::endl;

    return graph;
}

} // End namespace CARL