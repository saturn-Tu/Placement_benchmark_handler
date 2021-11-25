#include "util.h"
using namespace std;

void Node::PrintNode() {
  cout << name <<" "<< w <<" "<< h <<" ("<< x <<","<< y <<") "<< "L: ("<< legal_x <<","<< legal_y <<") "<< type << endl;
}

int Node::NodeAbs() {
  return abs(legal_x-x) + abs(legal_y-y);
}

bool Node::operator < (const Node& n2) {
  return this->x < n2.x;
}

Cluster::Cluster() {
  this->q = 0;
  this->weight = 0;
  this->width = 0;
}

void Cluster::AddCell(Node& node, int n_index) {
  this->nodes_index.push_back(n_index);
  this->weight++;
  this->q += this->weight * (node.x - this->width);
  this->width += node.w;
}
