#include "helper.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
struct Point
{
    int x, y;
};

struct Line
{
    Point begin;
    Point end;
};

enum class HeuristicModes
{
    xDifference,
    yDifference,
    euclidean,
    zero,
    last
};

int getDistance(const Point &p1, const Point &p2)
{
    double a = std::abs(p1.x - p2.x);
    a *= a;

    double b = std::abs(p1.y - p2.y);
    b *= b;

    int c = std::sqrt(a + b);

    return c;
}

template <size_t N>
class Graph
{
private:
    int adjMatrix[N][N] = {0};
    uint32_t vertexCount = 0;
    char vertexNames[N] = {0};
    Point coordinates[N] = {};
    static HeuristicModes heuristicMode;

    void dfsUtil(int vertex, std::array<bool, N> &visited);

public:
    static void setHeuristic(HeuristicModes);

    char getVertexName(int vertex);

    void addEdge(uint32_t i, uint32_t j);

    void addVertex(char name, const Point &coordinate);

    void print();

    void draw();

    int getNearbyVertex(const Point &pos);

    void depthFirstSearch(int start);

    void breathFirstSearch(int start);

    bool isConnected();

    std::array<int, N> djikstra(int start);

    int heuristic(int current, int finish);

    std::vector<int> reconstructPath(const std::array<int, N> &cameFrom, int end);

    std::vector<int> aStarSearch(int start, int finish);

    void drawPath(const std::vector<int> &path);

    static Graph getRandomGraph(int density);
};

template <size_t N>
void Graph<N>::setHeuristic(HeuristicModes heuristic)
{
    heuristicMode = heuristic;
}

template <size_t N>
HeuristicModes Graph<N>::heuristicMode = HeuristicModes::euclidean;

template <size_t N>
void Graph<N>::dfsUtil(int vertex, std::array<bool, N> &visited)
{
    std::cout << "Visited vertex: " << vertexNames[vertex] << "\n";
    visited[vertex] = true;
    for (int i = 0; i < N; i++)
    {
        if (adjMatrix[vertex][i] != 0 && !visited[i])
        {
            dfsUtil(i, visited);
        }
    }
    std::cout << "Backtracked to: " << vertexNames[vertex] << "\n";
}

template <size_t N>
char Graph<N>::getVertexName(int vertex)
{
    return vertexNames[vertex];
}

template <size_t N>
void Graph<N>::addEdge(uint32_t i, uint32_t j)
{
    if (i >= vertexCount || j >= vertexCount)
        return;

    if (i == j)
        return;

    int distance = getDistance(coordinates[i], coordinates[j]);

    adjMatrix[i][j] = distance;
    adjMatrix[j][i] = distance;
}

template <size_t N>
void Graph<N>::addVertex(char name, const Point &coordinate)
{
    vertexNames[vertexCount] = name;
    coordinates[vertexCount] = coordinate;
    vertexCount++;
}

