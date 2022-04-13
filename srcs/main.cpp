#include "plan.h"

int main(int argc, char** argv){
  Plan plan;
  plan.readAux(argv[1]);
  plan.mapCellInPartition();
  plan.mapNetInPartition();
  //plan.printNodes();
  plan.outputGDT("output.gdt");
  int inter_cell_num = 0;
  for(auto& p:plan.partitions_) {
    inter_cell_num += p.cell_num_;
  }
  std::cout << "cell: " << inter_cell_num << " / " << plan.macro_idx_2_node_idx_.size() << " / " << plan.nodes_.size() << std::endl;
  std::cout << "net: " << plan.inter_nets_.size() << " / " << plan.nets_.size() << std::endl;
  plan.outputPAFile2NCTUGR();
  std::cout << "partition area:\n";
  for(auto& pa : plan.partitions_) {
    std::cout << -(int)ClipperLib::Area(pa.contour_) << std::endl;
  }
  std::cout << "\npartition stdCell area:\n";
  for(auto& pa : plan.partitions_) {
    std::cout << pa.cell_area_ << std::endl;
  }
}