// grafo.hpp
#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "tipos.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

class Grafo {
private:
    std::vector<std::vector<Edge>> adjacency_list;
    std::vector<Vertex> vertices;
    std::unordered_map<VertexID, size_t> vertex_index_map;

    size_t edge_count = 0;
    VertexID max_vertex_id = 0;

public:
    // Constructor & file I/O
    Grafo() = default;
    ~Grafo() = default;

    // Load graph from DIMACS files
    void loadGraph(std::string_view gr_file, std::string_view co_file);

    // Accessors
    inline const std::vector<Edge>& getAdyacentes(VertexID vertex) const;
    inline const Vertex& getVertex(VertexID vertex) const;
    inline bool hasVertex(VertexID vertex) const;

    // Statistics
    inline size_t getNumVertices() const { return vertices.size(); }
    inline size_t getNumEdges() const { return edge_count; }

private:
    void parseGraphFile(std::string_view filename);
    void parseCoordinatesFile(std::string_view filename);
    void expandAdjacencyList(VertexID vertex_id);
};

#endif // GRAFO_HPP
