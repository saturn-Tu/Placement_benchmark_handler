#include "util.h"
using namespace std;

std::ostream& operator<< (std::ostream& os, const NodeType& obj) {
  os << static_cast<std::underlying_type<NodeType>::type>(obj);
  return os;
}

ostream& operator<<(ostream& os, const Node& n)
{
  os << n.name_ << " " << n.w_ << " " << n.h_ << " (" << n.x_ << "," << n.y_ <<") " << n.type_ << endl;
  return os;
}
