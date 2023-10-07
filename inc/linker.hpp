#ifndef LINKER_HPP
#define LINKER_HPP
#include<iostream>
#include "string.h"
#include "list"
#include <fstream>
#include "sekcija.hpp"
#include "ulazURelokTabelu.hpp"
#include "simbol.hpp"
#include<algorithm>
#include <vector>
#include <bits/stdc++.h>
#include "sekcijaLinker.hpp"
using namespace std;
struct Place{
  string sekcija;
  string hexAdresa;
};
class Linker{
private:
//svi fajlovi iz kojih treba da se ucita
list<char*> ulazniFajlovi;
vector<SekcijaLinker*> allSections;
vector<Simbol*> symbolTable;
vector<Simbol*> undefined_symbols;
//vector<UlazURelokTabelu*> relokacioni
map<long,string> mapa;
int rBr;
bool visestrukaDefinicija;

public:
Linker(){
this->rBr=0;
this->visestrukaDefinicija=false;
}
int getRBR(){
return this->rBr++;
}
void decRBR(){
  this->rBr--;
}
list<char*>  getInputFiles(){
  return this->ulazniFajlovi;
}
vector<Simbol*> getSymbolTable(){
  return this->symbolTable;
}
void addInputFile(char* fajl){
  this->ulazniFajlovi.push_back(fajl);
}
void addToMap(long adresa,string sadrzaj){
  mapa.insert({ adresa, sadrzaj});
}
void EditElementInMap(long adresa,string sadrzaj){
  mapa[adresa]=sadrzaj;
}
bool getVisestrukaDefinicijaFleg(){
  return this->visestrukaDefinicija;
}
void addSection(Sekcija* sekcija){//kad se pozove addSEction provera da li postoji to ime u all sections
  
    bool dodata=false;
    for (int i=0;i<this->allSections.size();i++){
      
        if(this->allSections[i]->ime==sekcija->ime){//postoji vec sekcija u allsections,dodati u tu sekciju u istoimene
          this->allSections[i]->addToSekcijaLinker(sekcija);
          dodata=true;
        }
    }
  if(dodata==false){//ne postoji sekcija sa datim imenom za sad
    SekcijaLinker* sl=new SekcijaLinker(sekcija->ime);
    Simbol* s=new Simbol(sekcija->file,getRBR(),sekcija->ime,0,sekcija->ime,"SCTN","LOKAL");
    
    this->symbolTable.push_back(s);
    sl->addToSekcijaLinker(sekcija);
    this->allSections.push_back(sl);
  }
}
void addSymbol(Simbol* s){
  //ako je sekcija UND dodati simbol u undefined simbols
  if(s->sekcija=="UND"){
    s->setRazresen(false);
    undefined_symbols.push_back(s);
    decRBR();
  }else{
    if(s->tip=="SCTN"){
      decRBR();
      //ne dodaju se sekcije u tabelu, one se naknadno dodaju
    }else{
      //proveriti da li simbol vec postoji ako postoji postaviti fleg visestruka definicija simbola
      
      for(int i=0;i<this->symbolTable.size();i++){
        if(this->symbolTable[i]->ime==s->ime){
          this->visestrukaDefinicija=true;
          break;
        }
      }
      symbolTable.push_back(s);
    }
    
  }
  //ako nije dodati u tabelu simbola
}
vector<Simbol*> getUndefinedSymbols(){
  return this->undefined_symbols;
}
vector<SekcijaLinker*> getAllSections(){
  return this->allSections;
}
map<long,string> getMap(){
  return this->mapa;
}
void findGivenSectionAndAddData(string sectionName,string data,string file){//pronadje se odgovarajuca sekcija i doda u nju
  for(int i=0;i<this->allSections.size();i++){
    if(this->allSections[i]->ime==sectionName){
      //sad se iterira kroz istoimene sekcije da se pronadje sekcija iz tog fajla
      for(int j=0;j<this->allSections[i]->istoimene_sekcije.size();j++){
        if(this->allSections[i]->istoimene_sekcije[j]->file==file){
          this->allSections[i]->istoimene_sekcije[j]->dodaj4Bajta(data);
          break;
        }
      }
      break;
    }
  }
  
}
void findGivenSectionAndAddRelokacioniZapis(string sectionName,UlazURelokTabelu* u,string file){//pronadje se odgovarajuca sekcija i doda u nju
  for(int i=0;i<this->allSections.size();i++){
    if(this->allSections[i]->ime==sectionName){
      //sad se iterira kroz istoimene sekcije da se pronadje sekcija iz tog fajla
      for(int j=0;j<this->allSections[i]->istoimene_sekcije.size();j++){
        if(this->allSections[i]->istoimene_sekcije[j]->file==file){
          this->allSections[i]->istoimene_sekcije[j]->dodajURelokTabeluLInker(u);
          break;
        }
      }
      break;
    }
  }
  
}
void findGivenSectionAndAddUlazUBazen(string sectionName,UlazUBazen* u,string file){
  for(int i=0;i<this->allSections.size();i++){
    if(this->allSections[i]->ime==sectionName){
      //sad se iterira kroz istoimene sekcije da se pronadje sekcija iz tog fajla
      for(int j=0;j<this->allSections[i]->istoimene_sekcije.size();j++){
        if(this->allSections[i]->istoimene_sekcije[j]->file==file){
          this->allSections[i]->istoimene_sekcije[j]->dodajULazUBazenLinker(u);
          break;
        }
      }
      break;
    }
  }
  
}
//napravim klasu SectionLinker i dodam polje string file kao fajl iy koga je sekcija
};
void ucitajIzUlaznogFajla(char* imeFajla);
void ucitajSekciju(string linija,char* imeFajla);
void dodaj4BuSekciju(string sekcija,string linija);
void formirajUlazUBazen(string sekcija,vector<string> ulaz,string file);
#endif