#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include<string>  


using namespace std;

int V = 4;

int minDistance(vector<int> dist, vector<bool> sptSet)
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

string createPath(vector<vector<int>> paths, int src, int dest) {
    
    if (paths[dest][0] != src)
        while (paths[dest][0] != src)
        {
            vector<int> tempPath = paths[paths[dest][0]];
            for (int i = tempPath.size() - 1; i >= 0; i--)
                paths[dest].insert(paths[dest].begin(), tempPath[i]);
        }

    string result = "";
    
    for (int i = 0; i < paths[dest].size(); i++) {
        result += to_string(paths[dest][i] + 1) + "->";
    }

    result += to_string(dest + 1);
    return result;
}

void printResult(vector<int> dist, vector<vector<int>> paths, int src) {
    printf("Paths: [s]->[d]  Min-Cost  Shortest Path\n");
    printf("       --------  --------  -------------\n");
    for (int i = 0; i < V; i++) {
        if (i == src)
            continue;
        string path = createPath(paths, src, i);
        printf("       [%d]->[%d]     %d     ", src + 1, i + 1, dist[i]);
        cout << path << endl;

    }
}


void printIteration(vector<int> dist, int n)
{
    printf("Dest\t| ");
    for (int i = 0; i < V; i++)
        printf(" %d |", i + 1);
    printf("\n");
    printf("Cost\t| ");
    for(int i = 0; i < V; i++)
        printf(" %d |", dist[i] == INT_MAX ? -1: dist[i]);
    printf("\n");
}


void lsrp(vector<vector<int>> graph, int src)
{
    vector<int> dist; 
    vector<vector<int>> paths;
    vector<bool> sptSet; 
    for (int i = 0; i < V; i++) {
        dist.push_back(INT_MAX), sptSet.push_back(false);
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
                paths[v].push_back(u);
            }

        cout << "\titer " << iterCount << ":\n";
        printIteration(dist, V);
        cout << "---------------------------" << endl << endl;
        iterCount++;

    }

    printResult(dist, paths, src);

}

int main()
{
    vector<vector<int>> graph; /*{{0, 19, 9, 0},
                        { 19, 0, 4, 3 },
                        { 9, 4, 0, 18 },
                        { 0, 3, 18, 0 } };*/

    vector<int> row1;
    row1.push_back(0);
    row1.push_back(19);
    row1.push_back(9);
    row1.push_back(0);
    graph.push_back(row1);

    vector<int> row2;
    row2.push_back(19);
    row2.push_back(0);
    row2.push_back(4);
    row2.push_back(3);
    graph.push_back(row2);

    vector<int> row3;
    row3.push_back(9);
    row3.push_back(4);
    row3.push_back(0);
    row3.push_back(18);
    graph.push_back(row3);

    vector<int> row4;
    row4.push_back(0);
    row4.push_back(3);
    row4.push_back(18);
    row4.push_back(0);
    graph.push_back(row4);

    lsrp(graph, 0);

    return 0;
}