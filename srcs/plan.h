#ifndef _LEGALIZER_H
#define _LEGALIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <cstdlib>
#include "util.h"
#include "../clipper/clipper.hpp"

class Plan {
public:
  int node_num_, terminal_num_, partition_num_;
  std::vector<Node> nodes_;
  std::map<std::string, int> node_idx_;
  std::vector<ClipperLib::Path> partitions_;
//------------function----------------------
  void readAux(std::string aux_file);
  void readPl(std::string pl_file);
  void readNode(std::string node_file);
  void readPartition(std::string partiton_file);
  void printNodes();
  void printLines();
  void outputPl();
  void outputGDT(std::string gdt_file);
  void checkPartitionsRectilinear();
};

#endif