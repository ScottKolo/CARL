#include "CARL.h"
#include <iostream>
#include <vector>

namespace CARL
{

template <class T_E_weight, class T_V_weight>
Graph<T_E_weight, T_V_weight>::Graph()
{

}

template <class T_E_weight, class T_V_weight>
int Graph<T_E_weight, T_V_weight>::degree(int vertex)
{
    return column_ptrs[vertex+1] - column_ptrs[vertex];
}

/* p [0..n] = cumulative sum of c [0..n-1], and then copy p [0..n-1] into c */
template <class T_E_weight, class T_V_weight>
double Graph<T_E_weight, T_V_weight>::cumulative_sum (std::vector<int> &p, std::vector<int> &c, int n)
{
    int nz = 0 ;
    double nz2 = 0 ;
    //if (!p || !c) return (-1) ;     /* check inputs */
    for (int i = 0 ; i < n ; i++)
    {
        p [i] = nz ;
        nz += c [i] ;
        nz2 += c [i] ;              /* also in double to avoid csi overflow */
        c [i] = p [i] ;             /* also copy p[0..n-1] back into c[0..n-1]*/
    }
    p [n] = nz ;
    return (nz2) ;                  /* return sum (c [0..n-1]) */
}

template <class T_E_weight, class T_V_weight>
Graph<T_E_weight, T_V_weight>::Graph(const int nnz, const int n, 
    const std::vector<int> I, const std::vector<int> J, 
    const std::vector<T_E_weight> val)
: column_ptrs(n+1), neighbors(nnz), edge_weights(nnz), num_vertices(n)
{
    int p;
    std::vector<int> w(n,0);
    for (int k = 0 ; k < nnz ; k++)
    {
        w [J[k]]++ ;           /* column counts */
    }

    cumulative_sum (column_ptrs, w, n) ;    /* column pointers */

    for (int k = 0 ; k < nnz ; k++)
    {
        neighbors [p = w[J[k]]++] = I[k] ;    /* A(i,j) is the pth entry in C */
        edge_weights[p] = val[k] ;
    }
}

template <class T_E_weight, class T_V_weight>
bool Graph<T_E_weight, T_V_weight>::is_directed()
{
    std::vector<int> w(column_ptrs);               /* allocate workspace */
    for (int j = 0; j < num_vertices; j++)
    {
        for (int p = w[j]; p < column_ptrs[j+1]; p++)
        {
            /* Check to make sure we haven't overflowed and that Aij = Aji */
            if ((w [neighbors [p]] < column_ptrs [neighbors [p] + 1]) &&
                (edge_weights [p] == edge_weights [w [neighbors [p]]]) && (j == neighbors[w[neighbors[p]]]))
                w [neighbors [p]] += 1 ;
            else
            {
                /* If the above conditions fail the matrix is not symmetric. */
                return true ;  /* ... and return true. */
            }
        }
    }
    return false ;   /* ... and return false. */
}

template <class T_E_weight, class T_V_weight>
void Graph<T_E_weight, T_V_weight>::print (bool brief)
{
    for (int j = 0 ; j < num_vertices ; j++)
    {
        std::cout << "    col " << j << " : locations " << column_ptrs[j] << 
            " to " << column_ptrs[j+1]-1 << std::endl;
        for (int p = column_ptrs[j]; p < column_ptrs[j+1]; p++)
        {
            std::cout << p << "      " << neighbors[p] << " : " << edge_weights[p] << std::endl;
            if (brief && p > 20)
            {
                std::cout << "  ..." << std::endl;
                return;
            }
        }
    }
}

template <class T_E_weight, class T_V_weight>
int Graph<T_E_weight, T_V_weight>::num_edges()
{
    return column_ptrs[num_vertices];
}

template class Graph<int, int>;
template class Graph<int, double>;
template class Graph<double, int>;
template class Graph<double, double>;

}