#ifndef ULAZUBAZEN_HPP
#define ULAZUBAZEN_HPP
#include<string>
#include<iostream>
#include<iomanip>

#include <cstring>
using namespace std;
class UlazUBazen{
  public:
  long vrednost;         
  long bitskaSirina; 
  long lokacija;
  string vrednostString;
  string lokacijaString;

  UlazUBazen(long vrednost,long bitskaSirina){
    this->vrednost=vrednost;
    this->bitskaSirina=bitskaSirina;
   
    }
    UlazUBazen(string vrednostString,string lokacijaString){
      this->vrednostString=vrednostString;
    this->lokacijaString=lokacijaString;
    }
  void postaviLokaciju(long lokacija){
    this->lokacija=lokacija;
  }
  long dohvVrednostUlaza(){
    return this->vrednost;
  }
  long dohvLokacijuUlaza(){
    return this->lokacija;
  }
  friend ostream &operator<<(ostream &os, const UlazUBazen ub);
};
#endif