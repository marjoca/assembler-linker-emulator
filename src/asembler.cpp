
#include "../inc/asembler.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;

void test(char *poruka)
{
  cout << poruka;
}
void dodajSimbol(char *imeSimbola)
{
  Simbol *s = new Simbol(0, imeSimbola, 0, "bss", "NOTYP", "GLOB", 0);
  // cout << *s;
}
Asembler *as = new Asembler(); // premestiti ako radi
bool prviPut = true;           // ovo premestiti u asembler.hpp
void dodajSekciju(string imeSekcije)
{
  if (as->prviProlaz)
  {
    if (!as->listaSekcijaEmpty())
    {
      as->setSizeInLastSection();
      as->getSectionTable().back()->dodajLokacijeLiteralimaUBazenu();
    }
    as->resetujLocCounter();
    Simbol *s = new Simbol(as->getAndIncRBrSimbola(), "." + imeSekcije, as->getLocationCounter(), "." + imeSekcije, "SCTN", "LOCAL", 0); // dodati rBr
    // cout << *s;
    as->addToSymbolList(*s);

    Sekcija *sek = new Sekcija(as->getAndIncRBrSekcije(), "PROGBITS", "." + imeSekcije); // type je NOBITS samo kad je sekcija .bss DODATI
    as->addToSectionList(sek);
  }
  else
  { // drugi prolaz

    if (prviPut)
    {
      //cout << "prva sekcija u drugom prolazu"
          // << "\n";
      as->postaviTekucuSekciju("." + imeSekcije);
      // cout<<(as->dohvTekucaSekcija().ime)<<"\n";
      prviPut = false;
    }
    else
    {
      // zameni staru sekciju
      // cout<<as->dohvTekucaSekcija();
      Sekcija *s = as->dohvTekucaSekcija();
      list<string>::iterator it;
      as->postaviTekucuSekciju("." + imeSekcije);
      as->resetujLocCounter();
      // as->azurirajTekucuSekciju();
    }
  }
}
void debagovanje(string simbol){
  //cout<<"DEBAGOVANJE"<<simbol;
  //cout<<simbol;
  if (as->prviProlaz)
  { 
    Simbol *s = new Simbol(as->getAndIncRBrSimbola(), simbol, 0, "UND", "NOTYP", "GLOBAL", 0, true);
    // cout << *s;
    as->addToSymbolList(*s);
  }
}
void dodajLabelu(string imeLabele)
{ 
  if (as->dohvPrviProlaz())
  {
    if (as->doesSymbolAlreadyExistsInSymbolTable(imeLabele.substr(0, imeLabele.size() - 1)))
    { // ova f-ja radi
    
      if (as->checkGlobalDefFleg(imeLabele.substr(0, imeLabele.size() - 1)))
      { // globalDEF=true//ova f-ja radii
        
        cout<<imeLabele;
        Simbol *s = new Simbol(as->dohvRBrSimbola(imeLabele.substr(0, imeLabele.size() - 1)), imeLabele.substr(0, imeLabele.size() - 1), as->getLocationCounter(), as->getNameOfLastSection(), "NOTYP", "GLOBAL", 0, false);
        
        as->editSymbolInSymbolTable(imeLabele.substr(0, imeLabele.size() - 1), *s); // ova se jos nije proverila
        
      }
      else
      { // GRESKA > VISESTRUKA DEFINICIJA SIMBOLA
        cout << "Visestruka definicija simbola! " << imeLabele.substr(0, imeLabele.size() - 1) << "\n";
      }
    }
    else
    { // nema takvog simbola u tabeli
      Simbol *s = new Simbol(as->getAndIncRBrSimbola(), imeLabele.substr(0, imeLabele.size() - 1), as->getLocationCounter(), as->getNameOfLastSection(), "NOTYP", "LOCAL", 0);
      
      as->addToSymbolList(*s);
    }
  }
  else
  {
  }
}

void dodajGlobalSimbol(string imeSimbola)
{
  if (as->dohvPrviProlaz())
  { 
    Simbol *s = new Simbol(as->getAndIncRBrSimbola(), imeSimbola, 0, "UND", "NOTYP", "GLOBAL", 0, true);
    
    as->addToSymbolList(*s);
  }
  else
  {
    // u drugom prolazu ne radimo nista
  }
}
void ispisiTabeluSimbola()
{
  list<Simbol> listaSimbola = as->getSymbolTable();
  list<Simbol>::iterator it;

  cout<<"#symtab"<<"\n";
  for (it = listaSimbola.begin(); it != listaSimbola.end(); ++it)
  {
    cout << (*it);
  }
}
void ispisiTabeluSekcija()
{
  list<Sekcija *> listaSekcija = as->getSectionTable();
  list<Sekcija *>::iterator it;
  cout<<"#shdr"<<"\n";
  for (it = listaSekcija.begin(); it != listaSekcija.end(); ++it)
  {
    cout << *(*it);
  }
  
}
void ispisiRelokacioneZapise(){
  list<Sekcija *> listaSekcija = as->getSectionTable();
  list<Sekcija *>::iterator it;
  
  for (it = listaSekcija.begin(); it != listaSekcija.end(); ++it)
  {
    cout<<"#.rela"<<(*(*it)).ime<<"\n";
    list<UlazURelokTabelu> relokTabela=(*(*it)).dohvRelokTabelu();
    list<UlazURelokTabelu>::iterator et;
    for (et = relokTabela.begin(); et != relokTabela.end(); ++et)
    cout << (*et);
  }
  cout<<"#end"<<endl;
}
void ispisiDataSvakeSekcijeIBazenLiterala(){
  list<Sekcija *> listaSekcija = as->getSectionTable();
  list<Sekcija *>::iterator it;
  
  for (it = listaSekcija.begin(); it != listaSekcija.end(); ++it)
  {
    cout<<"#"<<(*(*it)).ime<<"\n";
    list<string> datalist=(*(*it)).dohvData();
    list<UlazUBazen> bazenlist=(*(*it)).dohvBazen();
    list<string>::iterator et;
    list<UlazUBazen>::iterator jt;
    int i=0;
    //bool zadnji=false;
    for (et = datalist.begin(); et != datalist.end(); ++et){
      i++;
      cout << (*et);
      if(std::next(et) == datalist.end()) break;
      if(i%8==0) cout<<" ";
    }
    if(datalist.size()!=0) cout<<"\n";
   
    cout<<"#.bazen"<<"\n";
     for (jt=bazenlist.begin(); jt != bazenlist.end(); ++jt){
      
      cout <<(*jt);
    }
    cout<<"#end"<<"\n";
  }
}
/*void ispisiBazenLiterala()
{
  list<UlazUBazen> bazen = as->dohvBazen();
  list<UlazUBazen>::iterator it;
  cout<<"\n";
  for (it = bazen.begin(); it != bazen.end(); ++it)
  {
    cout << (*it);
  }
}*/
void dodajGlobalListaSimbola(char *lista)

