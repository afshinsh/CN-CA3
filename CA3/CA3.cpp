#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

#define V 4

int minDistance(int dist[], bool sptSet[])
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

int printSolution(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i] == INT_MAX ? -1: dist[i]);
    return 0;
}


void dijkstra(int graph[V][V], int src)
{
    int dist[V]; 
    vector<vector<int>> paths;
    bool sptSet[V]; 
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX, sptSet[i] = false;
        vector<int> vertices;
        paths.push_back(vertices);
    }
        

    dist[src] = 0;
    int iterCount = 1;
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                paths[]
            }
        cout << "iter " << iterCount << ":\n";
        printSolution(dist, V);
        cout << "---------------------------" << endl;
        iterCount++;

    }

}

int main()
{
    int graph[V][V] = { { 0, 19, 9, 0 },
                        { 19, 0, 8, 3 },
                        { 9, 0, 0, 0 },
                        { 0, 3, 0, 0 } };

    dijkstra(graph, 0);

    return 0;
}