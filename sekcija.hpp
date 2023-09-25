#ifndef SEKCIJA_HPP
#define SEKCIJA_HPP

#include<string>
#include<iostream>
#include <list>
#include "ulazUBazen.hpp"
#include "ulazURelokTabelu.hpp"
using namespace std;



class Sekcija{
  public:
    string file;//postavlja se samo u linkeru
    int rBr;
    int size;//prilikom ispisa cu da konvertujem u HEX
    string type;
    string ime;
    list<UlazURelokTabelu> realokacionaTabela;
    list<string> data;
    list<UlazUBazen> bazen;
    long pocetna_adresa;

    Sekcija(int rBr,string type,string ime){
        this->rBr=rBr;
        this->type=type;
        this->ime=ime;
        
    }
   Sekcija(string file,int rBr,int size,string type,string ime){//konstruktor za linker sekciju
      this->file=file;
        this->rBr=rBr;
        this->size=size;
        this->ime=ime;
        
    }
    Sekcija(){

    }
    list<UlazUBazen> dohvBazenLiterala(){
      return this->bazen;
    }
    void setPocetnaAdresa(long address){
      this->pocetna_adresa=address;
    }
    long getPocetnaAdresa(){
      return this->pocetna_adresa;
    }
    list<UlazURelokTabelu> dohvRelokTabelu(){
      return this->realokacionaTabela;
    }
    list<string> dohvData(){
      return this->data;
    }
    
    void setSize(int size){
      this->size=size;
    }
    int getSize(){
      return this->size;
    }
    void dodajPolaBajta(string data){
      this->data.push_back(data);
    }
    void dodaj4Bajta(string data){
      this->data.push_back(data);
      cout<<data;
    }
    void dodajURealokacionuTabelu(string sekcija,int offset,string tip,string simbol,int addend){
      UlazURelokTabelu* u=new UlazURelokTabelu(sekcija,offset,tip,simbol,addend);
      this->realokacionaTabela.push_back(*u);
}
    void dodajUlazUBazen(long vrednost){
      UlazUBazen* u=new UlazUBazen(vrednost,32);
      bazen.push_back(*u);
      }
    void dodajULazUBazenLinker(UlazUBazen* u){
       bazen.push_back(*u);
    }
    void dodajURelokTabeluLInker(UlazURelokTabelu* u){
      this->realokacionaTabela.push_back(*u);
    }
  bool daLiPostojiUlazUBazenu(long vrednost){
    list<UlazUBazen>::iterator it;

    for (it = bazen.begin(); it != bazen.end(); ++it){
        if((*it).vrednost==vrednost){
          return true;
        }
    }
    return false;
  }
  void dodajLokacijeLiteralimaUBazenu(){
    list<UlazUBazen>::iterator it;
    long location=this->size;
    for (it = bazen.begin(); it != bazen.end(); ++it){

          (*it).postaviLokaciju(location);
          location+=4;
        }
  }
  
  long dohvVelicinuBazenaLiteralaZaDatuSekciju(long vrednostsimbola){
    
    long lokacija=(this->bazen.back().dohvLokacijuUlaza())+4;
cout<<"lokacija "<<"\n";
    UlazUBazen* u=new UlazUBazen(vrednostsimbola,32);
    u->postaviLokaciju(lokacija);
      bazen.push_back(*u);

    return lokacija;
  }
  int dohvVelicinuBazena(){
    return (this->bazen.back().dohvLokacijuUlaza())+4;
  }
  list<UlazUBazen> dohvBazen(){
    return this->bazen;
  }
  long dohvLokacijuLiteralaIzBazena(long vrednost){
    list<UlazUBazen>::iterator it;

    for (it = bazen.begin(); it != bazen.end(); ++it){
        if((*it).vrednost==vrednost){
          return (*it).dohvLokacijuUlaza();
        }
    }
    return 0;
  }
    
    
   friend ostream &operator<<(ostream &os, const Sekcija sek); 
   
};
#endif
