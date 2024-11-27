#include "color_cycle.hpp"

void processGraphFile(const std::string& filename, std::vector<std::pair<int, char>>& V, std::vector<std::pair<int, int>>& E) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Eror open file" << std::endl;
        return;
    }
    int vertex_count, edge_count;
    file >> vertex_count >> edge_count;

    for (int i = 0; i < edge_count; ++i) {
        int u, v;
        file >> u >> v;
        E.emplace_back(u, v);
    }

    std::vector<std::string> colors(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
        file >> colors[i];
    }

    for (int i = 0; i < vertex_count; ++i) {
        V.emplace_back(i + 1, colors[i][0]); 
    }
}

void updateColors(std::vector<std::pair<int, char>>& V) {
    for (auto& vertex : V) {
        if (vertex.second == 'r') {
            vertex.second = 'y';
        } else if (vertex.second == 'b') {
            vertex.second = 'r';
        } else if (vertex.second == 'y') {
            vertex.second = 'b';
        }
    }
}

void test_color_cycle() {
    const std::string filename = "correct_graph.txt"; 
    std::vector<std::pair<int, char>> V;     
    std::vector<std::pair<int, int>> E;       

    processGraphFile(filename, V, E);

    std::cout << "Vertices and color:" << std::endl;
    for (const auto& pair : V) {
        std::cout << "Vertices: " << pair.first << ", Color: " << pair.second << std::endl;
    }

    std::cout << "\nEdges of graph:" << std::endl;
    for (const auto& edge : E) {
        std::cout << "Edge: " << edge.first << " - " << edge.second << std::endl;
    }

    std::cout << "\nAlisa:"<<std::endl;
    updateColors(V);
    for (const auto& pair : V) {
        std::cout << "Vertices: " << pair.first << ", Color: " << pair.second << std::endl;
    }

}