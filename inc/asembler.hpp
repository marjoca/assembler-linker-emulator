#ifndef ASEMBLER_HPP
#define ASEMBLER_HPP

#include "simbol.hpp"
#include "sekcija.hpp"
#include "ulazUBazen.hpp"

#include <iostream>
#include <list>
using namespace std;

//bool prviProlaz=true;
class Asembler{
  public:
  list<Sekcija*> listaSekcija;
  list<Simbol> listaSimbola;
  bool error;
  int locationCounter;
  int rBrSimbola;
  int rBrSekcije;
  bool prviProlaz=true;
  Sekcija* tekucaSekcija;
  

  Asembler(){
    this->locationCounter=0;
    this->rBrSimbola=0;
    this->rBrSekcije=0;
    this->error=false;
  }
  Sekcija* dohvTekucaSekcija(){
    return this->tekucaSekcija;
  }
  void postaviTekucuSekciju(string imeSekcije){
    //pretrazi liste sekcija po imenu
    list<Sekcija*>::iterator it;
    
    for (it = listaSekcija.begin(); it != listaSekcija.end(); ++it){
        if((*it)->ime==imeSekcije){
          
          this->tekucaSekcija=(*it);
        }
    }
  }
  /*void azurirajTekucuSekciju(){
    list<Sekcija>::iterator it;

    for (it = listaSekcija.begin(); it != listaSekcija.end(); ++it){
        if((*it).ime==(this->tekucaSekcija.ime)){
          it=listaSekcija.erase(it);
          listaSekcija.insert(it,this->tekucaSekcija);
        }
    }
  }*/
  void incLocCounter(){
    locationCounter++;
  }
  bool dohvPrviProlaz(){
    return this->prviProlaz;
  }
  void setPrviProlaz(bool n){
    this->prviProlaz=n;
  }
  void incLocationCounterForN(int n){
    locationCounter+=n;
  }
  void resetujLocCounter(){
    locationCounter=0;
  }
  int getAndIncRBrSimbola(){
    return rBrSimbola++;
  }
  int getAndIncRBrSekcije(){
    return rBrSekcije++;
  }
  void addToSymbolList(Simbol s){
    listaSimbola.push_back(s);
  }
  void addToSectionList(Sekcija* sek){
    listaSekcija.push_back(sek);
  }
  bool listaSekcijaEmpty(){
    if(listaSekcija.size()==0)return true;
    else return false;
  }
  int getLocationCounter(){
    return this->locationCounter;
  }
  void setSizeInLastSection(){
    listaSekcija.back()->setSize(this->locationCounter); //PROVERITI
  }
  string getNameOfLastSection(){
    string ime;
    ime=listaSekcija.back()->ime;
    return ime;
  }
  bool doesSymbolAlreadyExistsInSymbolTable(string imeSimbola){
    for (auto it = listaSimbola.begin(); it != listaSimbola.end(); ++it){
        if((*it).ime==imeSimbola){
          return true;
        }
    }
      return false;  
  }
  bool checkGlobalDefFleg(string imeSimbola){//PROVERITI
    list<Simbol>::iterator it=listaSimbola.begin();
    while((*it).ime!=imeSimbola){
      it++;
    }
    
    return (*it).globalDef;
  }
  int dohvRBrSimbola(string imeSimbola){
    list<Simbol>::iterator it=listaSimbola.begin();
    while((*it).ime!=imeSimbola){
      it++;
    }
    return (*it).rBr;
  }
  void editSymbolInSymbolTable(string imeSimbola,Simbol noviSimbol){//PROVERITIII
  list<Simbol>::iterator it;

    for (it = listaSimbola.begin(); it != listaSimbola.end(); ++it){
        if((*it).ime==imeSimbola){
          it=listaSimbola.erase(it);
          listaSimbola.insert(it,noviSimbol);
        }
    }
}
int getSymbolValueBasedOnSymbolName(string nameOfSymbol){
  list<Simbol>::iterator it;

    for (it = listaSimbola.begin(); it != listaSimbola.end(); ++it){
        if((*it).ime==nameOfSymbol){
          return (*it).vrednost;
        }
    }
    return 0;
}
Simbol getSymbolBasedOnSymbolName(string nameOfSymbol){
  list<Simbol>::iterator it;

    for (it = listaSimbola.begin(); it != listaSimbola.end(); ++it){
        if((*it).ime==nameOfSymbol){
          return (*it);
        }
    }
    //ovde treba da vrati nesto nema veze sta neki simbol sa nulama nprr
}
  list<Simbol> getSymbolTable(){
    return this->listaSimbola;
  }
  list<Sekcija*> getSectionTable(){
    return this->listaSekcija;
  }
  

};


