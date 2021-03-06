#include "util.h"
using namespace std;

ostream& operator<< (ostream& os, const NodeType& obj) {
  os << static_cast<underlying_type<NodeType>::type>(obj);
  return os;
}

ostream& operator<< (ostream& os, const Node& n) {
  os << n.name_ << " " << n.id_ << " " << n.w_ << " " << n.h_ << " (" << n.x_ << "," << n.y_ << ") " << n.type_ << " pa: " << n.partition_idx_ << endl;
  return os;
}

ostream& operator<< (ostream& os, const Partition& p) {
  os << "partition id: " << p.id_ << endl;
  os << "center_p: " << p.center_p_ << endl;
  os << p.contour_;
  os << "cell_num: " << p.cell_num_ << " inter_cell_num: " << p.inter_cell_num_ << 
    " inter_net_num = " << p.inter_nets_idx_.size() << endl;
  return os;
}

Node::Node () {
  type_ = NodeType::kCore;
}