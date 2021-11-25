#ifndef _LEGALIZER_H
#define _LEGALIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include "util.h"
using namespace std;


class Legalizer{
public:
    int node_num, terminal_num, line_num;
    vector<Node> nodes;
    map<string,int> node_idx;
    vector<vector<Line>>lines;
//------------function----------------------
    void ReadAux(string aux_file);
    void ReadPl(string pl_file);
    void ReadScl(string scl_file);
    void ReadNode(string node_file);
    void PrintNodes();
    void PrintLines();
    void DivideLines();
    void Abacus();
    void AbacusFast();
    int FastPlace(int l_index, int& sub_line_i, Node& node, int n_index, vector<Node>& nodes);
    void OutputPl();
};

#endif