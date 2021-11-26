#ifndef _UTIL_H
#define _UTIL_H
#include <string>
#include <iostream>
#include <vector>

enum class NodeType : uint8_t {
  kBlock = 0,
  kCore = 1
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
  friend ostream& operator<<(ostream& os, const Node& n);
};

#endif