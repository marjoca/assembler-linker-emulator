#include "../inc/sekcija.hpp"
#include<iostream>

using namespace std;


ostream &operator<<(ostream &os, const Sekcija sek)
{

    
    os<<"|"<<sek.rBr<<"|"<<"\t"<<"|"<<"0x"<<std::hex<<sek.size<<"|"<<"\t"<<"|"<<sek.type<<"|"<<"\t"<<"|"<<sek.ime<<"|"<<"\n"<<std::dec;//proveriti ovo za HEX
    /*os<<"data"<<"\n";
    
    int i=0;
        for (auto it = sek.data.begin(); it != sek.data.end(); ++it){
            i++;
            os<<(*it);
            if(i%2==0)
                os<<" ";
       
    }
    os<<"\n";
    os<<"bazen"<<"\n";
    for (auto it = sek.bazen.begin(); it != sek.bazen.end(); ++it){
            os<<(*it);
            
       
    }
    os<<"\n";
    os<<"realokaciona tabela"<<"\n";
    for (auto it = sek.realokacionaTabela.begin(); it != sek.realokacionaTabela.end(); ++it){
            os<<(*it);
            
       
    }*/
     
    
    
    return os;
}