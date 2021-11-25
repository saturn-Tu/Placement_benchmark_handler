#ifndef _UTIL_H
#define _UTIL_H
#include <string>
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <queue>
using namespace std;

class Node{
public:
    string name;
    int w, h;
    int x, y;
    int legal_x, legal_y;
    int type; //0:movable, 1:fixed node, 2:fixed node, but can overlap
//-------function-----------------
    Node(string n, int w, int h, int t): name(n), w(w), h(h), type(t){}
    bool operator < (const Node& n2);
    void PrintNode();
    int NodeAbs();
};

class Cluster{
public:
    vector<int> nodes_index;
    int lb_x, weight, width, q;
//-------function-----------------
    Cluster();
    void AddCell(Node& node, int n_index);
};

class Line{
public:
    int lb_x, lb_y, h, rt_x;
    vector <Cluster> clusters;
//-------function-----------------
    void PrintLine();
    int PlaceRow(Node& node, vector<Node>& nodes, int n_index);
    void AddCluster(Cluster& b_cluster, Cluster& cluster);
    void Collapse(Cluster& cluster, int c_index);
    void SetLb_x(Cluster& cluster);
    bool InSearchRange(const Node node, int range);
};

bool compare_node (Node lhs, Node rhs);
struct classcomp {
    bool operator() (const Node& lhs, const Node& rhs) const
        {return lhs.x < rhs.x;}
};

#endif