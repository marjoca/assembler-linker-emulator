#ifndef ULAZURELOKTABELU_HPP
#define ULAZURELOKTABELU_HPP

#include<string>
#include<iostream>
using namespace std;
class UlazURelokTabelu{
public:
string sekcija;
long offset;
string tip_relokacije;
string simbol;
long addend;
string file;
UlazURelokTabelu(string sekcija,long offset,string tip_relokacije,string simbol,long addend){
  this->sekcija=sekcija;
  this->offset=offset;
  this->tip_relokacije=tip_relokacije;
  this->simbol=simbol;
  this->addend=addend;
}
UlazURelokTabelu(string file,string sekcija,long offset,string tip_relokacije,string simbol,long addend){
  this->file=file;
  this->sekcija=sekcija;
  this->offset=offset;
  this->tip_relokacije=tip_relokacije;
  this->simbol=simbol;
  this->addend=addend;
}
string dohvNazivSekcije(){
  return this->sekcija;
}
friend ostream &operator<<(ostream &os, const UlazURelokTabelu u);
};
#endif