#include "../inc/ulazUBazen.hpp"

ostream &operator<<(ostream &os, const UlazUBazen ub){
  

  
  std::stringstream stream;
stream <<std::setfill('0') << std::setw(8) << std::hex << ub.vrednost;
std::string result( stream.str() );
const int length = result.length();
    char* char_array = new char[length + 1];
    strcpy(char_array, result.c_str());
for(int i=length-1;i>=1;i-=2){
  
  os<<char_array[i-1]<<char_array[i];
}
    
    os<<"\t"<<ub.bitskaSirina<<"\t"<<ub.lokacija<<"\n";
    //os<<ub.vrednost<<" "<<ub.bitskaSirina<<" "<<ub.lokacija<<endl;
    return os;
  }