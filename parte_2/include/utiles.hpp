// utiles.hpp
#ifndef UTILES_HPP
#define UTILES_HPP

#include "tipos.hpp"
#include <string>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;

class Utiles {
public:
    // File path resolution
    static fs::path resolvePath(std::string_view filename);

    // Parsing functions
    static std::pair<std::string, std::string> parseGraphFilenames(
        std::string_view mapname);

    // Coordinate conversion utilities
    static double distanceHaversine(
        Coordinate lat1, Coordinate lon1,
        Coordinate lat2, Coordinate lon2);

    // Output formatting
    static std::string formatPath(
        const std::vector<VertexID>& path,
        const std::vector<Distance>& edge_costs);
};

#endif // UTILES_HPP
