#ifndef _UTIL_H
#define _UTIL_H
#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include "../clipper/clipper.hpp"
#include <set>

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
  int partition_idx_ = -1;
  std::set<int> nets_idx_;
//-------function-----------------
  Node();
  Node(std::string n, int w, int h, NodeType t): name_(n), w_(w), h_(h), type_(t){}
  friend std::ostream& operator<<(std::ostream& os, const Node& n);
};

class Net {
public:
  std::string name_;
  // idx for terminal partition index
  std::set<int> terminals_idx_;
};

class Partition {
public:
  ClipperLib::Path contour_;
  int cell_num_;
  std::set<int> cells_idx_;
  std::set<int> inter_cells_;
  std::set<int> inter_nets_idx_;
  friend std::ostream& operator<<(std::ostream& os, const Partition& p);
};

#endif