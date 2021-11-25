#include "legalizer.h"

int main(int argc, char** argv){
  Legalizer legalizer;
  legalizer.ReadAux(argv[1]);
  legalizer.OutputPl();
}