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
    std::cout << p << std::endl;
    inter_cell_num += p.cell_num_;
  }
  std::cout << "cell: " << inter_cell_num << " / " << plan.nodes_.size() << std::endl;
}