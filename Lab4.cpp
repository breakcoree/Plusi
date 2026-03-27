#include <iostream>
#include <string>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// Ребро
struct EdgeNode {
    int to;
    int weight;
    EdgeNode* next;
    EdgeNode(int t, int w) {
        to = t;
        weight = w;
        next = nullptr;
    }
};

// Вершина
struct VertexNode {
    string name;
    int id;
    EdgeNode* edges;
    VertexNode* next;
    VertexNode(string n, int i) {
        name = n;
        id = i;
        edges = nullptr;
        next = nullptr;
    }
};

class Graph {
private:
    bool directed;
    VertexNode* HEAD;
    int nextId;

public:
    Graph(bool isDirected) {
        directed = isDirected;
        HEAD = nullptr;
        nextId = 0;
    }

    VertexNode* findVertexNode(string name) {
        VertexNode* v = HEAD;
        while (v) {
            if (v->name == name) return v;
            v = v->next;
        }
        return nullptr;
    }

    VertexNode* findVertexNodeById(int id) {
        VertexNode* v = HEAD;
        while (v) {
            if (v->id == id) return v;
            v = v->next;
        }
        return nullptr;
    }

    bool AddVertex(string name) {
        if (findVertexNode(name)) return false;
        VertexNode* node = new VertexNode(name, nextId++);
        node->next = HEAD;
        HEAD = node;
        return true;
    }

    void addEdgeOneWay(VertexNode* from, int toId, int weight) {
        EdgeNode* e = from->edges;
        while (e) {
            if (e->to == toId) {
                e->weight = weight;
                return;
            }
            e = e->next;
        }
        EdgeNode* ne = new EdgeNode(toId, weight);
        ne->next = from->edges;
        from->edges = ne;
    }

    void AddEdge(string from, string to, int weight) {
        VertexNode* a = findVertexNode(from);
        VertexNode* b = findVertexNode(to);
        if (!a || !b) return;
        addEdgeOneWay(a, b->id, weight);
    }

    void PrintGraph() {
        cout << "Граф (остановки и время):\n";
        VertexNode* v = HEAD;
        while (v) {
            cout << v->name << " -> ";
            EdgeNode* e = v->edges;
            while (e) {
                VertexNode* t = findVertexNodeById(e->to);
                cout << t->name << "(" << e->weight << " мин) ";
                e = e->next;
            }
            cout << endl;
            v = v->next;
        }
    }

    void BFS_Traverse(string start) {
        VertexNode* s = findVertexNode(start);
        if (!s) return;

        unordered_set<int> visited;
        queue<int> q;

        visited.insert(s->id);
        q.push(s->id);

        cout << "BFS: ";
        while (!q.empty()) {
            int id = q.front(); q.pop();
            VertexNode* v = findVertexNodeById(id);
            cout << v->name << " ";

            for (EdgeNode* e = v->edges; e; e = e->next) {
                if (!visited.count(e->to)) {
                    visited.insert(e->to);
                    q.push(e->to);
                }
            }
        }
        cout << endl;
    }

    void DFS_Traverse(string start) {
        VertexNode* s = findVertexNode(start);
        if (!s) return;

        unordered_set<int> visited;
        stack<int> st;
        st.push(s->id);

        cout << "DFS: ";
        while (!st.empty()) {
            int id = st.top(); st.pop();
            if (visited.count(id)) continue;

            visited.insert(id);
            VertexNode* v = findVertexNodeById(id);
            cout << v->name << " ";

            for (EdgeNode* e = v->edges; e; e = e->next) {
                st.push(e->to);
            }
        }
        cout << endl;
    }

    void Dijkstra(string start) {
        VertexNode* s = findVertexNode(start);
        if (!s) return;

        vector<int> ids;
        for (VertexNode* v = HEAD; v; v = v->next)
            ids.push_back(v->id);

        unordered_map<int, int> id2idx;
        for (int i = 0; i < ids.size(); i++)
            id2idx[ids[i]] = i;

        int n = ids.size();
        vector<int> dist(n, 1e9);
        vector<bool> used(n, false);

        dist[id2idx[s->id]] = 0;

        for (int i = 0; i < n; i++) {
            int v = -1;
            for (int j = 0; j < n; j++)
                if (!used[j] && (v == -1 || dist[j] < dist[v]))
                    v = j;

            used[v] = true;

            VertexNode* node = findVertexNodeById(ids[v]);

            for (EdgeNode* e = node->edges; e; e = e->next) {
                int to = id2idx[e->to];
                dist[to] = min(dist[to], dist[v] + e->weight);
            }
        }

        cout << "Кратчайшие пути от " << start << ":\n";
        for (int i = 0; i < n; i++) {
            VertexNode* v = findVertexNodeById(ids[i]);
            cout << v->name << ": " << dist[i] << " мин\n";
        }
    }
};

int main() {

    Graph g(true); // ориентированный граф

    // 20 остановок
    g.AddVertex("Центр");
    g.AddVertex("Парк");
    g.AddVertex("Вокзал");
    g.AddVertex("Университет");
    g.AddVertex("Школа");
    g.AddVertex("Больница");
    g.AddVertex("Магазин");
    g.AddVertex("Стадион");
    g.AddVertex("Мост");
    g.AddVertex("Рынок");
    g.AddVertex("ТЦ");
    g.AddVertex("Площадь");
    g.AddVertex("Аэропорт");
    g.AddVertex("Депо");
    g.AddVertex("Библиотека");
    g.AddVertex("Кинотеатр");
    g.AddVertex("Порт");
    g.AddVertex("ЖК");
    g.AddVertex("Склад");
    g.AddVertex("Озеро");

    // связи (время в минутах)
    g.AddEdge("Центр","Парк",5);
    g.AddEdge("Парк","Вокзал",7);
    g.AddEdge("Центр","Университет",10);
    g.AddEdge("Университет","Школа",4);
    g.AddEdge("Школа","Больница",6);
    g.AddEdge("Больница","Магазин",3);
    g.AddEdge("Магазин","Стадион",8);
    g.AddEdge("Стадион","Мост",5);
    g.AddEdge("Мост","Рынок",6);
    g.AddEdge("Рынок","ТЦ",4);
    g.AddEdge("ТЦ","Площадь",3);
    g.AddEdge("Площадь","Аэропорт",15);
    g.AddEdge("Аэропорт","Депо",10);
    g.AddEdge("Депо","Библиотека",7);
    g.AddEdge("Библиотека","Кинотеатр",5);
    g.AddEdge("Кинотеатр","Порт",9);
    g.AddEdge("Порт","ЖК",6);
    g.AddEdge("ЖК","Склад",4);
    g.AddEdge("Склад","Озеро",8);

    // вывод
    g.PrintGraph();

    // обходы
    g.BFS_Traverse("Центр");
    g.DFS_Traverse("Центр");

    // кратчайшие пути
    g.Dijkstra("Центр");

    return 0;
}
