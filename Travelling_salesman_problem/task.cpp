#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const int INF = INT_MAX;

// Функция для вычисления нижней оценки веса остатка пути
int lowerBound(vector<vector<int>>& graph, vector<bool>& visited, int current) {
    int n = graph.size();
    int min1 = INF, min2 = INF;
    for (int i = 0; i < n; ++i) {
        if (!visited[i] && graph[current][i] < min1) {
            min2 = min1;
            min1 = graph[current][i];
        } else if (!visited[i] && graph[current][i] < min2) {
            min2 = graph[current][i];
        }
    }
    return min1 + min2;
}

// Функция поиска в глубину с учётом веса добавляемой дуги и нижней оценки веса остатка пути
int depthFirstSearch(vector<vector<int>>& graph, vector<bool>& visited, int current, int start, int& totalWeight) {
    visited[current] = true;
    if (current == start && visited.size() == graph.size()) {
        return graph[current][start];
    }
    int n = graph.size();
    for (int i = 0; i < n; ++i) {
        if (!visited[i] && graph[current][i] < lowerBound(graph, visited, i)) {
            totalWeight += graph[current][i];
            int weight = depthFirstSearch(graph, visited, i, start, totalWeight);
            if (weight != -1) {
                return weight;
            }
            totalWeight -= graph[current][i];
        }
    }
    return -1;
}

int main() {
    int n;
    cout << "Enter the number of vertices: ";
    cin >> n;
    vector<vector<int>> graph(n, vector<int>(n, INF));
    cout << "Enter the adjacency matrix: \n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> graph[i][j];
        }
    }
    vector<bool> visited(n, false);
    int totalWeight = 0;
    int start = 0; // Начальная вершина
    int weight = depthFirstSearch(graph, visited, start, start, totalWeight);
    if (weight != -1) {
        cout << "The weight of the most efficient tour is " << weight << endl;
    } else {
        cout << "No tour found" << endl;
    }
    return 0;
}
