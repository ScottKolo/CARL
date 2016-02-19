#include <boost/algorithm/string/classification.hpp>         // for is_any_of
#include <boost/algorithm/string/detail/classification.hpp>  // for is_any_ofF
#include <boost/algorithm/string/predicate.hpp>              // for iequals
#include <boost/algorithm/string/split.hpp>                  // for split
#include <boost/concept/usage.hpp>                           // for SinglePa...
#include <boost/iterator/iterator_facade.hpp>                // for operator!=
#include <boost/range/irange.hpp>                            // for integer_...
#include <boost/timer/timer.hpp>
#include <iostream>                                          // for string
#include <iterator>                                          // for istream_...
#include <fstream>
#include <string>   // for basic_st...
#include <tuple>    // for tuple
#include <utility>  // for pair
#include <vector>   // for vector

#include "CARL.hpp"

enum class MatrixMarket_Object
{
    kMatrix = 'M'
};
enum class MatrixMarket_Format
{
    kArray = 'A',
    kCoordinate = 'C'
};
enum class MatrixMarket_Field
{
    kReal = 'R',
    kInteger = 'I',
    kComplex = 'C',
    kPattern = 'P'
};
enum class MatrixMarket_Symmetry
{
    kGeneral = 'G',
    kSymmetric = 'S',
    kSkewSymmetric = 'K',
    kHermitian = 'H'
};

std::tuple<MatrixMarket_Object,
           MatrixMarket_Format,
           MatrixMarket_Field,
           MatrixMarket_Symmetry>
read_matrix_market_header(std::string header_line);

MatrixMarket_Object parse_MatrixMarket_Object(std::string object_string);
MatrixMarket_Format parse_MatrixMarket_Format(std::string format_string);
MatrixMarket_Field parse_MatrixMarket_Field(std::string field_string);
MatrixMarket_Symmetry parse_MatrixMarket_Symmetry(std::string symmetry_string);

CARL::Graph read_graph(std::string filename);

int main(int argc, char *argv[])
{
    const std::string filename(argv[1]);

    CARL::Graph graph = read_graph(filename);
    boost::timer::auto_cpu_timer t;
    CARL::coarsen_heavy_edge(graph);
    // Check if Graph is valid
    // Check if matrix is symmetric

    // Ignore diagonal - self edges are irrelevant

    // Coarsen until 50 or fewer nodes
    // Graph<double,double> coarse_graph_50_vertices = graph.coarsen_until(50);

    // Or coarsen 5 times
    // Graph<double,double> coarse_graph_5_times = graph.coarsen(5);

    // Or coarsen just once
    // Graph<double,double> coarse_graph = graph.coarsen();

    // Report graph data before
    // graph.print(true);

    // Report graph data after
    // coarse_graph.print();

    std::cout << "Demo Complete!" << std::endl;

    return 0;
}

CARL::Graph
read_graph(const std::string filename)
{
    std::ifstream file_in(filename);
    std::string line;

    // Read Header
    std::getline(file_in, line);
    auto typecode = read_matrix_market_header(line);
    std::cout << "Typecode: " 
              << (char)std::get<0>(typecode)
              << (char)std::get<1>(typecode)
              << (char)std::get<2>(typecode)
              << (char)std::get<3>(typecode) << std::endl;

    // Skip commented lines
    while (!file_in.eof() && file_in.peek() == '%')
    {
        std::getline(file_in, line);
    }

    // Read rows, columns, and number of lines
    unsigned long rows, cols, num_lines;
    file_in >> rows >> cols >> num_lines;

    std::cout <<   "Rows: "    << rows 
              << ", Columns: " << cols
              << ", Lines: "   << num_lines << std::endl;

    // Create the graph object
    CARL::Graph graph(rows);
    CARL::Vertex v1, v2;
    CARL::EdgeWeight weight;

    if(std::get<2>(typecode) == MatrixMarket_Field::kPattern)
    {
        // Unweighted graph - assume all edge weights are 1
        while(!file_in.eof())
        {
            file_in >> v1 >> v2;
            if(v1 != v2)
            {
                boost::add_edge(v1, v2, 1, graph);
            }
        }
    }
    else
    {
        // Weighted graph - read in edge weights
        while(!file_in.eof())
        {
            file_in >> v1 >> v2 >> weight;
            if(v1 != v2)
            {
                boost::add_edge(v1, v2, weight, graph);
            }
        }
    }

    return graph;
}

std::tuple<MatrixMarket_Object,
           MatrixMarket_Format,
           MatrixMarket_Field,
           MatrixMarket_Symmetry>
read_matrix_market_header(std::string header_line)
{
    std::vector<std::string> tokenized_header;
    boost::split(tokenized_header, header_line, boost::is_any_of("\t "));

    std::tuple<MatrixMarket_Object,
               MatrixMarket_Format,
               MatrixMarket_Field,
               MatrixMarket_Symmetry> typecode;

    typecode =
        std::make_tuple(parse_MatrixMarket_Object(tokenized_header[1]),
                        parse_MatrixMarket_Format(tokenized_header[2]),
                        parse_MatrixMarket_Field(tokenized_header[3]),
                        parse_MatrixMarket_Symmetry(tokenized_header[4]));

    // TODO: Check Header Validity

    return typecode;
}

MatrixMarket_Object parse_MatrixMarket_Object(std::string object_string)
{
    if (boost::iequals(object_string, "matrix"))
    {
        return MatrixMarket_Object::kMatrix;
    }
    else
    {
        throw "Invalid Matrix Market object type";
    }
}

MatrixMarket_Format parse_MatrixMarket_Format(std::string format_string)
{
    if (boost::iequals(format_string, "array"))
    {
        return MatrixMarket_Format::kArray;
    }
    else if (boost::iequals(format_string, "coordinate"))
    {
        return MatrixMarket_Format::kCoordinate;
    }
    else
    {
        throw "Invalid Matrix Market format type";
    }
}

MatrixMarket_Field parse_MatrixMarket_Field(std::string field_string)
{
    if (boost::iequals(field_string, "real"))
    {
        return MatrixMarket_Field::kReal;
    }
    else if (boost::iequals(field_string, "integer"))
    {
        return MatrixMarket_Field::kInteger;
    }
    else if (boost::iequals(field_string, "complex"))
    {
        return MatrixMarket_Field::kComplex;
    }
    else if (boost::iequals(field_string, "pattern"))
    {
        return MatrixMarket_Field::kPattern;
    }
    else
    {
        throw "Invalid Matrix Market field type";
    }
}

MatrixMarket_Symmetry parse_MatrixMarket_Symmetry(std::string symmetry_string)
{
    if (boost::iequals(symmetry_string, "general"))
    {
        return MatrixMarket_Symmetry::kGeneral;
    }
    else if (boost::iequals(symmetry_string, "symmetric"))
    {
        return MatrixMarket_Symmetry::kSymmetric;
    }
    else if (boost::iequals(symmetry_string, "skew-symmetric"))
    {
        return MatrixMarket_Symmetry::kSkewSymmetric;
    }
    else if (boost::iequals(symmetry_string, "hermitian"))
    {
        return MatrixMarket_Symmetry::kHermitian;
    }
    else
    {
        throw "Invalid Matrix Market symmetry type";
    }
}
