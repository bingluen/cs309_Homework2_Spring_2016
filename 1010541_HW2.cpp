/**
 * CS309 Homework
 * 1010541 莊秉倫
 * Is connected graph having cycle ?
 */

#include <iostream>
#define undirected true
using namespace std;

void DFS(int node, int &numOfNode, bool*& traveled, bool*& graph, bool*& path);
void printAdjMatrix(bool*& matrix, int x, int y);
int numEdgeNotPath(int numOfNode, bool*& edge, bool*& path);
bool isConnectedGraph(int numOfNode, bool*& traveled);
bool isCycleMoreThan2(bool*& path, bool*& edge, int numOfNode = 0, int numOfEdgeNotPath = 0);
bool findCyclePath(int start, int end, int numOfNode, bool*& path, bool* cyclePath, bool* traveled = 0);

int main(int argc, char *argv[]) {

  int numOfNode = 0;

  /**
   * Read number of node
   */
  cin >> numOfNode;

  /**
   * Allocate memory
   */
  bool *traveled = new bool[numOfNode];
  bool *graph = new bool[numOfNode * numOfNode];
  bool *path = new bool[numOfNode * numOfNode];

  /**
   * initial memory
   */
  for (int i = 0; i < numOfNode; i++) {
    traveled[i] = false;
    for (int j = 0; j < numOfNode; j++) {
      graph[i * numOfNode + j] = path[i * numOfNode + j] = false;
    }
  }

  /**
   * Read graph
   */
  int fromNode = 0, toNode = 0;
  while (fromNode != -1) {
    cin >> fromNode;
    if (fromNode > 0) {
      cin >> toNode;
      if (undirected) {
        int row = (fromNode <= toNode) ? fromNode : toNode,
          column = (row == fromNode) ? toNode : fromNode;
        graph[(row - 1) * numOfNode + column - 1] = true;
      } else {
        graph[(fromNode - 1) * numOfNode + toNode - 1] = true;
      }
    }
  }

  DFS(0, numOfNode, traveled, graph, path);
  bool isConnected = isConnectedGraph(numOfNode, traveled);
  int numOfEdgeNotPath = numEdgeNotPath(numOfNode, graph, path);

  if (isConnected && numOfEdgeNotPath >= 2) {
    cout << "y" << endl;
    if (numOfEdgeNotPath >= 3 || isCycleMoreThan2(path, graph, numOfNode, numOfEdgeNotPath)) {
      cout << "There are at least three cycles." << endl;
    }
  } else {
    cout << "n" << endl;
  }

  /**
   * Free memory
   */
  delete [] traveled;
  delete [] graph;
  delete [] path;

  /**
   * Debug
   *
  cout << "[Debug] Print traveled array" << endl;
  printAdjMatrix(traveled, numOfNode, 1);
  cout << "[Debug] Print graph array" << endl;
  printAdjMatrix(graph, numOfNode, numOfNode);
  cout << "[Debug] Print path array" << endl;
  printAdjMatrix(path, numOfNode, numOfNode);
  cout << "[Debug] the graph is connected " << (isConnected ? "True" : "False") << endl;
  cout << "[Debug] edge is not path = " << numOfEdgeNotPath << endl;
  */
  return 0;
}

void DFS(int node, int &numOfNode, bool*& traveled, bool*& graph, bool*& path) {
  //cout << "[Debug] travel " << node + 1 << endl;
  traveled[node] = true;
  for (int i = 0; i < numOfNode; i++) {
    bool hasPath = false;
    int row = (node <= i) ? node: i,
      column = (row == node) ? i : node;
    if (undirected) {
        hasPath = graph[row * numOfNode + column];
    } else {
      hasPath = graph[(node) * numOfNode + i];
    }
    if (!traveled[i] && hasPath) {
      path[row * numOfNode + column] = true;
      DFS(i, numOfNode, traveled, graph, path);
    }
  }
}

void printAdjMatrix(bool*& matrix, int x, int y) {

  cout << "[Debug] \t";
  for (int i = 0; i < y; i++) {
    cout << i + 1 << "\t";
  }
  cout << endl;

  for (int i = 0; i < x; i++) {
    cout << "[Debug] " << i + 1 << "\t";
    for (int j = 0; j < y; j++) {
      cout << matrix[ i * y + j] << "\t";
    }
    cout << endl;
  }
}

int numEdgeNotPath(int numOfNode, bool*& edge, bool*& path) {
  int count = 0;
  for (int i = 0; i < numOfNode; i++) {
    for (int j = 0; j < numOfNode; j++) {
      if (edge[i * numOfNode + j] && !path[i * numOfNode + j]) {
        count++;
      }
    }
  }
  return count;
}

bool isConnectedGraph(int numOfNode, bool*& traveled) {
  for (int i = 0; i < numOfNode; i++) {
    if (!traveled[i]) return false;
  }
  return true;
}

bool isCycleMoreThan2(bool*& path, bool*& edge, int numOfNode, int numOfEdgeNotPath) {
  if (numOfNode == 0 || numOfEdgeNotPath == 0) return false;
  bool* cyclePath = new bool[2 * numOfNode];

  for (int i = 0; i < 2 * numOfNode; i++) {
    cyclePath[i] = false;
  }

  for (int i = 0; i < numOfNode && numOfEdgeNotPath > 0; i++) {
    for (int j = 0; j < numOfNode && numOfEdgeNotPath > 0; j++) {
      if (edge[i * numOfNode + j] && !path[i * numOfNode + j]) {
        findCyclePath(i, j, numOfNode, path, &(cyclePath[(2 - numOfEdgeNotPath) * numOfNode]));
        numOfEdgeNotPath--;
      }
    }
  }

  //printAdjMatrix(cyclePath, numOfNode * 2, 1);

  int numOfCommonNode = 0;
  for (int i = 0 ; i < numOfNode; i++) {
    if (cyclePath[i] && cyclePath[numOfNode + i])
    {
      numOfCommonNode++;
    }
  }

  delete [] cyclePath;

  if (numOfCommonNode >= 2) return true;
  else return false;
}

bool findCyclePath(int start, int end, int numOfNode, bool*& path, bool* cyclePath, bool* traveled) {
  if (start == end) { cyclePath[start] = true; return true; };

  if (traveled == 0) {
    traveled = new bool[numOfNode];
    for (int i = 0; i < numOfNode; i++) { traveled[i] = false; }
  }

  traveled[start] = true;

  for (int i = 0; i < numOfNode; i++) {
    int row = (start <= i) ? start : i,
      column = (row == start) ? i : start;
    if (!traveled[i] && path[row * numOfNode + column]) {
      bool isPath = findCyclePath(i, end, numOfNode, path, cyclePath, traveled);
      if (isPath) {
        cyclePath[start] = true;
        return true;
      }
    }
  }
  return false;
}
