// grafo.cpp
void Grafo::loadGraph(std::string_view gr_file, std::string_view co_file) {
    // 1. Parse coordinates FIRST (smaller file)
    parseCoordinatesFile(co_file);

    // 2. Pre-allocate adjacency list
    adjacency_list.resize(vertices.size());

    // 3. Parse graph with buffered I/O
    parseGraphFile(gr_file);
}

void Grafo::parseGraphFile(std::string_view filename) {
    std::ifstream file(filename.data(), std::ios::binary);
    file.rdbuf()->pubsetbuf(nullptr, 65536);  // 64KB buffer

    std::string line;
    while (std::getline(file, line)) {
        if (line[0] != 'a') continue;  // Skip non-arc lines

        VertexID id1, id2;
        Distance cost;
        std::sscanf(line.c_str(), "a %u %u %lu", &id1, &id2, &cost);

        // Expand if needed
        while (id1 >= adjacency_list.size()) {
            adjacency_list.resize(adjacency_list.size() * 2);
        }
        while (id2 >= adjacency_list.size()) {
            adjacency_list.resize(adjacency_list.size() * 2);
        }

        adjacency_list[id1].push_back({id2, cost});
        edge_count++;
    }
}
