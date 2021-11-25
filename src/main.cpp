#include "legalizer.h"

int main(int argc, char** argv){
  Legalizer legalizer;
  legalizer.readAux(argv[1]);
  legalizer.outputPl();
}