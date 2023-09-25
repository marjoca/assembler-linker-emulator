#ifndef SIMBOL_HPP
#define SIMBOL_HPP

#include<string>
#include<iostream>
#include<iomanip>


using namespace std;

class Simbol
{
public:
    int rBr;
    string ime;
    long vrednost;
    string sekcija;
    string tip;
    string bind;
    int size;
    bool globalDef;
    string file;
    bool razresen;
    Simbol(int rBr,string ime, long vrednost,string sekcija,string tip ,string bind, int size){
        this->rBr=rBr;
  this->ime=ime;
  this->vrednost=vrednost;
  this->sekcija=sekcija;
  this->tip=tip;
  this->bind=bind;
  this->size=size;
  this->globalDef=false;
    }
    Simbol(string file,int rBr,string ime, long vrednost,string sekcija,string tip ,string bind){
        this->rBr=rBr;
        this->ime=ime;
        this->vrednost=vrednost;
        this->sekcija=sekcija;
        this->tip=tip;
        this->bind=bind;
        this->file=file;
    }
    Simbol(int rBr,string ime,long vrednost,string sekcija,string tip ,string bind, int size,bool globalDef){
        this->rBr=rBr;
  this->ime=ime;
  this->vrednost=vrednost;
  this->sekcija=sekcija;
  this->tip=tip;
  this->bind=bind;
  this->size=size;
  this->globalDef=globalDef;
    }
    void setRazresen(bool r){
        this->razresen=r;
    }
    void setVrednost(long adresa){
        this->vrednost=adresa;
    }
    long getVrednost(){
        return this->vrednost;
    }
   friend ostream &operator<<(ostream &os, const Simbol s); 
};

#endif