{
  if (as->dohvPrviProlaz())
  {
    // Pointer to point the word returned by the strtok() function.
    char *povracaj;
    // Here, the delimiter is white space.
    povracaj = strtok(lista, ", ");
    while (povracaj != NULL)
    {
      dodajGlobalSimbol(povracaj);
      cout << povracaj << endl;
      povracaj = strtok(NULL, ", ");
    }
    
  }
  else
  {
  }
}
void dodajExternSimbol(string imeSimbola)
{
  if (as->dohvPrviProlaz())
  {
    if (as->doesSymbolAlreadyExistsInSymbolTable(imeSimbola))
    { // ova f-ja radi
      cout << ".extern simbol " << imeSimbola << " je vec u tabeli simbola\n";
      as->error=true;

    }
    else
    { // nema takvog simbola u tabeli
      Simbol *s = new Simbol(as->getAndIncRBrSimbola(), imeSimbola, 0, "UND", "NOTYP", "GLOBAL", 0);
      
      as->addToSymbolList(*s);
    }
  }
  else
  {
  }
}
void dodajExternListaSimbola(char *lista)
{
  if (as->dohvPrviProlaz())
  {
    // Pointer to point the word returned by the strtok() function.
    char *povracaj;
    // Here, the delimiter is white space.
    povracaj = strtok(lista, ", ");
    while (povracaj != NULL)
    {
      dodajExternSimbol(povracaj);
      cout << povracaj << endl;
      povracaj = strtok(NULL, ", ");
    }
    // cout<<lista;
  }
  else
  {
  }
}
void dodajWordSimbol(string simbol)
{if(as->dohvPrviProlaz()){
  as->incLocationCounterForN(4);
}else{
   if (as->doesSymbolAlreadyExistsInSymbolTable(simbol))
  { // ova f-ja radi
  //dohv simbol iz tabele simbola
  Simbol s=as->getSymbolBasedOnSymbolName(simbol);
  if (s.bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, as->getLocationCounter(), "R_x86_64_32", s.ime, 0);
        }
        else
        {
          if (s.bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, as->getLocationCounter(), "R_x86_64_32", s.sekcija, s.vrednost);
          }
        }
  }
  else
  { // nema takvog simbola u tabeli
    Simbol *s = new Simbol(as->getAndIncRBrSimbola(), simbol, 0, as->getNameOfLastSection(), "NOTYP", "LOCAL", 0);
    
    as->addToSymbolList(*s);
    if ((*s).bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, as->getLocationCounter(), "R_x86_64_32", (*s).ime, 0);
        }
        else
        {
          if ((*s).bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, as->getLocationCounter(), "R_x86_64_32", (*s).sekcija, (*s).vrednost);
          }
        }
  }
  as->incLocationCounterForN(4);
}
 
  //as->incLocationCounterForN(4);
}
void dodajWordDecimalLiteral(long decimalliteral)
{
  //dodajLiteralUBazen(decimalliteral);
  if(as->dohvPrviProlaz()){
     as->incLocationCounterForN(4);
  }else{
    std::stringstream stream;
stream <<std::setfill('0') << std::setw(8) << std::hex << decimalliteral;
std::string result( stream.str() );
const int length = result.length();
    char* char_array = new char[length + 1];
    strcpy(char_array, result.c_str());
for(int i=length-1;i>=1;i-=2){
  char* prvi=new char[2];
  prvi[0]=char_array[i-1];
  prvi[1]='\0';
  string p=prvi;
  //cout<<"PRVI "<<p<<endl;
  as->dohvTekucaSekcija()->dodajPolaBajta(p);
  char* drugi=new char[2];
  drugi[0]=char_array[i];
  drugi[1]='\0';
  string d=drugi;
   //cout<<"DRUGI "<<d<<endl;
  as->dohvTekucaSekcija()->dodajPolaBajta(d);
 
}
    as->incLocationCounterForN(4);
  }
 
}
void dodajWordHexLiteral(string hexliteral)
{
  long op = strtol(hexliteral.c_str(), NULL, 0);
  if(as->dohvPrviProlaz()){
     as->incLocationCounterForN(4);
  }else{
    std::stringstream stream;
stream <<std::setfill('0') << std::setw(8) << std::hex << op;
std::string result( stream.str() );
const int length = result.length();
    char* char_array = new char[length + 1];
    strcpy(char_array, result.c_str());
for(int i=length-1;i>=1;i-=2){
  char* prvi=new char[2];
  prvi[0]=char_array[i-1];
  prvi[1]='\0';
  string p=prvi;
  //cout<<"PRVI "<<p<<endl;
  as->dohvTekucaSekcija()->dodajPolaBajta(p);
  char* drugi=new char[2];
  drugi[0]=char_array[i];
  drugi[1]='\0';
  string d=drugi;
   //cout<<"DRUGI "<<d<<endl;
  as->dohvTekucaSekcija()->dodajPolaBajta(d);
 
}
    as->incLocationCounterForN(4);
  }
}
void dodajWordListaLiterala(char *lista)
{
  // Pointer to point the word returned by the strtok() function.
  char *povracaj;
  // Here, the delimiter is white space.
  povracaj = strtok(lista, ", ");
  while (povracaj != NULL)
  {
    string s = povracaj;
    if (s.compare(0, 2, "0x"))
    { // dodajwordhex
      // int l=strtol(s.c_str(), NULL, 0);
      dodajWordDecimalLiteral(stoi(s));
    }
    else
    {

      dodajWordHexLiteral(s);
    }
    cout<<s<<endl;
    povracaj = strtok(NULL, ", ");
  }
  // cout<<lista;
}
void dodajWordListaSimbola(char *lista)
{
  // Pointer to point the word returned by the strtok() function.
  char *povracaj;
  // Here, the delimiter is white space.
  povracaj = strtok(lista, ",");
  while (povracaj != NULL)
  {

    dodajWordSimbol(povracaj);

    povracaj = strtok(NULL, ",");
  }
  // cout<<lista;
}
void dodajSkipDecimal(int n)
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(n);
    
  }else{
    as->incLocationCounterForN(n);
    //n je broj bajtova
    for(int i=0;i<n;i++){//DOVRSITIIIII
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
  }
  
}
void dodajSkipHex(string n)
{ long br=strtol(n.c_str(), NULL, 0);
  
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(br);
    
  }else{
    as->incLocationCounterForN(br);
    //n je broj bajtova
    for(int i=0;i<br;i++){//DOVRSITIIIII
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
  }
}
void dodajEnd()
{
  // zavrsava se prvi prolaz i pocinje drugi!!!!!!!!!!

  if (as->dohvPrviProlaz())
  {
    as->setPrviProlaz(false);
    if (!as->listaSekcijaEmpty())
    {
      as->setSizeInLastSection();
      as->getSectionTable().back()->dodajLokacijeLiteralimaUBazenu();
      as->resetujLocCounter();
    }
  }
}
void dodajNBezArg(string instrukcija)
{
  if (as->dohvPrviProlaz())
  {
    if (instrukcija == "halt" || instrukcija == "int" || instrukcija == "ret")
    {
      as->incLocationCounterForN(4);
     
    }
    if (instrukcija == "iret")
    {
      as->incLocationCounterForN(8);
    }
  }
  else
  { // drugi prolaz
    if (instrukcija == "halt")
    {
      // dodati 4 bajta nula u section trenutna.data
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->incLocationCounterForN(4);
    }
    if (instrukcija == "int")
    {

      as->dohvTekucaSekcija()->dodajPolaBajta("1");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->incLocationCounterForN(4);
    }
    if (instrukcija == "ret")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("9");
      as->dohvTekucaSekcija()->dodajPolaBajta("3");
      as->dohvTekucaSekcija()->dodajPolaBajta("f");
      as->dohvTekucaSekcija()->dodajPolaBajta("e");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->incLocationCounterForN(4);
    }
    if (instrukcija == "iret")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("9");
      as->dohvTekucaSekcija()->dodajPolaBajta("6");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("e");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("4");
      as->dohvTekucaSekcija()->dodajPolaBajta("9");
      as->dohvTekucaSekcija()->dodajPolaBajta("3");
      as->dohvTekucaSekcija()->dodajPolaBajta("f");
      as->dohvTekucaSekcija()->dodajPolaBajta("e");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("8");
      as->incLocationCounterForN(8);
    }
  }
}
void dodajNJedanArgument(string instrukcija, string brRegistra)
{ // push,pop,not

  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg = stoi(brRegistra);
    string gpr = convertIntToHex(reg);
    if (instrukcija == "not")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("6");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gpr);
      as->dohvTekucaSekcija()->dodajPolaBajta(gpr);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "pop")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("9");
      as->dohvTekucaSekcija()->dodajPolaBajta("3");
      as->dohvTekucaSekcija()->dodajPolaBajta(gpr);
      as->dohvTekucaSekcija()->dodajPolaBajta("e");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("4");
    }
    if (instrukcija == "push")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("8");
      as->dohvTekucaSekcija()->dodajPolaBajta("1");
      as->dohvTekucaSekcija()->dodajPolaBajta("e");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gpr);
      as->dohvTekucaSekcija()->dodajPolaBajta("f");
      as->dohvTekucaSekcija()->dodajPolaBajta("f");
      as->dohvTekucaSekcija()->dodajPolaBajta("c");
    }
    as->incLocationCounterForN(4);
  }
}
void dodajNDvaArgumenta(string instrukcija, string reg1, string reg2)
{ // xchg|add|sub|mul|div|and|or|xor|shl|shr
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    int registar1 = stoi(reg1);
    int registar2 = stoi(reg2);
    string gprS = convertIntToHex(registar1);
    string gprD = convertIntToHex(registar2);
    if (instrukcija == "add")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("5");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "sub")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("5");
      as->dohvTekucaSekcija()->dodajPolaBajta("1");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "mul")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("5");
      as->dohvTekucaSekcija()->dodajPolaBajta("2");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "div")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("5");
      as->dohvTekucaSekcija()->dodajPolaBajta("3");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "xchg")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("4");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "and")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("6");
      as->dohvTekucaSekcija()->dodajPolaBajta("1");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "or")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("6");
      as->dohvTekucaSekcija()->dodajPolaBajta("2");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "xor")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("6");
      as->dohvTekucaSekcija()->dodajPolaBajta("3");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "shl")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("7");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    if (instrukcija == "shr")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("7");
      as->dohvTekucaSekcija()->dodajPolaBajta("1");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprD);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprS);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
    }
    as->incLocationCounterForN(4);
  }
}
void dodajCsrrd(string instrukcija, string programskiReg, string registar)
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg = stoi(registar);
    string gpr = convertIntToHex(reg);
    string csr;
    if (programskiReg == "status")
    {
      csr = "0";
    }
    if (programskiReg == "handler")
    {
      csr = "1";
    }
    if (programskiReg == "cause")
    {
      csr = "2";
    }

    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta(gpr);
    as->dohvTekucaSekcija()->dodajPolaBajta(csr);
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->incLocationCounterForN(4);
  }
}
void dodajCsrwr(string instrukcija, string registar, string programskiReg)
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg = stoi(registar);
    string gpr = convertIntToHex(reg);
    string csr;
    if (programskiReg == "status")
    {
      csr = "0";
    }
    if (programskiReg == "handler")
    {
      csr = "1";
    }
    if (programskiReg == "cause")
    {
      csr = "2";
    }

    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("4");
    as->dohvTekucaSekcija()->dodajPolaBajta(csr);
    as->dohvTekucaSekcija()->dodajPolaBajta(gpr);
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->incLocationCounterForN(4);
  }
}
void dodajCallDecimal(long operand)
{
  // operand je decimal literal

  if (as->dohvPrviProlaz())
  {

    dodajLiteralUBazen(operand);
    as->incLocationCounterForN(4);
  }
  else
  { // drugi prolaz

    as->dohvTekucaSekcija()->dodajPolaBajta("2");
    as->dohvTekucaSekcija()->dodajPolaBajta("1");
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(operand) - as->getLocationCounter();
   
    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

   
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajCallHex(string operand)
{ // skinuti 0x i konvertovati u dec
  
  long op = strtol(operand.c_str(), NULL, 0);
  if (as->dohvPrviProlaz())
  {

    dodajLiteralUBazen(op);
    as->incLocationCounterForN(4);
  }
  else
  { // drugi prolaz

    as->dohvTekucaSekcija()->dodajPolaBajta("2");
    as->dohvTekucaSekcija()->dodajPolaBajta("1");
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(op) - as->getLocationCounter();

    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
     
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajCallSimbol(string operand)
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {

    if (as->doesSymbolAlreadyExistsInSymbolTable(operand))
    { // simbol postoji u tabeli simbola

      long vrednostSimbola = as->getSymbolValueBasedOnSymbolName(operand);

      
      // ne postoji ulaz u bazenu alociraj prostor

        as->dohvTekucaSekcija()->dodajPolaBajta("2");
        as->dohvTekucaSekcija()->dodajPolaBajta("1");
        as->dohvTekucaSekcija()->dodajPolaBajta("f");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");

        // dodamo realokacioni zapis
        Simbol simbol = as->getSymbolBasedOnSymbolName(operand);
        long lokacija=as->dohvTekucaSekcija()->dohvVelicinuBazenaLiteralaZaDatuSekciju(vrednostSimbola);
        if (simbol.bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, lokacija, "R_x86_64_32", operand, 0);
        }
        else
        {
          if (simbol.bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, lokacija, "R_x86_64_32", simbol.sekcija, vrednostSimbola);
          }
        }
        
        long offset = lokacija - as->getLocationCounter();
        // covert offset in binary

        string binary = std::bitset<12>(offset).to_string();

        
        char *char_array = new char[binary.length() + 1];

        // make sure that the new string is null terminated
        char_array[binary.length()] = '\0';

        for (int i = 0; i < binary.length(); i++)
        {
          char_array[i] = binary[i];
        }
        // char* cifra=new char[4];
        for (int i = 0; i < binary.size();)
        {
          char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
          
          string d = convertBinToHex(cifra);
          as->dohvTekucaSekcija()->dodajPolaBajta(d);
          i += 4;
        }

        // then check if binary can fit in 12 bits
        // if can convert binary in hex digits 4 by 4

        as->incLocationCounterForN(4);
      
    }

    else
    { // ne postoji simbol u tabeli simbola
      cout << "error: ne postoji simbol: " << operand << "u tabeli simbola!"
           << "\n";
    }
  }
}
void dodajJmpDecimal(long operand)
{
  if (as->dohvPrviProlaz())
  {

    dodajLiteralUBazen(operand);
    as->incLocationCounterForN(4);
  }
  else
  { // drugi prolaz

    as->dohvTekucaSekcija()->dodajPolaBajta("3");
    as->dohvTekucaSekcija()->dodajPolaBajta("8");
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(operand) - as->getLocationCounter();

    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajJmpHex(string operand)
{
  
  long op = strtol(operand.c_str(), NULL, 0);
  if (as->dohvPrviProlaz())
  {

    dodajLiteralUBazen(op);
    as->incLocationCounterForN(4);
  }
  else
  { // drugi prolaz

    as->dohvTekucaSekcija()->dodajPolaBajta("3");
    as->dohvTekucaSekcija()->dodajPolaBajta("8");
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(op) - as->getLocationCounter();

    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

   
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajJmpSimbol(string operand)
{ 
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {

    if (as->doesSymbolAlreadyExistsInSymbolTable(operand))
    { // simbol postoji u tabeli simbola

      long vrednostSimbola = as->getSymbolValueBasedOnSymbolName(operand);

      
      // ne postoji ulaz u bazenu alociraj prostor

        as->dohvTekucaSekcija()->dodajPolaBajta("3");
        as->dohvTekucaSekcija()->dodajPolaBajta("8");
        as->dohvTekucaSekcija()->dodajPolaBajta("f");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");

        // dodamo realokacioni zapis
        Simbol simbol = as->getSymbolBasedOnSymbolName(operand);
        long lokacija=as->dohvTekucaSekcija()->dohvVelicinuBazenaLiteralaZaDatuSekciju(vrednostSimbola);
        
        if (simbol.bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, lokacija, "R_x86_64_32", operand, 0);
        }
        else
        {
          if (simbol.bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime,lokacija, "R_x86_64_32", simbol.sekcija, vrednostSimbola);
          }
        }

        long offset = lokacija - as->getLocationCounter();
        // covert offset in binary

        string binary = std::bitset<12>(offset).to_string();

       
        char *char_array = new char[binary.length() + 1];

        // make sure that the new string is null terminated
        char_array[binary.length()] = '\0';

        for (int i = 0; i < binary.length(); i++)
        {
          char_array[i] = binary[i];
        }
        // char* cifra=new char[4];
        for (int i = 0; i < binary.size();)
        {
          char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
          
          string d = convertBinToHex(cifra);
          as->dohvTekucaSekcija()->dodajPolaBajta(d);
          i += 4;
        }

        // then check if binary can fit in 12 bits
        // if can convert binary in hex digits 4 by 4

        as->incLocationCounterForN(4);
      
    }

    else
    { // ne postoji simbol u tabeli simbola
      cout << "error: ne postoji simbol: " << operand << "u tabeli simbola!"
           << "\n";
    }
  }
}
void dodajBranchDecimal(string instrukcija, string gpr1, string gpr2, int operand)
{

  
  if (as->dohvPrviProlaz())
  {
    dodajLiteralUBazen(operand);
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg1 = stoi(gpr1);
    string hexreg1 = convertIntToHex(reg1);
    int reg2 = stoi(gpr2);
    string hexreg2 = convertIntToHex(reg2);

    as->dohvTekucaSekcija()->dodajPolaBajta("3");
    if (instrukcija == "beq")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("9");
    }
    if (instrukcija == "bne")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("a");
    }
    if (instrukcija == "bgt")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("b");
    }
    as->dohvTekucaSekcija()->dodajPolaBajta("f");

    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg1);
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg2);
    // popunjava D bite
    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(operand) - as->getLocationCounter();

    // covert offset in binary
    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajBranchHex(string instrukcija, string gpr1, string gpr2, string operand)
{
 

  long op = strtol(operand.c_str(), NULL, 0);
  if (as->dohvPrviProlaz())
  {
    dodajLiteralUBazen(op);
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg1 = stoi(gpr1);
    string hexreg1 = convertIntToHex(reg1);
    int reg2 = stoi(gpr2);
    string hexreg2 = convertIntToHex(reg2);

    as->dohvTekucaSekcija()->dodajPolaBajta("3");
    if (instrukcija == "beq")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("9");
    }
    if (instrukcija == "bne")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("a");
    }
    if (instrukcija == "bgt")
    {
      as->dohvTekucaSekcija()->dodajPolaBajta("b");
    }
    as->dohvTekucaSekcija()->dodajPolaBajta("f");

    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg1);
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg2);
    // popunjava D bite
    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(op) - as->getLocationCounter();

    // covert offset in binary
    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajBranchSimbol(string instrukcija, string gpr1, string gpr2, string operand)
{
    int reg1 = stoi(gpr1);
    string hexreg1 = convertIntToHex(reg1);
    int reg2 = stoi(gpr2);
    string hexreg2 = convertIntToHex(reg2);

  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    
    if (as->doesSymbolAlreadyExistsInSymbolTable(operand))
    { // simbol postoji u tabeli simbola
      
      int vrednostSimbola = as->getSymbolValueBasedOnSymbolName(operand);

      
       // ne postoji ulaz u bazenu alociraj prostor

        as->dohvTekucaSekcija()->dodajPolaBajta("3");
        if (instrukcija == "beq")
        {
          as->dohvTekucaSekcija()->dodajPolaBajta("9");
        }
        if (instrukcija == "bne")
        {
          as->dohvTekucaSekcija()->dodajPolaBajta("a");
        }
        if (instrukcija == "bgt")
        {
          as->dohvTekucaSekcija()->dodajPolaBajta("b");
        }
        as->dohvTekucaSekcija()->dodajPolaBajta("f");
        as->dohvTekucaSekcija()->dodajPolaBajta(hexreg1);
        as->dohvTekucaSekcija()->dodajPolaBajta(hexreg2);
        

        // dodamo realokacioni zapis
        Simbol simbol = as->getSymbolBasedOnSymbolName(operand);
        long lokacija=as->dohvTekucaSekcija()->dohvVelicinuBazenaLiteralaZaDatuSekciju(vrednostSimbola);
        if (simbol.bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime,lokacija , "R_x86_64_32", operand, 0);
        }
        else
        {
          if (simbol.bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, lokacija, "R_x86_64_32", simbol.sekcija, vrednostSimbola);
          }
        }

        long offset =lokacija - as->getLocationCounter();
        // covert offset in binary

        string binary = std::bitset<12>(offset).to_string();

       
        char *char_array = new char[binary.length() + 1];

        // make sure that the new string is null terminated
        char_array[binary.length()] = '\0';

        for (int i = 0; i < binary.length(); i++)
        {
          char_array[i] = binary[i];
        }
        // char* cifra=new char[4];
        for (int i = 0; i < binary.size();)
        {
          char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
          
          string d = convertBinToHex(cifra);
          as->dohvTekucaSekcija()->dodajPolaBajta(d);
          i += 4;
        }

        // then check if binary can fit in 12 bits
        // if can convert binary in hex digits 4 by 4

        as->incLocationCounterForN(4);
      
    }

    else
    { // ne postoji simbol u tabeli simbola
      cout << "error: ne postoji simbol: " << operand << "u tabeli simbola!"
           << "\n";
    }
  }
}
void dodajLdImmDecimal(string operand, string gpr)
{ 
  long op = stol(operand);

  if (as->dohvPrviProlaz())
  {

    dodajLiteralUBazen(op);
    as->incLocationCounterForN(4);
  }
  else
  { // drugi prolaz
    int reg = stoi(gpr);
    string hexreg = convertIntToHex(reg);

    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("2");
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
   long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(op) - as->getLocationCounter();

    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
     
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajLdImmHex(string operand, string gpr) // u obliku 0x...
{ 

  
  long op = strtol(operand.c_str(), NULL, 0);
  
  if (as->dohvPrviProlaz())
  {
    dodajLiteralUBazen(op);
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg = stoi(gpr);
    string hexreg = convertIntToHex(reg);

    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("2");
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");

    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(op) - as->getLocationCounter();
    // covert offset in binary
    
    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }

    as->incLocationCounterForN(4);
  }
}
void dodajLdSymbolImm(string operand, string gpr) // operand je symbol
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    
    if (as->doesSymbolAlreadyExistsInSymbolTable(operand))
    { // simbol postoji u tabeli simbola

      int vrednostSimbola = as->getSymbolValueBasedOnSymbolName(operand);

      
      /*if (as->dohvTekucaSekcija()->daLiPostojiUlazUBazenu(vrednostSimbola))
      { // proverava da li postoji ulaz u bazenu literala->postoji
        cout << "PVDE JE PART 2 lv=" << as->getLocationCounter() << "\n";
        int reg = stoi(gpr);
        string hexreg = convertIntToHex(reg);

        as->dohvTekucaSekcija()->dodajPolaBajta("9");
        as->dohvTekucaSekcija()->dodajPolaBajta("2");
        as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
        as->dohvTekucaSekcija()->dodajPolaBajta("f");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");

        // dodamo realokacioni zapis
        Simbol simbol = as->getSymbolBasedOnSymbolName(operand);
        if (simbol.bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(vrednostSimbola), "R_x86_64_32", operand, 0);
        }
        else
        {
          if (simbol.bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(vrednostSimbola), "R_x86_64_32", simbol.sekcija, vrednostSimbola);
          }
        }

        int offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(vrednostSimbola) - as->getLocationCounter();
        // covert offset in binary

        string binary = std::bitset<12>(offset).to_string();

        cout << binary << "\n";
        char *char_array = new char[binary.length() + 1];

        // make sure that the new string is null terminated
        char_array[binary.length()] = '\0';

        for (int i = 0; i < binary.length(); i++)
        {
          char_array[i] = binary[i];
        }
        // char* cifra=new char[4];
        for (int i = 0; i < binary.size();)
        {
          char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
          cout << cifra << "\n";
          string d = convertBinToHex(cifra);
          as->dohvTekucaSekcija()->dodajPolaBajta(d);
          i += 4;
        }
        as->incLocationCounterForN(4);
      }*/
       // ne postoji ulaz u bazenu alociraj prostor
        

        int reg = stoi(gpr);
        string hexreg = convertIntToHex(reg);

        as->dohvTekucaSekcija()->dodajPolaBajta("9");
        as->dohvTekucaSekcija()->dodajPolaBajta("2");
        as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
        as->dohvTekucaSekcija()->dodajPolaBajta("f");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");

        // dodamo realokacioni zapis
        Simbol simbol = as->getSymbolBasedOnSymbolName(operand);
        long lokacija= as->dohvTekucaSekcija()->dohvVelicinuBazenaLiteralaZaDatuSekciju(vrednostSimbola);
        if (simbol.bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime,lokacija, "R_x86_64_32", operand, 0);
        }
        else
        {
          if (simbol.bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, lokacija, "R_x86_64_32", simbol.sekcija, vrednostSimbola);
          }
        }
        
        long offset = lokacija - as->getLocationCounter();
        // covert offset in binary

        string binary = std::bitset<12>(offset).to_string();

       
        char *char_array = new char[binary.length() + 1];

        // make sure that the new string is null terminated
        char_array[binary.length()] = '\0';

        for (int i = 0; i < binary.length(); i++)
        {
          char_array[i] = binary[i];
        }
        // char* cifra=new char[4];
        for (int i = 0; i < binary.size();)
        {
          char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
          
          string d = convertBinToHex(cifra);
          as->dohvTekucaSekcija()->dodajPolaBajta(d);
          i += 4;
        }

        // then check if binary can fit in 12 bits
        // if can convert binary in hex digits 4 by 4

        as->incLocationCounterForN(4);
      
    }
  
    else
    { // ne postoji simbol u tabeli simbola
      cout << "error: ne postoji simbol: " << operand << "u tabeli simbola!"
           << "\n";
    }
  }
  }

