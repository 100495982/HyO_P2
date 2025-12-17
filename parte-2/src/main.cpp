// main.cpp
#include "algoritmo.hpp"
#include "grafo.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: ./parte2 VERTICE1 VERTICE2 NOMBRE_MAPA SALIDA" << std::endl;
        std::cerr << "Ejemplo: ./parte2 1 309 NY solucion.txt" << std::endl;
        return 1;
    }

    VertexID start = std::stoul(argv[1]);
    VertexID goal = std::stoul(argv[2]);
    std::string mapa = argv[3];      // "NY"
    std::string salida_file = argv[4];

    // Construir nombres de archivo DIMACS
    std::string arcos_file = "USA-road-d-arcos." + mapa + ".txt";
    std::string coords_file = "USA-road-d-verticces." + mapa + ".txt";

    std::cout << "Cargando: " << arcos_file << " + " << coords_file << std::endl;

    Grafo grafo;
    grafo.loadGraph(arcos_file, coords_file);

    std::cout << grafo.getNumVertices() << " vertices, " 
              << grafo.getNumEdges() << " arcos" << std::endl;

    Algoritmo algoritmo;
    
    // Resolver con A* (heurística principal)
    SolucionAStar resultado = algoritmo.solveAStar(grafo, start, goal);

    // Escribir salida EXACTAMENTE como enunciado
    std::ofstream salida(salida_file);
    if (!resultado.path.empty()) {
        for (size_t i = 0; i < resultado.path.size(); ++i) {
            if (i > 0) salida << " - ";
            salida << resultado.path[i];
            if (i < resultado.path.size() - 1) {
                salida << " - " << resultado.costs[i];
            }
        }
    }
    salida.close();

    // Imprimir EXACTAMENTE formato enunciado
    std::cout << "Solucion optima encontrada con coste " 
              << resultado.total_cost << std::endl;
    std::cout << resultado.expansion_count << " expansiones" << std::endl;
    std::cout << "Tiempo de ejecucion " << resultado.elapsed << " segundos" << std::endl;

    return 0;
}
