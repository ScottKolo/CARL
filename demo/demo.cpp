// A simple program that computes the square root of a number
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "CARL.h"
extern "C"{
#include "mmio.h"
}

using namespace CARL;

int main (int argc, char *argv[])
{
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;   
    int i;

    // Get filename of matrix market file from command line
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
        return 0;
    }
    else    
    { 
        if ((f = fopen(argv[1], "r")) == NULL)
        {
            // Problem opening file
            printf("Error: Cannot open file %s\n", argv[1]);
            return 0;
        }
    }

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        return 0;
    }

    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && 
            mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);


    /* reserve memory for matrices */

    std::vector<int> I(2*nz-N);
    std::vector<int> J(2*nz-N);
    std::vector<double> val(2*nz-N);

    // mm_read_mtx_crd_data(f, M, N, nz, I, J, val, matcode);
    // for (int i = 0; i < nz; i++)
    // {
    //     I[i]--;
    //     J[i]--;
    //     val[i] = 1;
    // }

    // if (mm_is_symmetric(matcode) && mm_is_pattern(matcode))
    // {
    //     int *I_temp = (int *) malloc((2*nz-N) * sizeof())    
    // }

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    int n = 0;
    for (i = 0; i < nz; i++)
    {
        if (mm_is_symmetric(matcode) && mm_is_pattern(matcode))
        {
            fscanf(f, "%d %d\n", &I[n], &J[n]);
            I[n]--;  /* adjust from 1-based to 0-based */
            J[n]--;
            val[n] = 1;
            //printf("%d %d %d %f\n", n, I[n], J[n], val[n]);
            n++;
            if (I[n-1] != J[n-1])
            {
                I[n] = J[n-1];
                J[n] = I[n-1];
                val[n] = val[n-1];
                //printf("%d %d %d %f\n", n, I[n], J[n], val[n]);
                n++; 
            }
        }
    }

    if (f != stdin) fclose(f);
    //return 0;
    // Read in matrix market file into Graph

    Graph<double, double> graph(n, N, I, J, val);
    // Check if Graph is valid
    // Check if matrix is symmetric
    std::cout << "Checking to ensure the graph is undirected..." << std::endl;
    int ok = 0;
    //CARL_print(graph, 0);
    if (graph.is_directed())
    {
        std::cout << "CARL Error: Cannot coarsen a directed graph" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Graph is undirected (matrix is symmetric)" << std::endl;
    }

    // Ignore diagonal - self edges are irrelevant

    // Coarsen until 50 or fewer nodes
    //Graph<double,double> coarse_graph_50_vertices = graph.coarsen_until(50);

    // Or coarsen 5 times
    //Graph<double,double> coarse_graph_5_times = graph.coarsen(5);
    
    // Or coarsen just once
    //Graph<double,double> coarse_graph = graph.coarsen();

    // Report graph data before
    graph.print(true);

    // Report graph data after
    //coarse_graph.print();

    printf("Demo Complete!\n");

    return 0;
}