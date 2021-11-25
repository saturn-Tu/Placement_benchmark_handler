#include "util.h"
using namespace std;

void Node::printNode() {
  cout << name_ << " " << w_ << " " << h_ << " (" << x_ << "," << y_ <<") " << 
    "L: (" << legal_x_ << "," << legal_y_ << ") " << type_ << endl;
}

int Node::nodeAbs() {
  return abs(legal_x_-x_) + abs(legal_y_-y_);
}

bool Node::operator < (const Node& n2) {
  return this->x_ < n2.x_;
}
