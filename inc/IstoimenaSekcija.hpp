#ifndef ISTOIMENASEKCIJA_HPP
#define ISTOIMENASEKCIJA_HPP
#include<string>
#include<iostream>
#include<iomanip>
#include<vector>
#include "sekcija.hpp"
using namespace std;
class IstoimenaSekcija{
  public:
  string zajednicko_ime;
  vector<Sekcija*>sekcije;
  int ukupnaVelicina;
 IstoimenaSekcija(string zajednicko_ime){
  this->zajednicko_ime=zajednicko_ime;
  this->ukupnaVelicina=0;
 }
  void addSection(Sekcija* sek){
    sekcije.push_back(sek);
    this->ukupnaVelicina+=(sek->bazen.size()*4);
    this->ukupnaVelicina+=sek->getSize();
    
  }
};
#endif