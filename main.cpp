#include "graph.h"
#include "helper.h"
#include <SDL.h>
#include <chrono>
#include <ctime>
#include <iostream>

using namespace std;

class Timer
{
private:
	chrono::high_resolution_clock::time_point m_start;

public:
	void start()
	{
		m_start = chrono::high_resolution_clock::now();
	}
	uint64_t tick()
	{
		auto end = chrono::high_resolution_clock::now();

		auto duration = chrono::duration_cast<chrono::microseconds>(end - m_start).count();
		m_start = end;

		return duration;
	}
};

void handleInput();

bool running = true;
const int graphSize = 50;

int addedVertices = 0;

// first vertex in vertex addition mode
int firstVertex = -1;

// A star mode
bool inAStarMode = false;
int startVertexAStar = -1;

vector<int> shortestPath;

uint8_t density = 10;

bool addVertexEvent(const SDL_Event &e);
bool addEdgeEvent(const SDL_Event &e);
void handleKeyboardInput(const SDL_Event &e);
void handleMouseButtonDown(const SDL_Event &e);
bool addVertexToAStarEvent(const SDL_Event &e);
void performAStar(uint32_t end);

Graph<graphSize> g;

int main(int argc, char *args[])
{
	srand(time(nullptr));

	if (!helper::init())
		return 1;

	while (running)
	{
		handleInput();

		helper::clear();
		helper::setColor(0xFF, 0x00, 0x00);

		g.draw();
		g.drawPath(shortestPath);

		helper::present();
		SDL_Delay(10);
	}

	helper::shutdown();
	return 0;
}

void performAStar(uint32_t end)
{
	try
	{
		shortestPath = g.aStarSearch(startVertexAStar, end);
	}
	catch (...)
	{
		startVertexAStar = -1;
		cout << "No path\n";
		return;
	}
	Timer timer;
	timer.start();

	Graph<graphSize>::setHeuristic(HeuristicModes::euclidean);
	g.aStarSearch(startVertexAStar, end);
	cout << "Euclidean: " << timer.tick() << "\n";

	Graph<graphSize>::setHeuristic(HeuristicModes::zero);
	g.aStarSearch(startVertexAStar, end);
	cout << "Djikstra : " << timer.tick() << "\n";

	Graph<graphSize>::setHeuristic(HeuristicModes::xDifference);
	g.aStarSearch(startVertexAStar, end);
	cout << "X difference: " << timer.tick() << "\n";

	Graph<graphSize>::setHeuristic(HeuristicModes::yDifference);
	g.aStarSearch(startVertexAStar, end);
	cout << "Y difference: " << timer.tick() << "\n";

	startVertexAStar = -1;
}

void handleInput()
{
	SDL_Event e;

	// Event loop
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			handleKeyboardInput(e);
			break;
		case SDL_MOUSEBUTTONDOWN:
			handleMouseButtonDown(e);
			break;
		}
	}
}

void handleMouseButtonDown(const SDL_Event &e)
{
	if (!inAStarMode)
	{
		bool addedVertex = addVertexEvent(e);
		if (!addedVertex)
			addEdgeEvent(e);
	}
	else
	{
		addVertexToAStarEvent(e);
	}
}

bool addVertexToAStarEvent(const SDL_Event &e)
{
	shortestPath.clear();

	Point pos = {e.button.x, e.button.y};

	int vertex = g.getNearbyVertex(pos);

	if (vertex == -1)
		return false;

	if (startVertexAStar == -1)
	{
		startVertexAStar = vertex;
		return false;
	}

	performAStar(vertex);

	return true;
}

void handleKeyboardInput(const SDL_Event &e)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_c:
		// clear graph
		g = Graph<graphSize>();
		shortestPath.clear();
		firstVertex = -1;
		addedVertices = 0;
		startVertexAStar = -1;
		break;
	case SDLK_s:
		// switch astar mode
		inAStarMode = !inAStarMode;
		firstVertex = -1;
		startVertexAStar = -1;
		break;
	case SDLK_q:
		break;
	case SDLK_r:
		g = Graph<graphSize>::getRandomGraph(density);
		addedVertices = graphSize;
		shortestPath.clear();
		firstVertex = -1;
		startVertexAStar = -1;
		break;
	}
}

bool addVertexEvent(const SDL_Event &e)
{
	if (addedVertices >= graphSize)
		return false;

	Point pos = {e.button.x, e.button.y};

	if (g.getNearbyVertex(pos) != -1)
		return false;

	char letter = 'A' + addedVertices;
	g.addVertex(letter, pos);
	addedVertices++;

	firstVertex = -1;

	return true;
}

bool addEdgeEvent(const SDL_Event &e)
{
	Point pos = {e.button.x, e.button.y};

	int vertex = g.getNearbyVertex(pos);
	if (vertex == -1)
		return false;

	if (firstVertex == -1)
	{
		firstVertex = vertex;
	}
	else
	{
		g.addEdge(firstVertex, vertex);
		firstVertex = -1;
	}
	return true;
}