template <size_t N>
void Graph<N>::print()
{
    std::cout << "  ";
    for (size_t i = 0; i < N; i++)
    {
        std::cout << vertexNames[i] << " ";
    }
    std::cout << "\n";

    for (size_t i = 0; i < N; i++)
    {
        std::cout << vertexNames[i] << " ";
        for (size_t j = 0; j < N; j++)
        {
            std::cout << adjMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

template <size_t N>
void Graph<N>::draw()
{

    for (size_t i = 0; i < vertexCount; i++)
    {
        helper::drawFilledCircle(coordinates[i].x, coordinates[i].y, 10);
    }

    for (size_t i = 0; i < vertexCount; i++)
    {
        for (size_t j = 0; j < vertexCount; j++)
        {
            if (adjMatrix[i][j] != 0)
            {
                helper::drawLine(coordinates[i].x, coordinates[i].y, coordinates[j].x, coordinates[j].y);
            }
        }
    }
}

template <size_t N>
int Graph<N>::getNearbyVertex(const Point &pos)
{

    for (size_t i = 0; i < vertexCount; i++)
    {
        int xDif = std::abs(coordinates[i].x - pos.x);
        int yDif = std::abs(coordinates[i].y - pos.y);
        if (xDif < 20 && yDif < 20)
        {
            return i;
        }
    }
    return -1;
}

template <size_t N>
void Graph<N>::depthFirstSearch(int start)
{
    std::array<bool, N> visited;
    dfsUtil(start, visited);
}

template <size_t N>
void Graph<N>::breathFirstSearch(int start)
{
    std::queue<int> q;
    std::array<bool, N> visited;

    q.push(start);
    visited[start] = true;

    while (!q.empty())
    {
        int currentVertex = q.front();
        q.pop();

        std::cout << "Visited vertex: " << vertexNames[currentVertex] << "\n";

        for (int i = 0; i < N; i++)
        {
            if (adjMatrix[currentVertex][i] != 0 && !visited[i])
            {
                q.push(i);
                visited[i] = true;
            }
        }
    }
}

template <size_t N>
std::array<int, N> Graph<N>::djikstra(int start)
{
    std::array<int, N> distances;
    std::array<bool, N> visited;

    distances[start] = 0;

    std::fill(distances.begin() + 1, distances.end(), 9999999);
    std::fill(visited.begin(), visited.end(), false);

    while (true)
    {
        // find closest unvisited vertex
        int minDistance = 9999999;
        int closestVertex = 0;
        bool found = false;

        for (size_t i = 0; i < vertexCount; i++)
        {
            if (distances[i] < minDistance && !visited[i])
            {
                closestVertex = i;
                minDistance = distances[i];
                found = true;
            }
        }
        if (!found)
            break;
        found = false;

        visited[closestVertex] = true;

        // update distances to all unvisited adjancent vertices
        for (size_t i = 0; i < vertexCount; i++)
        {
            if (adjMatrix[closestVertex][i] != 0 && !visited[i])
            {
                int newDistance = adjMatrix[closestVertex][i] + distances[closestVertex];
                distances[i] = std::min(distances[i], newDistance);
            }
        }
    }

    return distances;
}

template <size_t N>
int Graph<N>::heuristic(int current, int finish)
{
    switch (heuristicMode)
    {
    case HeuristicModes::zero:
        return 0;
        break;
    case HeuristicModes::euclidean:
        return getDistance(coordinates[current], coordinates[finish]);
        break;
    case HeuristicModes::xDifference:
        return std::abs(coordinates[current].x - coordinates[finish].x);
        break;
    case HeuristicModes::yDifference:
        return std::abs(coordinates[current].y - coordinates[finish].y);
        break;
    default:
        throw std::runtime_error("Incorrect heuristic");
    }
}

template <size_t N>
std::vector<int> Graph<N>::reconstructPath(const std::array<int, N> &cameFrom, int end)
{
    std::vector<int> path;

    path.push_back(end);
    // construct path
    int currentVertex = cameFrom[end];

    while (currentVertex != -1)
    {
        path.push_back(currentVertex);
        currentVertex = cameFrom[currentVertex];
    }

    // print the path
    std::reverse(path.begin(), path.end());

    return path;
}

template <size_t N>
std::vector<int> Graph<N>::aStarSearch(int start, int finish)
{

    const int INF = 999999;

    std::array<int, N> gScore, fScore, cameFrom;

    std::fill(gScore.begin(), gScore.end(), INF);
    std::fill(fScore.begin(), fScore.end(), INF);
    std::fill(cameFrom.begin(), cameFrom.end(), -1);

    std::set<int> openSet;

    openSet.insert(start);

    gScore[start] = 0;
    fScore[start] = heuristic(start, finish);

    while (!openSet.empty())
    {

        // get vertex with the least fScore from openSet
        int min = INF;
        int currentVertex = -1;

        for (int vertex : openSet)
        {
            if (fScore[vertex] < min)
            {
                min = fScore[vertex];
                currentVertex = vertex;
            }
        }

        openSet.erase(currentVertex);

        // if it is the goal vertex
        if (currentVertex == finish)
        {
            return reconstructPath(cameFrom, currentVertex);
        }

        for (size_t i = 0; i < vertexCount; i++)
        {

            // if no vertex between
            if (adjMatrix[currentVertex][i] == 0)
                continue;

            int newGScore = gScore[currentVertex] + adjMatrix[currentVertex][i];

            // if better path is found to vertex "i"
            if (newGScore < gScore[i])
            {
                gScore[i] = newGScore;
                cameFrom[i] = currentVertex;
                fScore[i] = gScore[i] + heuristic(i, finish);
                openSet.insert(i);
            }
        }
    }

    throw std::logic_error("Not path found");
}

template <size_t N>
bool Graph<N>::isConnected()
{
    const int INF = 9999999;
    auto distances = djikstra(0);

    for (int distance : distances)
    {
        if (distance == INF)
        {
            return false;
        }
    }
    return true;
}

template <size_t N>
Graph<N> Graph<N>::getRandomGraph(int density)
{
    Graph<N> g;
    char letter = 'A';

    for (size_t i = 0; i < N; i++)
    {
        Point pos;

        bool goodPos = false;

        while (!goodPos)
        {
            goodPos = true;
            pos.x = rand() % helper::getWidth();
            pos.y = rand() % helper::getHeight();

            for (size_t j = 0; j < i; j++)
            {
                int xDif = std::abs(g.coordinates[j].x - pos.x);
                int yDif = std::abs(g.coordinates[j].y - pos.y);
                if (xDif < 5 || yDif < 5)
                {
                    goodPos = false;
                    break;
                }
            }
        }
        g.addVertex(letter, pos);

        letter++;
    }

    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            if (i == j)
                continue;
            uint8_t choice = rand() % 100;

            if (choice > 100 - density)
            {
                g.addEdge(i, j);
            }
        }
    }

    return g;
}

// draws path given by "path" using vertices of this graph
template <size_t N>
void Graph<N>::drawPath(const std::vector<int> &path)
{
    if (path.empty())
        return;

    helper::setColor(0x00, 0x00, 0xFF);
    Point current = coordinates[path[0]];
    Point next;

    for (size_t i = 1; i < path.size(); i++)
    {
        next = coordinates[path[i]];
        helper::drawLine(current.x, current.y, next.x, next.y);
        current = next;
    }

    helper::setColor(0x00, 0x00, 0xFF);
}
