#include "CARL.h"
#include <stdio.h>

namespace CARL
{

template <class T_E_weight, class T_V_weight>
Graph<T_E_weight, T_V_weight> *CARL_graph_calloc(int vertices, int edges)
{
    Graph<T_E_weight, T_V_weight> *graph = CARL_calloc(1, sizeof(Graph<T_E_weight, T_V_weight>));
    if (!graph) return (NULL);
    graph->num_vertices = vertices;
    graph->num_edges = edges;
    graph->p = (int*) CARL_calloc(vertices+1, sizeof(int));
    graph->neighbors = (int*) CARL_calloc(edges, sizeof(int));
    graph->edge_weights = (double*) CARL_calloc(edges, sizeof(double));
    graph->vertex_weights = (double*) CARL_calloc(vertices, sizeof(double));
    return graph;
}

// cs *cs_spalloc (csi m, csi n, csi nzmax, csi values, csi triplet)
// {
//     cs *A = (cs*) SuiteSparse_calloc (1, sizeof (cs)) ;    /* allocate the cs struct */
//     if (!A) return (NULL) ;                 /* out of memory */
//     A->m = m ;                              /* define dimensions and nzmax */
//     A->n = n ;
//     A->nzmax = nzmax = MONGOOSE_MAX2 (nzmax, 1) ;
//     A->nz = triplet ? 0 : -1 ;              /* allocate triplet or comp.col */
//     A->p = (csi*) SuiteSparse_malloc (triplet ? nzmax : n+1, sizeof (csi)) ;
//     A->i = (csi*) SuiteSparse_malloc (nzmax, sizeof (csi)) ;
//     A->x = values ? (double*) SuiteSparse_malloc (nzmax, sizeof (double)) : NULL ;
//     return ((!A->p || !A->i || (values && !A->x)) ? cs_spfree (A) : A) ;
// }

/* wrapper for malloc */
void *CARL_malloc (int n, size_t size)
{
    return (malloc (CARL_MAX (n,1) * size)) ;
}

/* wrapper for calloc */
void *CARL_calloc (int n, size_t size)
{
    return (calloc (CARL_MAX (n,1), size)) ;
}

/* wrapper for free */
void *CARL_free (void *p)
{
    if (p) free (p) ;       /* free p if it is not already NULL */
    return (NULL) ;         /* return NULL to simplify the use of CARL_free */
}

/* Determines if a sparse matrix A is symmetric */
/* ok variable indicates that the return value is valid */
/* Otherwise, we might return 0 on an error and not know anything went wrong */




}