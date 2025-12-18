// main.cpp
#include "algoritmo.hpp"
#include "grafo.hpp"

#include <fstream>
#include <iostream>
#include <string>

// usage info to know how to run the program
static void usage() {
    std::cerr << "Uso: ./parte2 START GOAL MAP.gr MAP.co OUT_FILE\n";
    std::cerr << "Ejemplo: ./parte2 1 309 USA-road-d.BAY.gr USA-road-d.BAY.co solucion.txt\n";
}

int main(int argc, char* argv[]) {
    // we check the number of arguments we have received
    if (argc != 6) {
        usage();
        return 1;
    }

    // we parse the start and goal IDs
    VertexID start{};
    VertexID goal{};
    try {
        start = static_cast<VertexID>(std::stoul(argv[1]));
        goal  = static_cast<VertexID>(std::stoul(argv[2]));
    } catch (...) {
        std::cerr << "Error: START y GOAL deben ser enteros.\n";
        return 2;
    }

    const std::string gr_path  = argv[3];
    const std::string co_path  = argv[4];
    const std::string out_path = argv[5];

    // loading graph data
    Grafo grafo;
    grafo.loadGraph(gr_path, co_path);


    // here we chose the algorithm to run:
    Algoritmo algoritmo;
    SolucionAStar resultado = algoritmo.solveAStar(grafo, start, goal);
	//SolucionAStar resultado = algoritmo.solveDijkstra(grafo, start, goal); // optimal brute-force baseline
	//SolucionAStar resultado = algoritmo.solveBFS(grafo, start, goal);    // non-optimal
	//SolucionAStar resultado = algoritmo.solveDFS(grafo, start, goal);    // non-optimal


    // we write thee path to OUT_FILE in required format: v - cost - v - cost - ... - v
    std::ofstream out(out_path);
    if (!out) {
        std::cerr << "Error: no se puede abrir OUT_FILE: " << out_path << "\n";
        return 3;
    }

    if (!resultado.path.empty()) {
        for (size_t i = 0; i < resultado.path.size(); ++i) {
            if (i) out << " - ";
            out << resultado.path[i];

            if (i + 1 < resultado.path.size()) {
                out << " - (" << resultado.costs[i] << ")";
            }
        }
        out << "\n";
    }
    out.close();

    // we print the stats to the console
    std::cout << grafo.getNumVertices() << "\n"; // 1) number of vertices processed (.co)
    std::cout << grafo.getNumEdges() << "\n"; // 2) number of arcs processed (.gr)
    std::cout << resultado.total_cost << "\n"; // 3) optimal cost
    std::cout << resultado.expansion_count << "\n"; // 4) number of nodes expanded nodes
	std::cout << std::fixed << std::setprecision(6) << resultado.elapsed << "\n"; // 5)  execution time (seconds)

    return 0;
}
