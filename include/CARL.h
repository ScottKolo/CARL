#ifndef CARL_H
#define CARL_H

#include <cstddef>
#include <cstdlib>
#include <vector>

namespace CARL
{

template <class T_E_weight, class T_V_weight>
class Graph
{
  public:
    Graph();
    Graph(const int nnz, const int n, const std::vector<int> I, 
        const std::vector<int> J, const std::vector<T_E_weight> val);
    void  print(bool brief=false);
    bool  is_directed();
    int   degree(int vertex);
    Graph coarsen(int coarsen_count = 1);
    Graph coarsen_until(int vertex_count);
    void  coarsen_in_place(int coarsen_count = 1);
    void  coarsen_in_place_until(int vertex_count);
    int   num_edges();

  private:
    int num_vertices;    /* number of vertices */
    //int num_edges;       /* number of edges */
    std::vector<int> column_ptrs;              /* column pointers (size n+1) */
    std::vector<int> neighbors;      /* neighbors, size = num_edges */
    std::vector<T_E_weight> edge_weights;   /* edge weights, size = num_edges
                                 *  = weight of edge (neigh(k), p(neigh(k))) */
    std::vector<T_V_weight> vertex_weights; /* vertex weights, size = num_vertices */
    double cumulative_sum (std::vector<int> &p, std::vector<int> &c, int n);
};

template <class T_E_weight, class T_V_weight>
Graph<T_E_weight, T_V_weight> *CARL_graph_calloc(int vertices, int edges);

void *CARL_malloc (int n, size_t size);
void *CARL_calloc (int n, size_t size);
void *CARL_free (void *p);

#define CARL_MAX(a,b) (((a) > (b)) ? (a) : (b))
#define CARL_MIN(a,b) (((a) < (b)) ? (a) : (b))

}

#endif // CARL_H