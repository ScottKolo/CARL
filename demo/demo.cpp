#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include "CARL.h"

#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <string>
#include <tuple>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

using namespace CARL;

enum class MatrixMarket_Object   { kMatrix = 'M' };
enum class MatrixMarket_Format   { kArray = 'A', kCoordinate = 'C' };
enum class MatrixMarket_Field    { kReal = 'R', kInteger = 'I', kComplex = 'C', kPattern = 'P' };
enum class MatrixMarket_Symmetry { kGeneral = 'G', kSymmetric = 'S', kSkewSymmetric = 'K', kHermitian = 'H' };

std::tuple<MatrixMarket_Object,
           MatrixMarket_Format,
           MatrixMarket_Field,
           MatrixMarket_Symmetry> 
           read_matrix_market_header (std::string header_line);

MatrixMarket_Object parse_MatrixMarket_Object(std::string object_string);
MatrixMarket_Format parse_MatrixMarket_Format(std::string format_string);
MatrixMarket_Field parse_MatrixMarket_Field(std::string field_string);
MatrixMarket_Symmetry parse_MatrixMarket_Symmetry(std::string symmetry_string);

std::tuple<unsigned long, unsigned long> parse_input_line(std::string line);

boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> read_graph (std::string filename);

int main (int argc, char *argv[])
{
    std::string str(argv[1]);

    read_graph(str);
    
    // Check if Graph is valid
    // Check if matrix is symmetric

    // Ignore diagonal - self edges are irrelevant

    // Coarsen until 50 or fewer nodes
    //Graph<double,double> coarse_graph_50_vertices = graph.coarsen_until(50);

    // Or coarsen 5 times
    //Graph<double,double> coarse_graph_5_times = graph.coarsen(5);
    
    // Or coarsen just once
    //Graph<double,double> coarse_graph = graph.coarsen();

    // Report graph data before
    //graph.print(true);

    // Report graph data after
    //coarse_graph.print();

    std::cout << "Demo Complete!" << std::endl;

    return 0;
}

boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> read_graph (std::string filename)
{
    std::ifstream file_in (filename);
    std::string line;

    // Read Header
    std::getline(file_in, line);
    auto typecode = read_matrix_market_header(line);
    std::cout << "Typecode: " 
              << (char)std::get<0>(typecode) 
              << (char)std::get<1>(typecode)
              << (char)std::get<2>(typecode) 
              << (char)std::get<3>(typecode) << std::endl;

    while (std::getline(file_in, line))
    {
        if (line.length() < 1 || line[0] == '%' || line[0] == ' ')
        {
            // Skip any empty or commented lines
            continue;
        }
        else
        {
            break;
        }
    }
    
    // Parse rows, columns, and line number data
    unsigned long rows, cols, num_lines;
    boost::tokenizer<> tokenized_line(line);
    boost::tokenizer<>::iterator line_iterator = tokenized_line.begin();
    rows      = std::stoul(*(line_iterator++));
    cols      = std::stoul(*(line_iterator++));
    num_lines = std::stoul(*(line_iterator++));

    std::cout << "Rows: " << rows << ", Columns: " << cols << ", Lines: " << num_lines << std::endl;

    // create a typedef for the Graph type
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

    // Declare a graph object
    Graph graph(rows);
    unsigned long row, column;

    while (std::getline(file_in, line))
    {
        // Parse the input line
        std::tie(row, column) = parse_input_line(line);

        // Add the edges to the graph object
        boost::add_edge(row, column, graph);
    }

    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    boost::graph_traits<Graph>::adjacency_iterator ni, ni_end;
    for (std::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi)
    {
        std::cout << "Vertex " << *vi << " has neighbors: ";
        for (std::tie(ni,ni_end) = boost::adjacent_vertices(*vi, graph); ni != ni_end; ++ni)
        {
            std::cout << *ni << " ";
        }
        std::cout << std::endl;
    }

    return graph;
}

std::tuple<MatrixMarket_Object,
           MatrixMarket_Format,
           MatrixMarket_Field,
           MatrixMarket_Symmetry> 
           read_matrix_market_header (std::string header_line)
{
    std::vector<std::string> tokenized_header;
    boost::split(tokenized_header, header_line, boost::is_any_of("\t "));

    std::tuple<MatrixMarket_Object,
               MatrixMarket_Format,
               MatrixMarket_Field,
               MatrixMarket_Symmetry> typecode;

    typecode = std::make_tuple(parse_MatrixMarket_Object  (tokenized_header[1]),
                               parse_MatrixMarket_Format  (tokenized_header[2]),
                               parse_MatrixMarket_Field   (tokenized_header[3]),
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

std::tuple<unsigned long, unsigned long> parse_input_line(std::string line)
{
    std::vector<std::string> tokenized_line;
    boost::split(tokenized_line, line, boost::is_any_of("\t "));

    std::tuple<unsigned long, unsigned long> parsed_line;

    parsed_line = std::make_tuple(std::stoul(tokenized_line[0]),
                                  std::stoul(tokenized_line[1]));

    return parsed_line;
}
