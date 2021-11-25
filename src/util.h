#ifndef _UTIL_H
#define _UTIL_H
#include <string>
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <queue>

class Node{
public:
  std::string name;
  int w, h;
  int x, y;
  int legal_x, legal_y;
  int type; //0:movable, 1:fixed node, 2:fixed node, but can overlap
//-------function-----------------
  Node(std::string n, int w, int h, int t): name(n), w(w), h(h), type(t){}
  bool operator < (const Node& n2);
  void PrintNode();
  int NodeAbs();
};

class Cluster{
public:
  std::vector<int> nodes_index;
  int lb_x, weight, width, q;
//-------function-----------------
  Cluster();
  void AddCell(Node& node, int n_index);
};

bool compare_node (Node lhs, Node rhs);
struct classcomp {
  bool operator() (const Node& lhs, const Node& rhs) const
    {return lhs.x < rhs.x;}
};

#endif