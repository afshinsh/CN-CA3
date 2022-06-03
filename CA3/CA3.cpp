#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include<string>  
#include <algorithm>
#include <chrono>
#include <sstream>  


using namespace std::chrono;
using namespace std;

const string TOPOLOGY_CMD = "topology";
const string SHOW_CMD = "show";
const string LSRP_CMD = "lsrp";
const string DVRP_CMD = "dvrp";
const string MODIFY_CMD = "modify";
const string REMOVE_CMD = "remove";
const string EXIT = "exit";

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

void removeEdge(vector<vector<int>>& graph, vector<string> words) {
    if(words.size() < 1)
        throw string("Error: Bad Input");
    vector <string> out;
    split_str(words[1], '-', out);
    if (out[0] == out[1])
        throw string("Error: Same Source and Destination");
    int u = stoi(out[0]) - 1;
    int v = stoi(out[1]) - 1;
    if (u >= graph.size() || v >= graph.size())
        throw string("Error: Index out of range");

    graph[u][v] = 0;
    graph[v][u] = 0;
}

void modifyGraph(vector<vector<int>>& graph, vector<string> words) {
    if (words.size() < 1)
        throw string("Error: Bad Input");
    vector <string> out;
    split_str(words[1], '-', out);
    if (out[0] == out[1])
        throw string("Error: Same Source and Destination");
    
    int u = stoi(out[0]) - 1;
    int v = stoi(out[1]) - 1;
    if (u >= graph.size() || v >= graph.size())
        throw string("Error: Index out of range");

    graph[u][v] = stoi(out[2]);
    graph[v][u] = stoi(out[2]);
}


void dvrp(vector<vector<int>> graph, int src)
{
    int V = graph.size();
    vector<int> dist(V);
    vector<int> parent(V);
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;

    dist[src] = 0;
    for (int k = 0; k < V - 1; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (graph[i][j] == 0)
                    continue;

                if (dist[i] != INT_MAX && dist[i] + graph[i][j] < dist[j]) {
                    dist[j] = dist[i] + graph[i][j];
                    parent[j] = i;
                }
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
    


    cout << "-------------------------from src " << (src + 1) << " -------------------------" << endl;
    cout << "Dest\tNext Hop\t Dist\tShortest Path" << endl;
    cout << "-------------------------------------------------" << endl;

    vector<int> path;
    for (int i = 0; i < V; i++) {
        int temp = i;
        
        while (temp != src) {
            path.push_back(temp);
            temp = parent[temp];
        }
        path.push_back(src);
        cout << i + 1 << "\t";
        if (path.size() > 1)
            cout << path[path.size() - 2] + 1;
        else
            cout << src+1;
        
        cout << "\t\t" << dist[i] << "\t" << "[";
        for (int j = path.size() -1 ; j >= 0; j--) {
            cout << path[j] + 1;
            if (j != 0)
                cout << " --> ";
        }
        cout << "]" << endl;
        path.clear();
    }
    cout << endl;



}

void executeDVRP(vector<vector<int>> graph, vector<string> words) {

    auto start = high_resolution_clock::now();
    int src;
    if (words.size() > 1)
        dvrp(graph, stoi(words[1]) - 1);
    else
        for (int i = 0; i < graph.size(); i++)
            dvrp(graph, i);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
        << duration.count() << " microseconds" << endl;
}


int V;
bool TopologyCreated = false;


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
        if (edge.src == edge.dest)
            throw string("Same Source and Destination");
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


void showGraph(vector<vector<int>> graph) {
    if (!TopologyCreated)
        throw string("Topology is not created");
    printf("u|v |  ");
    for (int i = 0; i < V; i++)
        printf("%d  ", i + 1);
    cout << endl << endl;;
    for (int i = 0; i <= V; i++)
        printf("----");
    cout << endl << endl;;
    for (int i = 0; i < graph.size(); i++) {
        printf("%d  |  ", i + 1);
        for (int j = 0; j < graph[i].size(); j++) {
            if (i == j)
                printf("0  ");
            else
                printf("%d  ", graph[i][j] == 0 ? -1 : graph[i][j]);

        }
        cout << endl << endl;
        
    }

}
vector<vector<int>> CreateTopology(vector<string> words) {

    /*if (TopologyCreated)
        throw string("topology already created!");*/

    vector<Edge> edges = ParseEdges(words);
    V = SetV(edges);
    vector<vector<int>> graph = CreateGraph(edges);
    TopologyCreated = true;
    cout << "Topology created successfully!" << endl;
    return graph;
}

void executeLSRP(vector<vector<int>> graph, vector<string> words) {

    if (words.size() > 1)
        lsrp(graph, stoi(words[1]) - 1);
    else {
        auto start = high_resolution_clock::now();

        for (int i = 0; i < V; i++) {
            printf("\t --------------from src %d ---------------\n\n", i + 1);
            lsrp(graph, i);
            printf("------------------------------------------------------------------\n\n");

        }
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);
        /*cout << "Time taken by function: "
            << duration.count() << " microseconds" << endl;*/

    }
        

}

int getCommand(string& cmd) {
    cout << "Enter Your Command : ";
    getline(cin, cmd);
    return 1;
}

int main()
{
    vector<vector<int>> graph; 
    string cmd;
    while (getCommand(cmd)) {
        
        try {
            vector <string> words;
            split_str(cmd, ' ', words);

            if (words[0] == TOPOLOGY_CMD)
                graph = CreateTopology(words);
            else if (words[0] == SHOW_CMD)
                showGraph(graph);
            else if (words[0] == MODIFY_CMD) {
                modifyGraph(graph, words);
            }
            else if (words[0] == REMOVE_CMD) {
                removeEdge(graph, words);
            }
            else if (words[0] == LSRP_CMD)
                executeLSRP(graph, words);
            else if (words[0] == DVRP_CMD) {
                executeDVRP(graph, words);
            }
            else if (words[0] == EXIT)
                break;
            else
                cout << "Command Not Found!" << endl;
        }
        catch (string exeption) {
            cout << exeption << endl;
        }
    }

    return 0;
}