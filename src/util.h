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
  std::string name_;
  int w_, h_;
  int x_, y_;
  int legal_x_, legal_y_;
  int type_; //0:movable, 1:fixed node, 2:fixed node, but can overlap
//-------function-----------------
  Node(std::string n, int w, int h, int t): name_(n), w_(w), h_(h), type_(t){}
  bool operator < (const Node& n2);
  void printNode();
  int nodeAbs();
};

bool compare_node (Node lhs, Node rhs);
struct classcomp {
  bool operator() (const Node& lhs, const Node& rhs) const
    {return lhs.x_ < rhs.x_;}
};

#endif