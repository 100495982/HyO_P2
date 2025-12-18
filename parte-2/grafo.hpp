// grafo.hpp
#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "tipos.hpp"

#include <string_view>
#include <unordered_map>
#include <vector>

class Grafo {
private:
    // Internal storage (indexed by [0..N-1])
    std::vector<std::vector<Edge>> adjacency_list;
    std::vector<Vertex> vertices;

    // Map external VertexID (DIMACS id) -> internal index
    std::unordered_map<VertexID, size_t> vertex_index_map;

    size_t edge_count = 0;

public:
    Grafo() = default;
    ~Grafo() = default;

    void loadGraph(std::string_view gr_file, std::string_view co_file);

    const std::vector<Edge>& getAdyacentes(VertexID vertex) const {
        return adjacency_list.at(vertex_index_map.at(vertex));
    }

    const Vertex& getVertex(VertexID vertex) const {
        return vertices.at(vertex_index_map.at(vertex));
    }

    bool hasVertex(VertexID vertex) const {
        return vertex_index_map.find(vertex) != vertex_index_map.end();
    }

    // Enunciado wants number of vertices processed from .co and arcs from .gr [file:1]
    size_t getNumVertices() const { return vertices.size(); }
    size_t getNumEdges() const { return edge_count; }

private:
    void parseGraphFile(std::string_view filename);
    void parseCoordinatesFile(std::string_view filename);
};

#endif // GRAFO_HPP
