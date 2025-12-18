// main.cpp
#include "algoritmo.hpp"
#include "grafo.hpp"

#include <fstream>
#include <iostream>
#include <string>

static void usage() {
    std::cerr << "Uso: ./parte2 START GOAL MAP.gr MAP.co OUT_FILE\n";
    std::cerr << "Ejemplo: ./parte2 1 309 USA-road-d.BAY.gr USA-road-d.BAY.co solucion.txt\n";
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        usage();
        return 1;
    }

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

    Grafo grafo;
    grafo.loadGraph(gr_path, co_path);

    Algoritmo algoritmo;
    //SolucionAStar resultado = algoritmo.solveAStar(grafo, start, goal);
	//SolucionAStar resultado = algoritmo.solveDijkstra(grafo, start, goal); // optimal brute-force baseline
	//SolucionAStar resultado = algoritmo.solveBFS(grafo, start, goal);    // non-optimal
	//SolucionAStar resultado = algoritmo.solveDFS(grafo, start, goal);    // non-optimal


    // Write path to OUT_FILE in required format: v - cost - v - cost - ... - v [file:1]
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

    // 5-line contract for parte-2.py:
    // 1) vertices processed (.co)
    // 2) arcs processed (.gr)
    // 3) optimal cost
    // 4) expanded nodes
    // 5) elapsed seconds
    std::cout << grafo.getNumVertices() << "\n";
    std::cout << grafo.getNumEdges() << "\n";
    std::cout << resultado.total_cost << "\n";
    std::cout << resultado.expansion_count << "\n";
	std::cout << std::fixed << std::setprecision(6) << resultado.elapsed << "\n";

    return 0;
}
