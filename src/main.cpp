#include "legalizer.h"

int main(int argc, char** argv){
    Legalizer legalizer;
    legalizer.ReadAux(argv[1]);
    legalizer.DivideLines();
    /*for (auto& l : legalizer.lines) {
        if (l.size()<=1)
            continue;
        cout << "size = " << l.size() << endl; 
        for (auto& sub_l : l)
            sub_l.PrintLine();
        cout << endl;
    }*/
    //legalizer.Abacus();
    legalizer.AbacusFast();
    legalizer.OutputPl();
}