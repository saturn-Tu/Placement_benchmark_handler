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

class Legalizer {
public:
  int node_num_, terminal_num_, line_num_;
  std::vector<Node> nodes_;
  std::map<std::string, int> node_idx_;
//------------function----------------------
  void readAux(std::string aux_file);
  void readPl(std::string pl_file);
  void readNode(std::string node_file);
  void printNodes();
  void printLines();
  void outputPl();
};

#endif