#include "../inc/linker.hpp"
Linker *l = new Linker();
void dodaj4BuSekciju(string sekcija, string linija, string file)
{
  // podeliti liniju po delimiteru space i za svaki token pozvati f-ju

  const int length = linija.length();
  char *char_array = new char[length + 1];
  strcpy(char_array, linija.c_str());
  char *p;
  // Here, the delimiter is white space.
  p = strtok(char_array, " ");
  while (p != NULL)
  { // izdvoji token sa sve |
    string token = p;
    if (token.back() == '\n')
    {
      string promenjeno = token.substr(0, token.size() - 1);
      l->findGivenSectionAndAddData(sekcija, promenjeno, file);
    }
    else
    {
      l->findGivenSectionAndAddData(sekcija, token, file);
    }

    p = strtok(NULL, " ");
  }
}
void formirajUlazUBazen(string sekcija, vector<string> ulaz, string file)
{
  UlazUBazen *u = new UlazUBazen(ulaz[0], ulaz[2]);
  l->findGivenSectionAndAddUlazUBazen(sekcija, u, file);
}
void formirajSekciju(vector<string> sekcija)
{
  string ime = sekcija[4].substr(0, sekcija[4].size() - 1);
  Sekcija *sek = new Sekcija(sekcija[0], stoi(sekcija[1]), strtol((sekcija[2]).c_str(), NULL, 0), sekcija[3], ime);
  l->addSection(sek);
}
void formirajSimbol(vector<string> simbol)
{

  Simbol *s = new Simbol(simbol[0], l->getRBR(), simbol[2], stoi(simbol[3]), simbol[4], simbol[5], simbol[6]);
  l->addSymbol(s);
}
void formirajRelokacioniZapis(string sekcija, vector<string> relok_zapis, string file)
{
  UlazURelokTabelu *u = new UlazURelokTabelu(file, sekcija, stoi(relok_zapis[0]), relok_zapis[1], relok_zapis[2], stoi(relok_zapis[3]));
  l->findGivenSectionAndAddRelokacioniZapis(sekcija, u, file);
}
void ucitajUlazUBazen(string sekcija, string linija, string file)
{
  vector<string> ulaz;

  const int length = linija.length();
  char *char_array = new char[length + 1];

  // copying the contents of the
  // string to char array
  strcpy(char_array, linija.c_str());
  char *p;
  // Here, the delimiter is white space.
  p = strtok(char_array, "\t\t");
  while (p != NULL)
  { // izdvoji token sa sve |
    string token = p;
    if (token.back() == '\n')
    {
      string promenjeno = token.substr(0, token.size() - 1);
      ulaz.push_back(promenjeno);
    }
    else
    {
      ulaz.push_back(token);
    }

    p = strtok(NULL, "\t\t");
  }
  formirajUlazUBazen(sekcija, ulaz, file);
}
void ucitajRelokacioniZapis(string sekcija, string linija, string fajl)
{
  vector<string> relok_zapis;

  const int length = linija.length();
  char *char_array = new char[length + 1];

  // copying the contents of the
  // string to char array
  strcpy(char_array, linija.c_str());
  char *p;
  // Here, the delimiter is white space.
  p = strtok(char_array, "\t");
  while (p != NULL)
  { // izdvoji token sa sve |
    string token = p;
    token.erase(remove(token.begin(), token.end(), '|'), token.end());
    relok_zapis.push_back(token);
    p = strtok(NULL, "\t");
  }
  formirajRelokacioniZapis(sekcija, relok_zapis, fajl);
}
void ucitajSekciju(string linija, char *imeFajla)
{
  string fajl = imeFajla;
  vector<string> sekcija;
  sekcija.push_back(fajl);

  const int length = linija.length();
  char *char_array = new char[length + 1];

  // copying the contents of the
  // string to char array
  strcpy(char_array, linija.c_str());
  char *p;
  // Here, the delimiter is white space.
  p = strtok(char_array, "\t");
  while (p != NULL)
  { // izdvoji token sa sve |
    string token = p;
    token.erase(remove(token.begin(), token.end(), '|'), token.end());
    sekcija.push_back(token);
    p = strtok(NULL, "\t");
  }
  formirajSekciju(sekcija);
}
void ucitajSimbol(string linija, char *imeFajla)
{ // tokeni: fajl|
  string fajl = imeFajla;
  vector<string> simbol;
  simbol.push_back(fajl);

  const int length = linija.length();
  char *char_array = new char[length + 1];

  strcpy(char_array, linija.c_str());
  char *p;
  p = strtok(char_array, "\t");
  while (p != NULL)
  { // izdvoji token sa sve |
    string token = p;
    token.erase(remove(token.begin(), token.end(), '|'), token.end());
    simbol.push_back(token);
    p = strtok(NULL, "\t");
  }
  formirajSimbol(simbol);
}
void postaviPocetneAdreseOdNule()
{
  long adresa = 0;
  vector<SekcijaLinker *> sekcije = l->getAllSections();
  for (int i = 0; i < sekcije.size(); i++)
  {
    sekcije[i]->setStartingAddress(adresa);
    adresa += sekcije[i]->getUkupnaVelicina();
    sekcije[i]->postaviSvimIstoimenimSekcijamaPocetnuAdresu();
  }
}
void postaviPocetneAdreseOdZadate(long pocetna)
{

  long adresa = pocetna;
  vector<SekcijaLinker *> sekcije = l->getAllSections();
  for (int i = 0; i < sekcije.size(); i++)
  {
    if (sekcije[i]->getPlace() == false)
    {
      sekcije[i]->setStartingAddress(adresa);
      adresa += sekcije[i]->getUkupnaVelicina();
      sekcije[i]->postaviSvimIstoimenimSekcijamaPocetnuAdresu();
    }
    cout << "ADRESE " << sekcije[i]->getStartingAddress() << " sekcija" << sekcije[i]->ime;
  }
}
void postaviVirtuelneAdreseSimbola()
{
  vector<SekcijaLinker *> sekcije = l->getAllSections();
  vector<Simbol *> simboli = l->getSymbolTable();
  for (int i = 0; i < simboli.size(); i++)
  {
    for (int j = 0; j < sekcije.size(); j++)
    {
      if (simboli[i]->sekcija == sekcije[j]->ime)
      {
        vector<Sekcija *> istoimene = sekcije[j]->getIstoimeneSekcije();
        for (int k = 0; k < istoimene.size(); k++)
        {
          if (simboli[i]->file == istoimene[k]->file)
          {
            simboli[i]->setVrednost(istoimene[k]->getPocetnaAdresa() + simboli[i]->getVrednost());
            break;
          }
        }
        break;
      }
    }
  }
}
bool proveraNedefinisanihSimbola()
{
  vector<Simbol *> undefined = l->getUndefinedSymbols();
  vector<Simbol *> symbols = l->getSymbolTable();
  for (int i = 0; i < undefined.size(); i++)
  {
    for (int j = 0; j < symbols.size(); j++)
    {
      if (undefined[i]->ime == symbols[j]->ime)
      {
        undefined[i]->setRazresen(true);
        break;
      }
    }
  }
  for (int i = 0; i < undefined.size(); i++)
  {
    if (undefined[i]->razresen == true)
    {
    }
    else
    {
      return false;
    }
  }
  return true;
}
void mapiranje()
{
  // prolazi kroz allSections pa kroz istoimene sekcije i dodaje u mapu prvo data pa bazen
  long adresa;
  vector<SekcijaLinker *> sekcije = l->getAllSections();
  for (int i = 0; i < sekcije.size(); i++)
  {
    adresa = sekcije[i]->getStartingAddress();
    for (int j = 0; j < sekcije[i]->istoimene_sekcije.size(); j++)
    {
      list<string> data = sekcije[i]->istoimene_sekcije[j]->dohvData();
      list<string>::iterator it;
      for (it = data.begin(); it != data.end(); ++it)
      {
        l->addToMap(adresa, (*it));
        adresa += 4;
      }
      list<UlazUBazen> bazen = sekcije[i]->istoimene_sekcije[j]->dohvBazen();
      list<UlazUBazen>::iterator et;
      for (et = bazen.begin(); et != bazen.end(); ++et)
      {
        l->addToMap(adresa, (*et).vrednostString);
        adresa += 4;
      }
    }
  }
}
long pronadjiPocetnuAdresu(string imeSekcije, string fajl)
{
  vector<SekcijaLinker *> sekcije = l->getAllSections();
  for (int i = 0; i < sekcije.size(); i++)
  {
    vector<Sekcija *> istoimene = sekcije[i]->getIstoimeneSekcije();
    for (int j = 0; j < istoimene.size(); j++)
    {
      if (istoimene[j]->ime == imeSekcije && istoimene[j]->file == fajl)
      {
        return istoimene[j]->getPocetnaAdresa();
      }
    }
  }
  return 0;
}
long pronadjiVrednostSimbola(string imeSimbola)
{
  vector<Simbol *> simboli = l->getSymbolTable();
  for (int i = 0; i < simboli.size(); i++)
  {
    if (simboli[i]->ime == imeSimbola)
    {
      return simboli[i]->getVrednost();
    }
  }
  return 0;
}
string convertIntToLittleEndianHex(long br)
{
  std::stringstream stream;

  stream << std::setfill('0') << std::setw(8) << std::hex << br;
  std::string result(stream.str());
  const int length = result.length();
  char *char_array = new char[length + 1];
  char *niz = new char[length + 1];
  strcpy(char_array, result.c_str());
  // for(int j=0;j<length;j+=2){
  int j = 0;
  for (int i = length - 1; i >= 1 && j < length; i -= 2, j += 2)
  {
    niz[j] = char_array[i - 1];
    niz[j + 1] = char_array[i];
  }
  //}
  niz[length] = '\0';
  string rez = niz;
  return rez;
}
void razresavanje()
{
  // prodje kroz sve all sections pa kroz istoimene pa kroz njene realokacione zapise
  // pocetnu adresu sekcije ima u istoimenim
  // izracuna P,S,A
  // vrednost simbola dohvati iz symboltable
  long P;
  long S;
  long A;
  vector<SekcijaLinker *> sekcije = l->getAllSections();
  for (int i = 0; i < sekcije.size(); i++)
  {
    vector<Sekcija *> istoimene = sekcije[i]->getIstoimeneSekcije();
    for (int j = 0; j < istoimene.size(); j++)
    {
      list<UlazURelokTabelu> relokaciona_tabela = istoimene[j]->dohvRelokTabelu();
      list<UlazURelokTabelu>::iterator et;
      for (et = relokaciona_tabela.begin(); et != relokaciona_tabela.end(); ++et)
      {
        P = istoimene[j]->getPocetnaAdresa() + (*et).offset;
        A = (*et).addend;
        // 1.slucaj ako je simbol=sekcija
        if (((*et).simbol).at(0) == '.')
        {
          // pronadji pocetnu adresu sekcije koja je ovde navedena iz istog je fajla kao i zapis
          S = pronadjiPocetnuAdresu((*et).simbol, (*et).file);
          long rez = S + A;
          string zamapu = convertIntToLittleEndianHex(rez);
          l->EditElementInMap(P, zamapu);
        }
        else
        { // simbol=simbol
          S = pronadjiVrednostSimbola((*et).simbol);
          long rez = S + A;
          string zamapu = convertIntToLittleEndianHex(rez);
          l->EditElementInMap(P, zamapu);
        }
      }
    }
  }
}
void ispisMape()
{
  map<long, string> mapa = l->getMap();
  for (auto itr = mapa.begin(); itr != mapa.end(); ++itr)
  {
    cout << itr->first << ':' << itr->second << '\n';
  }
}
void ucitajIzUlaznogFajla(char *imeFajla)
{
  ifstream ulaz;

  ulaz.open(imeFajla);

  string fajl = imeFajla;
  string linija;

  getline(ulaz, linija);
  linija += "\n";
  if (linija == "#shdr\n")
  {

    getline(ulaz, linija);
    linija += "\n";
    while (linija != "#symtab\n")
    {
      ucitajSekciju(linija, imeFajla);
      getline(ulaz, linija);
      linija += "\n";
    }
  }
  if (linija == "#symtab\n")
  {

    getline(ulaz, linija);
    linija += "\n";

    while (linija.substr(0, 6) != "#.rela")
    {
      ucitajSimbol(linija, imeFajla);
      getline(ulaz, linija);
      linija += "\n";
    }
  }

  while (linija.substr(0, 6) == "#.rela")
  {
    linija.erase(0, 6);
    string sekcija = linija.substr(0, linija.size() - 1);
    getline(ulaz, linija);
    linija += "\n";

    // ovde dohvata sekciju ciji su realokacioni zapisi
    while (linija.substr(0, 6) != "#.rela" && linija.substr(0, 4) != "#end")
    { // ovde puni realokacione zapise
      ucitajRelokacioniZapis(sekcija, linija, fajl);
      getline(ulaz, linija);
      linija += "\n";
    }
  }
  if (linija.substr(0, 4) == "#end")
  { // preskoci end
    getline(ulaz, linija);
    linija += "\n";
  }
  while (linija.substr(0, 2) == "#.")
  {
    linija.erase(0, 1);
    string sekcija = linija.substr(0, linija.size() - 1); // naziv sekcijeeee

    getline(ulaz, linija);
    linija += "\n";
    // ime sekcije cija je ovo data
    while (linija.substr(0, 7) != "#.bazen")
    {
      dodaj4BuSekciju(sekcija, linija, fajl);
      getline(ulaz, linija);
      linija += "\n";
      // data te sekcije
    }
    if (linija.substr(0, 7) == "#.bazen")
    { // puni bazen za sekciju iznad
      getline(ulaz, linija);
      linija += "\n"; // preskace natpis #.bazen

      while (linija.substr(0, 4) != "#end")
      { // ucitava liniju po liniju bazena
        ucitajUlazUBazen(sekcija, linija, fajl);
        getline(ulaz, linija);
        linija += "\n";
      }
    }
    if (linija.substr(0, 4) == "#end")
    { // preskoci to end
      getline(ulaz, linija);
      linija += "\n";
    }
  }

  ulaz.close();
}
/*./linker -hex
-place=data@0x4000F000 -place=my_data@0x40000000
-o mem_content.hex
ulaz1.o ulaz2.o*/
int main(int argc, char *argv[])
{
  ofstream izlaz;
  bool flegHex = false;
  list<char *> ulazniFajlovi;
  list<Place> places;
  /*char* imeUlaznogFajla=argv[a];*/
  char *imeIzlaznogFajla;
  char *p1;
  char *p2;

  for (int i = 0; i < argc; i++)
  {
    string argv_string = argv[i];

    p1 = strstr(argv[i], "-place=");
    p2 = strstr(argv[i], ".o");
    if (p1 != nullptr)
    { //-place
      string original = p1;
      string substring = "-place=";
      size_t ind = original.find(substring); // Find the starting position of substring in the string
      if (ind != string::npos)
      {
        original.erase(ind, substring.length());
        const int length = original.length();

        // declaring character array (+1 for null terminator)
        char *char_array = new char[length + 1];

        // copying the contents of the
        // string to char array
        strcpy(char_array, original.c_str());
        char *p;
        // Here, the delimiter is white space.
        p = strtok(char_array, "@");
        bool prviPut = true;
        Place place;
        while (p != NULL)
        {
          if (prviPut)
          {
            string sek = p;
            place.sekcija = "." + sek;
            prviPut = false;
          }
          else
          {
            string hex = p;
            place.hexAdresa = hex;
          }

          p = strtok(NULL, " ");
        }
        places.push_back(place);
      }
    }
    if (p2 != nullptr)
    { // ulazni fajlovi
      l->addInputFile(argv[i]);
    }
    if (argv_string == "-hex")
    { // ovo ne radi

      flegHex = true;
    }
    if (argv_string == "-o")
    { // ovo ne radi vrv jer je arhv char*

      imeIzlaznogFajla = argv[i + 1];

      izlaz.open(imeIzlaznogFajla);
    }
  }

  list<char *>::iterator it;
  list<char *> listaFajlova = l->getInputFiles();
  for (it = listaFajlova.begin(); it != listaFajlova.end(); ++it)
  {
    ucitajIzUlaznogFajla(*it);
  }

  vector<SekcijaLinker *> sekcije = l->getAllSections();
  for (int i = 0; i < sekcije.size(); i++)
  {
    sekcije[i]->izracunajUkupnuVelicinuSekcije();
  }
  // proveriti da li su postavljeni placeholderi
  bool place = false;
  if (places.size() == 0)
  {                                                  // nisu postavljeni placeholderi
    cout << "nisu postavljeni placeholderi" << endl; // brojanje adresa krece od nule
    place = true;
    postaviPocetneAdreseOdNule();
  }
  else
  { // postavljeni su placeholderi
    cout << "postavljeni su placeholderi" << endl;
    vector<SekcijaLinker *> setovane;
    list<Place>::iterator it;

    for (int j = 0; j < sekcije.size(); j++)
    {
      for (it = places.begin(); it != places.end(); ++it)
      {
        if ((*it).sekcija == sekcije[j]->ime)
        {
          sekcije[j]->setPlace(true);
          long op = strtol((*it).hexAdresa.c_str(), NULL, 0);
          sekcije[j]->setStartingAddress(op);
          setovane.push_back(sekcije[j]);
          break;
        }
      }
    }
    // sortiraj setovane
    std::sort(setovane.begin(), setovane.end(), [](const auto &a, const auto &b)
              { return a->pocetna_adresa < b->pocetna_adresa; });
    // dohvata pocetnu adresu za sekcije kojima nije predefinisan place
    long pocetna = setovane[setovane.size() - 1]->getStartingAddress() + setovane[setovane.size() - 1]->getUkupnaVelicina();

    cout << "SETOVANE SU SLEDECE SEKCIJE!" << endl;
    for (int i = 0; i < setovane.size(); i++)
    {
      cout << setovane[i]->ime << " " << setovane[i]->getStartingAddress() << " ";
    }
    // proverava preklapanje
    if (setovane.size() == 1)
    {
      postaviPocetneAdreseOdZadate(pocetna);
      place = true;
    }
    for (int i = 0; i < setovane.size() - 1; i++)
    {
      if (setovane[i]->getStartingAddress() + setovane[i]->getUkupnaVelicina() <= setovane[i + 1]->getStartingAddress())
      {
        // sve ok moze da se mapira
        // setovane[i]->postaviSvimIstoimenimSekcijamaPocetnuAdresu();
        place = true;
        postaviPocetneAdreseOdZadate(pocetna);
        for (int j = 0; j < setovane.size(); j++)
        {
          setovane[j]->postaviSvimIstoimenimSekcijamaPocetnuAdresu();
        }
      }
      else
      {
        place = false;
        cout << "GRESKAAA, NE MOGU DA SE MAPIRAJU SEKCIJE SA OVIM VREDNOSTIMA PLACE!" << endl;
        break;
      }
    }
  }

  cout << endl;
  cout << "UNDEFINED SYMBOLS" << endl;
  vector<Simbol *> und = l->getUndefinedSymbols();
  for (int i = 0; i < und.size(); i++)
  {
    cout << und[i]->ime << endl;
  }
  // proveriti da li postoje nedefinisani simboli
  // postaviVirtuelneAdreseSimbola();
  bool undefined = proveraNedefinisanihSimbola();
  bool visestruka_definicija = l->getVisestrukaDefinicijaFleg();
  if (flegHex && place && undefined && !visestruka_definicija)
  {
    cout << "SVE OK MAPIRAJ" << endl;
    // ispis tabele simbola

    // ima izlazni fajl, ako hex nije naveden ne generise se izlazni fajl
    mapiranje();
    postaviVirtuelneAdreseSimbola();

    // ispise tabelu simbola linkera
    vector<Simbol *> tabelaSimbola = l->getSymbolTable();
    for (int i = 0; i < tabelaSimbola.size(); i++)
    {
      cout << tabelaSimbola[i]->file << "\t" << tabelaSimbola[i]->rBr << "\t" << tabelaSimbola[i]->ime << "\t" << tabelaSimbola[i]->vrednost << "\t" << tabelaSimbola[i]->sekcija << "\t" << tabelaSimbola[i]->tip << "\t" << tabelaSimbola[i]->bind << endl;
    }
    razresavanje();
    ofstream out(imeIzlaznogFajla);
    streambuf *coutbuf = std::cout.rdbuf(); // save old buf
    cout.rdbuf(out.rdbuf());
    ispisMape();
    std::cout.rdbuf(coutbuf);

    izlaz.close();
  }
  else
  {
    if (flegHex == false)
    {
      cout << "Mora da se navede opcija -hex u komandnoj liniji!"
           << "\n";
    }
    if (!place)
    {
      cout << "Ne mogu da se mapiraju sekcije sa ovim vrednostima place-a" << endl;
    }
    if (!undefined)
    {
      cout << "Postoje nedefinisani simboli" << endl;
    }
    if (visestruka_definicija)
    {
      cout << "Postoji visestruka definicija simbola" << endl;
    }
  }
  // ispise pocetne adrese svih sekcija
  for (int i = 0; i < sekcije.size(); i++)
  {
    cout << sekcije[i]->ime << endl;

    cout << sekcije[i]->getStartingAddress() << endl;
  }

  return 0;
}

//../linker -hex -place=my_code@0x40000000 -place=math@0xF0000000 -o program.hex handler.o math.o main1.o isr_terminal.o isr_timer.o isr_software.o