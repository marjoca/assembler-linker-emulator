#include "../inc/ulazURelokTabelu.hpp"
#include<iostream>

using namespace std;


ostream &operator<<(ostream &os, const UlazURelokTabelu u)
{

    
    os<<"|"<<u.offset<<"|"<<"\t"<<"|"<<u.tip_relokacije<<"|"<<"\t"<<"|"<<u.simbol<<"|"<<"\t"<<"|"<<u.addend<<"|"<<"\n";//proveriti ovo za HEX
    
    return os;
}