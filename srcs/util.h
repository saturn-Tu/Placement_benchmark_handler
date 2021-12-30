#ifndef _UTIL_H
#define _UTIL_H
#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include "../clipper/clipper.hpp"
#include <unordered_set>
#include <utility>
#include <boost/functional/hash.hpp>

enum class NodeType : char {
  kBlock = 'B',
  kCore = 'C'
};

class Node {
public:
  std::string name_;
  int id;
  int w_, h_;
  int x_, y_;
  NodeType type_;
  int partition_idx_ = -1;
  std::unordered_set<int> nets_idx_;
  std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>> > pins_position;
//-------function-----------------
  Node();
  Node(std::string n, int w, int h, NodeType t): name_(n), w_(w), h_(h), type_(t){}
  friend std::ostream& operator<<(std::ostream& os, const Node& n);
};

class Net {
public:
  std::string name_;
  int id;
  // idx for terminal partition index
  std::unordered_set<int> terminals_idx_;
};

class Partition {
public:
  ClipperLib::Path contour_;
  int id;
  int cell_num_;
  ClipperLib::IntPoint center_p;
  std::unordered_set<int> cells_idx_;
  std::unordered_set<int> inter_cells_;
  std::unordered_set<int> inter_nets_idx_;
  friend std::ostream& operator<<(std::ostream& os, const Partition& p);
};

#endif