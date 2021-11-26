#ifndef _UTIL_H
#define _UTIL_H
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

enum class NodeType : char {
  kBlock = 'B',
  kCore = 'C'
};

class Node {
public:
  std::string name_;
  int w_, h_;
  int x_, y_;
  NodeType type_;
//-------function-----------------
  Node();
  Node(std::string n, int w, int h, NodeType t): name_(n), w_(w), h_(h), type_(t){}
  friend std::ostream& operator<<(std::ostream& os, const Node& n);
};

#endif