void dodajLdDecimal(int operand, string gpr)
{
  // cout << as->getLocationCounter() << "\n";
  if (as->dohvPrviProlaz())
  {
    dodajLiteralUBazen(operand);
    as->incLocationCounterForN(8);
  }
  else
  {
    int reg = stoi(gpr);
    string hexreg = convertIntToHex(reg);

    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("2");
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");

    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(operand) - as->getLocationCounter();
    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }

    // then check if binary can fit in 12 bits
    // if can convert binary in hex digits 4 by 4
    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("3");
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->incLocationCounterForN(8);
  }
}
void dodajLdHex(string operand, string gpr) // oblik 0x....
{
  int op = strtol(operand.c_str(), NULL, 0);
  
  if (as->dohvPrviProlaz())
  {

    dodajLiteralUBazen(op);
    as->incLocationCounterForN(8);
  }
  else
  {

    int reg = stoi(gpr);
    string hexreg = convertIntToHex(reg);

    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("2");
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
   
    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(op) - as->getLocationCounter();
    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string();

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }

    // then check if binary can fit in 12 bits
    // if can convert binary in hex digits 4 by 4
    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("3");
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
    as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");

    as->incLocationCounterForN(8);
  }
}
void dodajLdSimbol(string operand, string gpr)
{
  
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(8);
  }
  else
  {
    // dohv vrednost simbola iz tabele simbola
    if (as->doesSymbolAlreadyExistsInSymbolTable(operand))
    {
      int vrednostSimbola = as->getSymbolValueBasedOnSymbolName(operand);
      
      // ne postoji ulaz u bazenu alociraj prostor
        int reg = stoi(gpr);
        string hexreg = convertIntToHex(reg);

        as->dohvTekucaSekcija()->dodajPolaBajta("9");
        as->dohvTekucaSekcija()->dodajPolaBajta("2");
        as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
        as->dohvTekucaSekcija()->dodajPolaBajta("f");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");

        Simbol simbol = as->getSymbolBasedOnSymbolName(operand);
        long lokacija=as->dohvTekucaSekcija()->dohvVelicinuBazenaLiteralaZaDatuSekciju(vrednostSimbola);
        if (simbol.bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, lokacija, "R_x86_64_32", operand, 0);
        }
        else
        {
          if (simbol.bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, lokacija, "R_x86_64_32", simbol.sekcija, vrednostSimbola);
          }
        }
        
        long offset = lokacija - as->getLocationCounter();
        // covert offset in binary

        string binary = std::bitset<12>(offset).to_string();

        
        char *char_array = new char[binary.length() + 1];

        // make sure that the new string is null terminated
        char_array[binary.length()] = '\0';

        for (int i = 0; i < binary.length(); i++)
        {
          char_array[i] = binary[i];
        }
        // char* cifra=new char[4];
        for (int i = 0; i < binary.size();)
        {
          char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
          
          string d = convertBinToHex(cifra);
          as->dohvTekucaSekcija()->dodajPolaBajta(d);
          i += 4;
        }
        as->incLocationCounterForN(4);
        // then check if binary can fit in 12 bits
        // if can convert binary in hex digits 4 by 4
        as->dohvTekucaSekcija()->dodajPolaBajta("9");
        as->dohvTekucaSekcija()->dodajPolaBajta("3");
        as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
        as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);
        as->dohvTekucaSekcija()->dodajPolaBajta("0");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");

        as->incLocationCounterForN(4);
      

      // ako ne postoji dodati vrednost tog simbola u bazen literala i ugraditi pomeraj do toga, pomeraj je velicina
      // bazena literala za tu sekciju
      // napraviti f-ju koja dohvata velicinu bazena literala
    }
    else
    {
      cout << "simbol: " << operand << " ne postoji u tabeli simbola!"
           << "\n";
    }
  }
}
void dodajLdRegDir(string reg1, string reg2)
{
  
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg = stoi(reg2);
    string gpr = convertIntToHex(reg);
    int r = stoi(reg1);
    string operand = convertIntToHex(r);

    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("1");
    as->dohvTekucaSekcija()->dodajPolaBajta(gpr);
    as->dohvTekucaSekcija()->dodajPolaBajta(operand);
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->incLocationCounterForN(4);
  }
}
void dodajLdRegInd(string reg1, string reg2)
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg = stoi(reg2);
    string gpr = convertIntToHex(reg);
    int r = stoi(reg1);
    string operand = convertIntToHex(r);

    as->dohvTekucaSekcija()->dodajPolaBajta("9");
    as->dohvTekucaSekcija()->dodajPolaBajta("2");
    as->dohvTekucaSekcija()->dodajPolaBajta(gpr);
    as->dohvTekucaSekcija()->dodajPolaBajta(operand);
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->incLocationCounterForN(4);
  }
}
void dodajLdRegistarDecimalLiteral(string reg1, int literal, string reg2)
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    if (kolikoJeBitovaZapravoPotrebno(literal) > 12)
    {
      // postaviti neki error fleg i prijaviti gresku u procesu asembliranja
      cout << "GRESKAAA";
    }
    else
    {
      int reg = stoi(reg2);
      string gprA = convertIntToHex(reg);
      int r = stoi(reg1);
      string gprB = convertIntToHex(r);
      as->dohvTekucaSekcija()->dodajPolaBajta("9");
      as->dohvTekucaSekcija()->dodajPolaBajta("2");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprA);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprB);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");

      string binary = std::bitset<12>(literal).to_string(); // offset do literala u bazenu konvertuje u binary
      char *char_array = new char[binary.length() + 1];

      // make sure that the new string is null terminated
      char_array[binary.length()] = '\0';

      for (int i = 0; i < binary.length(); i++)
      {
        char_array[i] = binary[i];
      }
      // char* cifra=new char[4];
      for (int i = 0; i < binary.size();)
      {
        char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
        
        string d = convertBinToHex(cifra);
        as->dohvTekucaSekcija()->dodajPolaBajta(d);
        i += 4;
      }
      as->incLocationCounterForN(4);
    }
  }
}
void dodajLdRegistarHexLiteral(string reg1, string hexliteral, string reg2)
{

  int op = strtol(hexliteral.c_str(), NULL, 0);

  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    if (kolikoJeBitovaZapravoPotrebno(op) > 12)
    {
      // postaviti neki error fleg i prijaviti gresku u procesu asembliranja
      cout << "GRESKAAA";
    }
    else
    {
      int reg = stoi(reg2);
      string gprA = convertIntToHex(reg);
      int r = stoi(reg1);
      string gprB = convertIntToHex(r);
      as->dohvTekucaSekcija()->dodajPolaBajta("9");
      as->dohvTekucaSekcija()->dodajPolaBajta("2");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprA);
      as->dohvTekucaSekcija()->dodajPolaBajta(gprB);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");

      string binary = std::bitset<12>(op).to_string(); // offset do literala u bazenu konvertuje u binary
      char *char_array = new char[binary.length() + 1];

      // make sure that the new string is null terminated
      char_array[binary.length()] = '\0';

      for (int i = 0; i < binary.length(); i++)
      {
        char_array[i] = binary[i];
      }
      // char* cifra=new char[4];
      for (int i = 0; i < binary.size();)
      {
        char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
        
        string d = convertBinToHex(cifra);
        as->dohvTekucaSekcija()->dodajPolaBajta(d);
        i += 4;
      }
      as->incLocationCounterForN(4);
    }
  }
}
void dodajLdRegistarSimbol(string reg1, string simbol, string reg2)
{
  cout << "greska u procesu asembliranja, nije dozvoljeno reg + simbol za ld\n";
  // prijaviti gresku u procesu asembliranja

  as->incLocationCounterForN(4);
}
void dodajStDecimalLiteral(string reg, int decimalliteral)
{
  if (as->dohvPrviProlaz())
  {
    dodajLiteralUBazen(decimalliteral);
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg1 = stoi(reg);
    string gpr = convertIntToHex(reg1);

    as->dohvTekucaSekcija()->dodajPolaBajta("8");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta(gpr);

    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(decimalliteral) - as->getLocationCounter();
    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

   
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajStHexLiteral(string reg, string hexliteral)
{
  int op = strtol(hexliteral.c_str(), NULL, 0);
  if (as->dohvPrviProlaz())
  {
    dodajLiteralUBazen(op);
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg1 = stoi(reg);
    string gpr = convertIntToHex(reg1);

    as->dohvTekucaSekcija()->dodajPolaBajta("8");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("f");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta(gpr);

    long offset = as->dohvTekucaSekcija()->dohvLokacijuLiteralaIzBazena(op) - as->getLocationCounter();
    // covert offset in binary

    string binary = std::bitset<12>(offset).to_string(); // offset do literala u bazenu konvertuje u binary

    
    char *char_array = new char[binary.length() + 1];

    // make sure that the new string is null terminated
    char_array[binary.length()] = '\0';

    for (int i = 0; i < binary.length(); i++)
    {
      char_array[i] = binary[i];
    }
    // char* cifra=new char[4];
    for (int i = 0; i < binary.size();)
    {
      char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
      
      string d = convertBinToHex(cifra);
      as->dohvTekucaSekcija()->dodajPolaBajta(d);
      i += 4;
    }
    as->incLocationCounterForN(4);
  }
}
void dodajStSimbol(string reg, string simbol)
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    // dohv vrednost simbola iz tabele simbola
    if (as->doesSymbolAlreadyExistsInSymbolTable(simbol))
    {
      int vrednostSimbola = as->getSymbolValueBasedOnSymbolName(simbol);
      
       // ne postoji ulaz u bazenu alociraj prostor
        int gpr = stoi(reg);
        string hexreg = convertIntToHex(gpr);

        as->dohvTekucaSekcija()->dodajPolaBajta("8");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");
        as->dohvTekucaSekcija()->dodajPolaBajta("f");
        as->dohvTekucaSekcija()->dodajPolaBajta("0");
        as->dohvTekucaSekcija()->dodajPolaBajta(hexreg);

        Simbol s = as->getSymbolBasedOnSymbolName(simbol);
        long lokacija=as->dohvTekucaSekcija()->dohvVelicinuBazenaLiteralaZaDatuSekciju(vrednostSimbola);
        if (s.bind == "GLOBAL")
        {
          as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime,lokacija , "R_x86_64_32", simbol, 0);
        }
        else
        {
          if (s.bind == "LOCAL")
          {
            as->dohvTekucaSekcija()->dodajURealokacionuTabelu(as->dohvTekucaSekcija()->ime, lokacija, "R_x86_64_32", s.sekcija, vrednostSimbola);
          }
        }
       
        long offset = lokacija - as->getLocationCounter();
        // covert offset in binary

        string binary = std::bitset<12>(offset).to_string();

        
        char *char_array = new char[binary.length() + 1];

        // make sure that the new string is null terminated
        char_array[binary.length()] = '\0';

        for (int i = 0; i < binary.length(); i++)
        {
          char_array[i] = binary[i];
        }
        // char* cifra=new char[4];
        for (int i = 0; i < binary.size();)
        {
          char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
          
          string d = convertBinToHex(cifra);
          as->dohvTekucaSekcija()->dodajPolaBajta(d);
          i += 4;
        }
        as->incLocationCounterForN(4);
        // then check if binary can fit in 12 bits
        // if can convert binary in hex digits 4 by 4
      

      // ako ne postoji dodati vrednost tog simbola u bazen literala i ugraditi pomeraj do toga, pomeraj je velicina
      // bazena literala za tu sekciju
      // napraviti f-ju koja dohvata velicinu bazena literala
    }
    else
    {
      cout << "simbol: " << simbol << " ne postoji u tabeli simbola!"
           << "\n";
    }
  }
}
void dodajStRegInd(string reg1, string reg2)
{

  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    int reg = stoi(reg1);
    string gprC = convertIntToHex(reg);
    int regb = stoi(reg2);
    string gprA = convertIntToHex(regb);

    as->dohvTekucaSekcija()->dodajPolaBajta("8");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta(gprA);
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta(gprC);
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");
    as->dohvTekucaSekcija()->dodajPolaBajta("0");

    as->incLocationCounterForN(4);
  }
}
void dodajStRegistarDecimalLiteral(string reg1, string reg2, int decimalliteral)
{
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    if (kolikoJeBitovaZapravoPotrebno(decimalliteral) > 12)
    {
      // postaviti neki error fleg i prijaviti gresku u procesu asembliranja
      cout << "GRESKAAA";
    }
    else
    {
      int reg = stoi(reg2);
      string gprA = convertIntToHex(reg);
      int r = stoi(reg1);
      string gprC = convertIntToHex(r);

      as->dohvTekucaSekcija()->dodajPolaBajta("8");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprA);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprC);

      string binary = std::bitset<12>(decimalliteral).to_string(); // offset do literala u bazenu konvertuje u binary
      char *char_array = new char[binary.length() + 1];

      // make sure that the new string is null terminated
      char_array[binary.length()] = '\0';

      for (int i = 0; i < binary.length(); i++)
      {
        char_array[i] = binary[i];
      }
      // char* cifra=new char[4];
      for (int i = 0; i < binary.size();)
      {
        char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
        
        string d = convertBinToHex(cifra);
        as->dohvTekucaSekcija()->dodajPolaBajta(d);
        i += 4;
      }
      as->incLocationCounterForN(4);
    }
  }
}
void dodajStRegistarHexLiteral(string reg1, string reg2, string hexliteral)
{

  int op = strtol(hexliteral.c_str(), NULL, 0);
  if (as->dohvPrviProlaz())
  {
    as->incLocationCounterForN(4);
  }
  else
  {
    if (kolikoJeBitovaZapravoPotrebno(op) > 12)
    {
      // postaviti neki error fleg i prijaviti gresku u procesu asembliranja
      cout << "GRESKAAA";
    }
    else
    {
      int reg = stoi(reg2);
      string gprA = convertIntToHex(reg);
      int r = stoi(reg1);
      string gprC = convertIntToHex(r);

      as->dohvTekucaSekcija()->dodajPolaBajta("8");
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprA);
      as->dohvTekucaSekcija()->dodajPolaBajta("0");
      as->dohvTekucaSekcija()->dodajPolaBajta(gprC);

      string binary = std::bitset<12>(op).to_string(); // offset do literala u bazenu konvertuje u binary
      char *char_array = new char[binary.length() + 1];

      // make sure that the new string is null terminated
      char_array[binary.length()] = '\0';

      for (int i = 0; i < binary.length(); i++)
      {
        char_array[i] = binary[i];
      }
      // char* cifra=new char[4];
      for (int i = 0; i < binary.size();)
      {
        char cifra[4] = {char_array[i], char_array[i + 1], char_array[i + 2], char_array[i + 3]};
        
        string d = convertBinToHex(cifra);
        as->dohvTekucaSekcija()->dodajPolaBajta(d);
        i += 4;
      }
      as->incLocationCounterForN(4);
    }
  }
}
void dodajStRegistarSimbol(string reg1, string reg2, string simbol)
{
  cout << "\n"
       << reg1 << " " << reg2 << " " << simbol << "\n";
  as->incLocationCounterForN(4);
}
void dodajLiteralUBazen(long literal)
{
  // proveriti da li postoji ulaz u bazenu literala

  if (as->getSectionTable().back()->daLiPostojiUlazUBazenu(literal))
  {
  }
  else
  {
    as->getSectionTable().back()->dodajUlazUBazen(literal);
  }
}
int kolikoJeBitovaZapravoPotrebno(int n)
{
  return (32 - __builtin_clz(n));
}
bool dohvPrviProlaz2()
{
  return as->dohvPrviProlaz();
}
string convertBinToHex(char *binzapis)
{
  if (strcmp(binzapis, "0000") == 0)
  {
    return "0";
  }
  if (strcmp(binzapis, "0001") == 0)
  {
    return "1";
  }
  if (strcmp(binzapis, "0010") == 0)
  {
    return "2";
  }
  if (strcmp(binzapis, "0011") == 0)
  {
    return "3";
  }
  if (strcmp(binzapis, "0100") == 0)
  {
    return "4";
  }
  if (strcmp(binzapis, "0101") == 0)
  {
    return "5";
  }
  if (strcmp(binzapis, "0110") == 0)
  {
    return "6";
  }
  if (strcmp(binzapis, "0111") == 0)
  {
    return "7";
  }
  if (strcmp(binzapis, "1000") == 0)
  {
    return "8";
  }
  if (strcmp(binzapis, "1001") == 0)
  {
    return "9";
  }
  if (strcmp(binzapis, "1010") == 0)
  {
    return "a";
  }
  if (strcmp(binzapis, "1011") == 0)
  {
    return "b";
  }
  if (strcmp(binzapis, "1100") == 0)
  {
    return "c";
  }
  if (strcmp(binzapis, "1101") == 0)
  {
    return "d";
  }
  if (strcmp(binzapis, "1110") == 0)
  {
    return "e";
  }
  if (strcmp(binzapis, "1111") == 0)
  {
    return "f";
  }
  else
  {
    return "greska";
  }
}
string convertIntToHex(int broj)
{
  if (broj <= 9)
  {
    return to_string(broj);
  }
  else
  {
    if (broj == 10)
    {
      return "a";
    }
    if (broj == 11)
    {
      return "b";
    }
    if (broj == 12)
    {
      return "c";
    }
    if (broj == 13)
    {
      return "d";
    }
    if (broj == 14)
    {
      return "e";
    }
    if (broj == 15)
    {
      return "f";
    }
    else
    {
      cout << "greska: indeks registra moze biti do 15!"
           << "\n";
    }
  }
  return "error";
}
