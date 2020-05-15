
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

struct IGraph {
    virtual ~IGraph() {}
    
    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;
    
    virtual int VerticesCount() const  = 0;
    
    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph: public IGraph {
public:
    explicit ListGraph(size_t vertices_count) : vertices(vertices_count) {
    }
    
    ListGraph(const IGraph& graph) {
    }

    
    virtual ~ListGraph() {}
    
    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override {
        vertices[from].push_back(to);
    }
    
    int VerticesCount() const override {
        return vertices.size();
    }
    
    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> result;
        
        result.resize(vertices[vertex].size());
        std::copy(vertices[vertex].begin(), vertices[vertex].end(), result.begin());
        
        return result;
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> result;
        for (int i = 0; i < vertices.size(); ++i) {
            for (int v : vertices[i]) {
                if (v == vertex) {
                    result.push_back(i);
                }
            }
        }
        return result;
    }
    
private:
    std::vector<std::vector<int>> vertices;
};

void dfs_aux(const IGraph& graph, int vertex, std::vector<bool>& visited, void (*clb)(int)) {
    visited[vertex] = true;
    clb(vertex);
    std::vector<int> next_vertices = graph.GetNextVertices(vertex);
    for (int child : next_vertices) {
        if (!visited[child]) {
            dfs_aux(graph, child, visited, clb);
        }
    }
    
}

void dfs(const IGraph& graph, void (*clb)(int)) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            dfs_aux(graph, i, visited, clb);
        }
    }
}

void bfs(const IGraph& graph, void (*clb)(int)) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> q;
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            visited[i] = true;
            q.push(i);
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                clb(v);
                std::vector<int> next_vertices = graph.GetNextVertices(v);
                for (int child : next_vertices) {
                    if (!visited[child]) {
                        visited[child] = true;
                        q.push(child);
                    }
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    ListGraph graph(7);
    graph.AddEdge(0, 1);
    
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 3);
    graph.AddEdge(2, 4);
    graph.AddEdge(3, 4);
    graph.AddEdge(1, 6);
    graph.AddEdge(4, 2);
    
    dfs(graph, [](int v) {
        std::cout << v << std::endl;
    });
    std::cout << "----------" << std::endl;
    bfs(graph, [](int v) {
        std::cout << v << std::endl;
    });
    return 0;
}
