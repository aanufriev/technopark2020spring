#include <iostream>
#include <vector>
#include <algorithm>

struct Edge {
    Edge(int from, int to, int weight)
        : from(from)
        , to(to)
        , weight(weight) {
    }
    
    int from;
    int to;
    int weight;
};


class DSU {
public:
    DSU(size_t size) : parent(size), rank(size) {
        for(size_t i = 0; i < size; ++i) {
            parent[i] = i;
            rank[i] = 1;
        }
    }
    
    int find_set(int v) {
        if (v == parent[v]) {
            return v;
        }
        return parent[v] = find_set(parent[v]);
    }
    
    void union_set(int v, int u) {
        int v_set = find_set(v);
        int u_set = find_set(u);
        
        if (parent[v] < parent[u]) {
            std::swap(v_set, u_set);
        }
        parent[u_set] = v_set;
        rank[v_set] += rank[u_set];
    }
private:
    std::vector<int> parent;
    std::vector<int> rank;
};

void kruskal(std::vector <Edge>& edges, void (*func)(int from, int to)) {
    DSU dsu(edges.size());
    
    std::sort(edges.begin(), edges.end(),
              [](const auto& l, const auto& r) {
                  return l.weight < r.weight;
    });
    
    for (Edge& edge : edges) {
        int v = dsu.find_set(edge.from);
        int u = dsu.find_set(edge.to);
        
        if (v != u) {
            dsu.union_set(v, u);
            func(edge.from, edge.to);
        }
    }
}

int main(int argc, const char * argv[]) {
    std::vector <Edge> edges = {
        { 2, 3, 3 },
        { 1, 6, 4 },
        { 5, 7, 5 },
        { 6, 8, 5 },
        { 7, 8, 5 },
        { 2, 4, 1 },
        { 3, 4, 1 },
        { 1, 3, 2 },
        { 0, 3, 6 },
        { 4, 7, 6 },
        { 3, 6, 7 },
        { 0, 1, 9 },
        { 3, 5, 9 },
        { 5, 6, 1 },
        { 5, 8, 1 },
        { 0, 2, 2 }
    };
    kruskal(edges, [](int v, int u) {
        std::cout << v << " " << u << std::endl;
    });
    return 0;
}
