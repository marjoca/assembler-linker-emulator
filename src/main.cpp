#include <iostream>
#include <fstream>
#include "../parser.tab.h"
#include "../inc/asembler.hpp"
using namespace std;

extern FILE *yyin;
extern int yy_scan_string(const char *);



void prviProlazFunkcija(char* imeUlaznogFajla,char* imeIzlaznogFajla){
  ifstream ulaz;
  ofstream izlaz;

  ulaz.open(imeUlaznogFajla);
  izlaz.open(imeIzlaznogFajla);

  string linija;
  
  /*--------------------inicijalizacija pocetnih promenljivih----------------------------*/
  
  //videti sta treba za listuSekcija

  while(getline(ulaz,linija)){
    linija+='\n';
    yy_scan_string(linija.c_str());
    /*cout<<linija;*/
    yyparse();
  }
  if(dohvPrviProlaz2()==false){
    cout<<"drugi prolaz";
    ulaz.clear();
    ulaz.seekg(0);
    while(getline(ulaz,linija)){
    linija+='\n';
    //cout<<linija;
    yy_scan_string(linija.c_str());
    yyparse();
  }
  }
  ofstream out(imeIzlaznogFajla);
  streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  cout.rdbuf(out.rdbuf());

  ispisiTabeluSekcija();
  ispisiTabeluSimbola();
  ispisiRelokacioneZapise();
  ispisiDataSvakeSekcijeIBazenLiterala();
  
  std::cout.rdbuf(coutbuf);
  ulaz.close();
  izlaz.close();
}
int main(int argc,char* argv[]){

  char* imeUlaznogFajla=argv[3];
  char* imeIzlaznogFajla=argv[2];

  prviProlazFunkcija(imeUlaznogFajla,imeIzlaznogFajla);
  
  return 0;
}