#include "util.h"
using namespace std;

ostream& operator<< (ostream& os, const NodeType& obj) {
  os << static_cast<underlying_type<NodeType>::type>(obj);
  return os;
}

ostream& operator<< (ostream& os, const Node& n) {
  os << n.name_ << " " << n.w_ << " " << n.h_ << " (" << n.x_ << "," << n.y_ << ") " << n.type_ << " pa: " << n.partition_idx_ << endl;
  return os;
}

Node::Node () {
  type_ = NodeType::kCore;
}