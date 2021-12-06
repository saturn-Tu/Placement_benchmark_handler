#include "plan.h"

int main(int argc, char** argv){
  Plan plan;
  plan.readAux(argv[1]);
  plan.mapCellInPartition();
  plan.mapNetInPartition();
  //plan.printNodes();
  plan.outputGDT("output.gdt");
  for(auto& p:plan.partitions_) {
    std::cout << p << std::endl;
  }
}