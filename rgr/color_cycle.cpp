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

    std::vector<long long> R;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(32500,45000);
    std::cout << std::endl;
    long long r;
    for (auto& vertex : V) {
        r = dist(gen); // Generate random r
        //std::cout << "r (before modification) = " << r << std::endl;
        if (vertex.second == 'r') {
            r &= ~3; 
        } else if (vertex.second == 'b') {
            r &= ~3; 
            r |= 1;  
        } else if (vertex.second == 'y') {
            r &= ~3; 
            r |= 2; 
        }

        std::cout << "r (after modification) = " << r << std::endl;
        R.push_back(r);
    }

    std::vector<long long> P,Q,C,D,N;
    long long p,q,c,d,n,phi;

    for (const auto& vertex : V) {
        do {
            do {
                //q = generateRandomPrime();
                q = dist(gen);
            } while (!millerRabinTest(q, 100));
            p = 2 * q + 1;
        } while (!millerRabinTest(p, 100));
        n = p * q;
        phi = (p - 1) * (q - 1);
        std::cout << "q = " << q << std::endl;
        std::cout << "p = " << p << std::endl;
        std::cout << "n = " << n << std::endl;
        std::cout << "phi = " << phi << std::endl;
        Q.push_back(q);
        P.push_back(p);
        N.push_back(n);
        auto keys = generate_rsa_keys(phi);
        c = keys.first.first; 
        d = keys.second.first;
        C.push_back(c);
        D.push_back(d);
        std::cout<<"C = "<<c<<" D = "<<d<<std::endl;
    }

    std::cout<<std::endl;
    std::vector<long long> Z;
    long long z;
    for (size_t i = 0; i < R.size(); ++i) {
        r = R[i];
        d = D[i];
        n = N[i];
        std::cout<<"R = "<<r<<" d= "<<d<<" n="<<n<<std::endl;
        z = pow_module(r, d, n); 
        if(z<0){
            z = z + n;
        }
        std::cout << "z = " << z << " for vertex " << V[i].first << std::endl;
        Z.push_back(z); 
    }

}