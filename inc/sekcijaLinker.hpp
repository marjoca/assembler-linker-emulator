#ifndef SEKCIJALINKER_HPP
#define SEKCIJALINKER_HPP

#include<string>
#include<iostream>
#include <list>
#include "ulazUBazen.hpp"
#include <vector>
#include "sekcija.hpp"
#include "ulazURelokTabelu.hpp"
using namespace std;



class SekcijaLinker{
  public:
    string ime;
    int ukupnaVelicina;
    vector<Sekcija*> istoimene_sekcije;
    bool mapirano;
    long pocetna_adresa;
    bool place;
    long place_adresa;
    

    SekcijaLinker(string ime){
        this->ime=ime;
        this->mapirano=false;
        this->place=false;
        this->ukupnaVelicina=0;
    }
    void addToSekcijaLinker(Sekcija* s){
      istoimene_sekcije.push_back(s);
      
    }
    long getUkupnaVelicina(){
      return (long)this->ukupnaVelicina;
    }
    void setPlace(bool t){
      this->place=t;
    }
    bool getPlace(){
      return this->place;
    }
    vector<Sekcija*> getIstoimeneSekcije(){
      return this->istoimene_sekcije;
    }
    void setStartingAddress(long address){
      this->pocetna_adresa=address;
    }
    long getStartingAddress(){
      return this->pocetna_adresa;
    }
    void izracunajUkupnuVelicinuSekcije(){
      //kada se sve sekcije ucitaju prodje se kroz vector istoimenih sekcija i saberu se sve velicine
      for(int i=0;i<this->istoimene_sekcije.size();i++){
        this->ukupnaVelicina+=this->istoimene_sekcije[i]->getSize();
        this->ukupnaVelicina+=this->istoimene_sekcije[i]->bazen.size()*4;
      }
    }
    void postaviSvimIstoimenimSekcijamaPocetnuAdresu(){
      long adresa=this->pocetna_adresa;
      for(int i=0;i<this->istoimene_sekcije.size();i++){
        this->istoimene_sekcije[i]->setPocetnaAdresa(adresa);
        adresa+=this->istoimene_sekcije[i]->getSize();
        adresa+=this->istoimene_sekcije[i]->bazen.size()*4;
      }
    }
   
    
   
   
};
#endif