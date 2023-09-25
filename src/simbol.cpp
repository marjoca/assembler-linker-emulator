#include "../inc/simbol.hpp"

ostream &operator<<(ostream &os, const Simbol s)
{

    /*string ispis = "";*/
    os<<"|"<<s.rBr<<"|"<<"\t";
    os<<"|"<<s.ime<<"|"<<"\t"<<"|"<< std::internal << std::setfill('0') << std::setw(8) <<s.vrednost<<"|"<<"\t"<<"|"<<s.sekcija<<"|"<<"\t"<<"|"<<s.tip<<"|"<<"\t"<<"|"<<s.bind<<"|"<<"\t"<<"|"<<s.size<<"|"<<"\n";
    

    
    return os;
}