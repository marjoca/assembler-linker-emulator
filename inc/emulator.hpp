#ifndef EMULATOR_HPP
#define EMULATOR_HPP
#include "string.h"
#include<string>
#include<iostream>
#include <list>
#include <map>
#include <fstream>
#include<algorithm>
#include <vector>
#include <iomanip>


using namespace std;



class Emulator{
  public:
    long r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,ldpomocni;
    map<long,string> memorija;
    long status,handler,cause;
    bool fleg;

    Emulator(string adresaPcNakonRestarta){
       this->r0=0;
       this->r1=0;
       this->r2=0;
       this->r3=0;
       this->r4=0;
       this->r5=0;
       this->r6=0;
       this->r7=0;
       this->r8=0;
       this->r9=0;
       this->r10=0;
       this->r11=0;
       this->r12=0;
       this->r13=0;
       this->r14=0;
       this->status=0;
       this->handler=0;
       this->cause=0;
       this->ldpomocni=0;
       //convert hex string to long
       long pc = strtol(adresaPcNakonRestarta.c_str(), NULL, 0);
       cout<<pc;
       this->r15=pc;
       this->fleg=true;
    }
    void ucitaj(string adresa,string instrukcija){
      long adr=stol(adresa);
      memorija[adr]=instrukcija;
    }
    bool getFleg(){
      return this->fleg;
    }
    void setFleg(bool b){
      this->fleg=b;
    }
    string dohvatiIzMemorije(long adresa){
      
      return memorija[adresa];
    }
    map<long,string> getMap(){
      return this->memorija;
    }
    
    void izvrsiInstrukciju(string oc,string mod,string regA,string regB,string regC,string D);
    void izvrsavaj();
    void ucitajZapisUMemoriju(vector<string>zapis);
    void ucitajIzUlaznogFajla(char* imeFajla);
    void ispisiMemoriju();
    long* getStatusRegister(string reg){
      if(reg=="0"){
        return &status;
      }
      if(reg=="1"){
        return &handler;
      }
      if(reg=="2"){
        return &cause;
      }
      return nullptr;
    }
    long* getRegister(string reg){
      if(reg=="0"){
        return &r0;
      }
      if(reg=="1"){
        return &r1;
      }
      if(reg=="2"){
        return &r2;
      }
      if(reg=="3"){
        return &r3;
      }
      if(reg=="4"){
        return &r4;
      }
      if(reg=="5"){
        return &r5;
      }
      if(reg=="6"){
        return &r6;
      }
      if(reg=="7"){
        return &r7;
      }
      if(reg=="8"){
        return &r8;
      }
      if(reg=="9"){
        return &r9;
      }
      if(reg=="a"){
        return &r10;
      }
      if(reg=="b"){
        return &r11;
      }
      if(reg=="c"){
        return &r12;
      }
      if(reg=="d"){
        return &r13;
      }
      if(reg=="e"){
        return &r14;
      }
      if(reg=="f"){
        return &r15;
      }
      return nullptr;
    }
};
#endif