void debagovanje(string simbol);
void dodajLiteralUBazen(long literal);
void test(char* poruka);
void dodajSimbol(char* imeSimbola);
void dodajSekciju(string imeSekcije);
void dodajLabelu(string imeLabele);
void dodajGlobalSimbol(string imeSimbola);
void ispisiTabeluSimbola();
void ispisiTabeluSekcija();
void ispisiRelokacioneZapise();
void ispisiDataSvakeSekcijeIBazenLiterala();
void dodajGlobalListaSimbola(char* lista);
void dodajExternSimbol(string imeSimbola);
void dodajExternListaSimbola(char* lista);
void dodajSkipDecimal(int n);
void dodajSkipHex(string n);
void dodajEnd();
void dodajNBezArg(string instrukcija);
void dodajNJedanArgument(string instrukcija,string brRegistra);
void dodajNDvaArgumenta(string instrukcija,string reg1,string reg2);
void dodajCsrrd(string instrukcija,string programskiReg,string registar);
void dodajCsrwr(string instrukcija,string registar,string programskiReg);
void dodajCallDecimal(long operand);
void dodajCallHex(string operand);
void dodajCallSimbol(string operand);
void dodajJmpDecimal(long operand);
void dodajJmpHex(string operand);
void dodajJmpSimbol(string operand);
void dodajBranchDecimal(string instrukcija,string gpr1,string gpr2,int operand);
void dodajBranchHex(string instrukcija,string gpr1,string gpr2,string operand);
void dodajBranchSimbol(string instrukcija,string gpr1,string gpr2,string operand);
void dodajLdImmDecimal(string operand,string gpr);//odavde op u obliku decimalnog broja
void dodajLdImmHex(string operand,string gpr);//u obliku 0x...
void dodajLdSymbolImm(string operand,string gpr);//operand je symbol
void dodajLdDecimal(int operand,string gpr);
void dodajLdHex(string operand,string gpr);
void dodajLdSimbol(string operand,string gpr);
void dodajLdRegDir(string reg1,string reg2);
void dodajLdRegInd(string reg1,string reg2);
void dodajLdRegistarDecimalLiteral(string reg1,int literal,string reg2);
void dodajLdRegistarHexLiteral(string reg1,string hexliteral,string reg2);
void dodajLdRegistarSimbol(string reg1,string simbol,string reg2);
void dodajStDecimalLiteral(string reg,int decimalliteral);
void dodajStHexLiteral(string reg,string hexliteral);
void dodajStSimbol(string reg,string simbol);
void dodajStRegInd(string reg1,string reg2);
void dodajStRegistarDecimalLiteral(string reg1,string reg2,int decimalliteral);
void dodajStRegistarHexLiteral(string reg1,string reg2,string hexliteral);
void dodajStRegistarSimbol(string reg1,string reg2,string simbol);
void ispisiBazenLiterala();
int kolikoJeBitovaZapravoPotrebno(int n);
void dodajWordSimbol(string simbol);
void dodajWordDecimalLiteral(long decimalliteral);
void dodajWordHexLiteral(string hexliteral);
void dodajWordListaLiterala(char* lista);
void dodajWordListaSimbola(char* lista);
bool dohvPrviProlaz2();
string convertIntToHex(int broj);
string convertBinToHex(char* binzapis);

#endif