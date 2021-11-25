#ifndef _LEGALIZER_H
#define _LEGALIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include "util.h"

class Legalizer{
public:
  int node_num, terminal_num, line_num;
  std::vector<Node> nodes;
  std::map<std::string, int> node_idx;
//------------function----------------------
  void ReadAux(std::string aux_file);
  void ReadPl(std::string pl_file);
  void ReadNode(std::string node_file);
  void PrintNodes();
  void PrintLines();
  void OutputPl();
};

#endif