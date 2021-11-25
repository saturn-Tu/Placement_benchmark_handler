#include "util.h"
using namespace std;

void Node::printNode() {
  cout << name_ << " " << w_ << " " << h_ << " (" << x_ << "," << y_ <<") " << type_ << endl;
}

bool Node::operator < (const Node& n2) {
  return this->x_ < n2.x_;
}
