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

class Plan {
public:
  int node_num_, terminal_num_, partition_num_, nets_num_, pins_num_;
  std::vector<Node> nodes_;
  std::map<std::string, int> node_idx_;
  std::vector<Partition> partitions_;
  std::vector<Net> nets_;
//------------function----------------------
  void readAux(std::string aux_file);
  void printNodes();
  void printLines();
  void outputPl();
  void outputGDT(std::string gdt_file);
  void checkPartitionsRectilinear();
  void mapCellInPartition();
  void mapNetInPartition();

private:
  void readPl(std::string pl_file);
  void readNode(std::string node_file);
  void readPartition(std::string partiton_file);
  void readNet(std::string net_file);
};

#endif