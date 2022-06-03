#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include<string>  
#include <sstream>  


using namespace std;

const string TOPOLOGY_CMD = "topology";
const string SHOW_CMD = "show";
const string LSRP_CMD = "lsrp";
const string DVRP_CMD = "dvrp";
const string MODIFY_CMD = "modify";
const string REMOVE_CMD = "remove";


int V = 4;

vector<int> bellmanFord(vector<vector<int>> graph, int src, int V)
{
    vector<int> dist(V);
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;

    dist[src] = 0;
    for (int k = 0; k < V - 1; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (graph[i][j] == 0)
                    continue;

                if (dist[i] != INT_MAX && dist[i] + graph[i][j] < dist[j])
                    dist[j] = dist[i] + graph[i][j];
            }
        }
    }
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (graph[i][j] == 0)
                continue;
            
            if (dist[i] != INT_MAX && dist[i] + graph[i][j] < dist[j])
                cout << "Graph contains negative weight cycle" << endl;
           
        }
    }
    
    return dist;
}


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


void split_str(string const& str, const char delim,
    vector <string>& out)
{
    stringstream s(str);

    string s2;
    while (getline(s, s2, delim))
    {
        if (s2 == "")
            continue;
        out.push_back(s2); 
    }
}


struct Edge {
    int src;
    int dest;
    int cost;
};

vector<Edge> ParseEdges(vector<string> edges) {
    vector<Edge> parsedEdges;
    for (int i = 1; i < edges.size(); i++) {
        vector <string> out;
        split_str(edges[i], '-', out);
        Edge edge;
        edge.src = stoi(out[0]);
        edge.dest = stoi(out[1]);
        edge.cost = stoi(out[2]);
        parsedEdges.push_back(edge);
    }

    return parsedEdges;
}

int SetV(vector<Edge> edges) {
    int maxSrc = INT_MIN;
    int maxDest = INT_MIN;

    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].src > maxSrc)
            maxSrc = edges[i].src;
        if (edges[i].dest > maxDest)
            maxDest = edges[i].dest;
    }

    return max(maxSrc, maxDest);
}


vector<vector<int>> CreateGraph(vector<Edge> edges) {
    vector<vector<int>> graph;
    for (int i = 0;i < V; i++) {
        vector<int> row;
        for (int j = 0; j < V; j++)
            row.push_back(0);
        graph.push_back(row);
    }

    for (int i = 0; i < edges.size(); i++) {
        Edge edge = edges[i];
        graph[edge.src - 1][edge.dest - 1] = edge.cost;
        graph[edge.dest - 1][edge.src - 1] = edge.cost;
    }

    return graph;
}


int main()
{
    vector<vector<int>> graph; /*{{0, 19, 9, 0},
    //                              { 19, 0, 4, 3 },
    //                              { 9, 4, 0, 18 },
    //                              { 0, 3, 18, 0 } };*/

    //vector<int> row1;
    //row1.push_back(0);
    //row1.push_back(19);
    //row1.push_back(9);
    //row1.push_back(0);
    //graph.push_back(row1);

    //vector<int> row2;
    //row2.push_back(19);
    //row2.push_back(0);
    //row2.push_back(4);
    //row2.push_back(3);
    //graph.push_back(row2);

    //vector<int> row3;
    //row3.push_back(9);
    //row3.push_back(4);
    //row3.push_back(0);
    //row3.push_back(18);
    //graph.push_back(row3);

    //vector<int> row4;
    //row4.push_back(0);
    //row4.push_back(3);
    //row4.push_back(18);
    //row4.push_back(0);
    //graph.push_back(row4);

    //lsrp(graph, 0);
    //vector<int> distance = bellmanFord(graph, 2, 4);
    //cout << "Vertex Distance from Source" << endl;
    //for (int i = 0; i < distance.size(); i++)
    //    cout << i << "\t\t" << distance[i] << endl;
   
    string cmd;
    while (getline(cin, cmd)) {
        

        vector <string> out;  
        split_str(cmd, ' ', out);
         
        if (out[0] == TOPOLOGY_CMD) {
            vector<Edge> edges = ParseEdges(out);
            V = SetV(edges);
            graph = CreateGraph(edges);
        }
        else if (out[0] == SHOW_CMD) {

        }
        else if (out[0] == MODIFY_CMD) {

        }
        else if (out[0] == REMOVE_CMD) {

        }
        else if (out[0] == LSRP_CMD) {

        }
        else if (out[0] == DVRP_CMD) {

        }
        else {
            cout << "Command Not Found!" << endl;
        }


    }

    return 0;
}