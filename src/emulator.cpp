#include "../inc/emulator.hpp"

bool iret=false;
bool ldMem=false;
void Emulator::izvrsiInstrukciju(string oc,string mod,string regA,string regB,string regC,string D){
  
  if(oc=="0" && mod=="0" && regA=="0" && regB=="0" && regC=="0" && D=="000"){

    cout<<"halt "<<endl;
    cout<<"r1= "<<"0x"<<std::setfill('0') << std::setw(8) << std::hex<<r1<<endl;
    cout<<"r2= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r2<<endl;
    cout<<"r3= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r3<<endl;
    cout<<"r4= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r4<<endl;
    cout<<"r5= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r5<<endl;
    cout<<"r6= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r6<<endl;
    cout<<"r7= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r7<<endl;
    cout<<"r8= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r8<<endl;
    cout<<"r9= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r9<<endl;
    cout<<"r10= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r10<<endl;
    cout<<"r11= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r11<<endl;
    cout<<"r12= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r12<<endl;
    cout<<"r13= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r13<<endl;
    cout<<"r14= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r14<<endl;
    cout<<"r15= "<<"0x"<<std::setfill('0')<< std::setw(8)<<r15<<endl;
    cout<<"status= "<<"0x"<<std::setfill('0')<< std::setw(8)<<status<<endl;
    cout<<"handlers= "<<"0x"<<std::setfill('0')<< std::setw(8)<<handler<<endl;
    cout<<"cause= "<<"0x"<<std::setfill('0')<< std::setw(8)<<cause<<endl;
    setFleg(false);
  }
  if(oc=="1" && mod=="0" && regA=="0" && regB=="0" && regC=="0" && D=="000"){
    cout<<"int ";
    r14-=4;
    std::stringstream stream;
    stream<<std::hex<<status;
    std::string result(stream.str());
    memorija[r14]=result;
    
    r14-=4;
    std::stringstream stream1;
    stream1<<std::hex<<r15;
    std::string result1(stream1.str());
    memorija[r14]=result1;
    
    cause=4;
    status=status & (~0x1);
    r15=handler;
  }
  if(oc=="9" && mod=="6" && regA=="0" && regB=="e" && regC=="0" && D=="004"){
    cout<<"iret prvi deo ";
    iret=true;
  }
  if(iret && oc=="9" && mod=="3" && regA=="f" && regB=="e" && regC=="0" && D=="008"){
    cout<<"iret obaa ";
    //u memoriji je string koji je hex njega treba da konvertujem u long
    
    r15=strtol((memorija[r14]).c_str(),NULL,16);
    r14+=4;
    status=strtol((memorija[r14]).c_str(),NULL,16);
    r14+=4;
  iret=false;
  }
  if(oc=="2" && mod=="1" && regA=="f" && regB=="0" && regC=="0"){
    // D je pomeraj to operanda koji je u bazenu literala
    cout<<"call ";
     r14-=4;
    std::stringstream stream;
    stream<<std::hex<<r15;
    std::string result(stream.str());
    memorija[r14]=result;
    long offsetDoBazena=strtol(D.c_str(),NULL,16);
    long pomeraj=r15-4+offsetDoBazena;
    string little_endian=memorija[pomeraj];
    const int length = little_endian.length();
    char* char_array = new char[length + 1];
    char* big=new char[length+1];
  strcpy(char_array, little_endian.c_str());
  big[6]=char_array[0];
  big[7]=char_array[1];
  big[4]=char_array[2];
  big[5]=char_array[3];
  big[2]=char_array[4];
  big[3]=char_array[5];
  big[0]=char_array[6];
  big[1]=char_array[7];
  big[length]='\0';


    long big_endian=strtol(big,NULL,16);
    
    
    r15=big_endian;
  }
  if(oc=="9" && mod=="3" && regA=="f" && regB=="e" && regC=="0" && D=="000"){
    cout<<"ret ";
    r15=strtol((memorija[r14]).c_str(),NULL,16);
    r14+=4;
    
  }
  if(oc=="3" && mod=="8" && regA=="f" && regB=="0" && regC=="0"){
    // D je pomeraj to operanda koji je u bazenu literala
    cout<<"jmp ";
    long offsetDoBazena=strtol(D.c_str(),NULL,16);
    long pomeraj=r15-4+offsetDoBazena;
    string little_endian=memorija[pomeraj];
    const int length = little_endian.length();
    char* char_array = new char[length + 1];
    char* big=new char[length+1];
  strcpy(char_array, little_endian.c_str());
  big[6]=char_array[0];
  big[7]=char_array[1];
  big[4]=char_array[2];
  big[5]=char_array[3];
  big[2]=char_array[4];
  big[3]=char_array[5];
  big[0]=char_array[6];
  big[1]=char_array[7];
  big[length]='\0';


    long big_endian=strtol(big,NULL,16);
    
    r15=big_endian;
  }
  if(oc=="3" && mod=="9" && regA=="f"){//regb regc i D
    cout<<"beq ";
    long* gpr1=getRegister(regB);
    long* gpr2=getRegister(regC);
    if(*gpr1==*gpr2){
      long offsetDoBazena=strtol(D.c_str(),NULL,16);
    long pomeraj=r15-4+offsetDoBazena;
    string little_endian=memorija[pomeraj];
    const int length = little_endian.length();
    char* char_array = new char[length + 1];
    char* big=new char[length+1];
    strcpy(char_array, little_endian.c_str());
    big[6]=char_array[0];
    big[7]=char_array[1];
    big[4]=char_array[2];
    big[5]=char_array[3];
    big[2]=char_array[4];
    big[3]=char_array[5];
    big[0]=char_array[6];
    big[1]=char_array[7];
    big[length]='\0';


    long big_endian=strtol(big,NULL,16);
    r15=big_endian;

    }
    
  }
  if(oc=="3" && mod=="a" && regA=="f"){
    cout<<"bne ";
    long* gpr1=getRegister(regB);
    long* gpr2=getRegister(regC);
    if(*gpr1!=*gpr2){
      long offsetDoBazena=strtol(D.c_str(),NULL,16);
    long pomeraj=r15-4+offsetDoBazena;
    string little_endian=memorija[pomeraj];
    const int length = little_endian.length();
    char* char_array = new char[length + 1];
    char* big=new char[length+1];
    strcpy(char_array, little_endian.c_str());
    big[6]=char_array[0];
    big[7]=char_array[1];
    big[4]=char_array[2];
    big[5]=char_array[3];
    big[2]=char_array[4];
    big[3]=char_array[5];
    big[0]=char_array[6];
    big[1]=char_array[7];
    big[length]='\0';


    long big_endian=strtol(big,NULL,16);
    r15=big_endian;

    }
  }
  if(oc=="3" && mod=="b" && regA=="f"){
    cout<<"bgt ";
    long* gpr1=getRegister(regB);
    long* gpr2=getRegister(regC);
    if(*gpr1>*gpr2){
      long offsetDoBazena=strtol(D.c_str(),NULL,16);
    long pomeraj=r15-4+offsetDoBazena;
    string little_endian=memorija[pomeraj];
    const int length = little_endian.length();
    char* char_array = new char[length + 1];
    char* big=new char[length+1];
    strcpy(char_array, little_endian.c_str());
    big[6]=char_array[0];
    big[7]=char_array[1];
    big[4]=char_array[2];
    big[5]=char_array[3];
    big[2]=char_array[4];
    big[3]=char_array[5];
    big[0]=char_array[6];
    big[1]=char_array[7];
    big[length]='\0';


    long big_endian=strtol(big,NULL,16);
    r15=big_endian;

    }
  }
  if(oc=="8" && mod=="1" && regA=="e" && regB=="0" && D=="ffc"){
    long* gprc=getRegister(regC);
    cout<<"push ";
    r14-=4;
    std::stringstream stream;
    stream<<std::hex<<*gprc;
    std::string result1(stream.str());
    memorija[r14]=result1;
   
  }
  if(oc=="9" && mod=="3" && regB=="e" && regC=="0" && D=="004"){//radiii
    cout<<"pop "<<endl;
    long* reg=getRegister(regA);
    long v=strtol((memorija[r14]).c_str(),NULL,16);
    *reg=v;
    r14+=4;
//ovde je problem u ret ima isti kod ko pop
  }
  if(oc=="4" && mod=="0" && regA=="0"){
    cout<<"xchg ";
     long* gprD=getRegister(regB);
     long* gprS=getRegister(regC);
    long temp=*gprD;
    *gprD=*gprS;
    *gprS=temp;
  }
  if(oc=="5" && mod=="0"){
    cout<<"add ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)+(*c);
    
  }
  if(oc=="5" && mod=="1"){
    cout<<"sub ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)-(*c);
  }
  if(oc=="5" && mod=="2"){
    cout<<"mul ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)*(*c);
  }
  if(oc=="5" && mod=="3"){
    cout<<"div ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)/(*c);
  }
  if(oc=="6" && mod=="0" && regC=="0" && D=="000"){
    cout<<"not ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    *a=~(*b);
  }
  if(oc=="6" && mod=="1" && D=="000"){
    cout<<"and ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)&(*c);
  }
  if(oc=="6" && mod=="2" && D=="000"){
    cout<<"or ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)|(*c);
  }
  if(oc=="6" && mod=="3" && D=="000"){
    cout<<"xor ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)^(*c);
  }
  if(oc=="7" && mod=="0" && D=="000"){
    cout<<"shl ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)<<(*c);
  }
  if(oc=="7" && mod=="1" && D=="000"){
    cout<<"shr ";
    long* a=getRegister(regA);
    long* b=getRegister(regB);
    long* c=getRegister(regC);
    *a=(*b)>>(*c);
  }
  if(oc=="9" && mod=="2" && regB=="f" && D!="000"){//videti sta cu kad se ucita sledeca za ld
    cout<<"ld prvi put";
    ldMem=true;
    long* gpr=getRegister(regA);
    long offsetDoBazena=strtol(D.c_str(),NULL,16);
    long pomeraj=r15-4+offsetDoBazena;
    string little_endian=memorija[pomeraj];
    const int length = little_endian.length();
    char* char_array = new char[length + 1];
    char* big=new char[length+1];
    strcpy(char_array, little_endian.c_str());
    big[6]=char_array[0];
    big[7]=char_array[1];
    big[4]=char_array[2];
    big[5]=char_array[3];
    big[2]=char_array[4];
    big[3]=char_array[5];
    big[0]=char_array[6];
    big[1]=char_array[7];
    big[length]='\0';

    long big_endian=strtol(big,NULL,16);
    
    *gpr=big_endian;
    ldpomocni=big_endian;

  }
  if(oc=="9" && mod=="3" && regA==regB && ldMem){
     cout<<"ld drugi put ";
    long* gpr=getRegister(regA);
    
    string big_endian=memorija[ldpomocni];//ovde mesa big i little endian_____
    

    long big=strtol(big_endian.c_str(),NULL,16);
    *gpr=big;
    ldMem=false;
    
  }
  if(oc=="9" && mod=="1" && regC=="0" && D=="000"){
    //ld registarsko direktno
    cout<<"ld reg ind ";
    long* gpr=getRegister(regA);
    long* operand=getRegister(regB);
    *gpr=*operand;
  }
  if(oc=="9" && mod=="2" && regC=="0" && D=="000"){
    //ld registarsko indirektno
    long* gprA=getRegister(regA);
    long* gprB=getRegister(regB);
    string operand=memorija[*gprB];
    *gprA=strtol(operand.c_str(),NULL,16);
  }
  if(oc=="9" && mod=="2" && regB!="f" && regC=="0" && D!="000"){
    cout<<"LD [REG + HEXLITERAL]"<<endl;
    long* gprA=getRegister(regA);
    long* gprB=getRegister(regB);
    

    long literal=strtol((D).c_str(),NULL,16);
    
    *gprA=strtol((memorija[*gprB + literal]).c_str(),NULL,16);
  }
  if(oc=="8" && mod=="0" && regA=="f" && D!="000"){//videti sta cu kad se ucita sledeca za ld
    cout<<"st ";
    long* gprC=getRegister(regC);

    long offsetDoBazena=strtol(D.c_str(),NULL,16);
    long pomeraj=r15-4+offsetDoBazena;
    string little_endian=memorija[pomeraj];
    const int length = little_endian.length();
    char* char_array = new char[length + 1];
    char* big=new char[length+1];
    strcpy(char_array, little_endian.c_str());
    big[6]=char_array[0];
    big[7]=char_array[1];
    big[4]=char_array[2];
    big[5]=char_array[3];
    big[2]=char_array[4];
    big[3]=char_array[5];
    big[0]=char_array[6];
    big[1]=char_array[7];
    big[length]='\0';
    long dodatak=strtol(big,NULL,16);
    std::stringstream stream;
    stream<<std::hex<<*gprC;
    std::string result(stream.str());
    memorija[dodatak]=result;
  }
  if(oc=="8" && mod=="0" && D=="000"){
    //st registarsko indirektno
    long* gprA=getRegister(regA);
    long* gprC=getRegister(regC);
    std::stringstream stream;
    stream<<std::hex<<*gprC;
    std::string result(stream.str());
    memorija[*gprA]=result;
  }
  if(oc=="8" && mod=="0" && regA!="f" && D!="000"){
    //st reg + literal
    long* gprA=getRegister(regA);
    long* gprC=getRegister(regC);
    long offsetDoBazena=strtol(D.c_str(),NULL,16);
    long pomeraj=r15-4+offsetDoBazena;
    string little_endian=memorija[pomeraj];
    const int length = little_endian.length();
    char* char_array = new char[length + 1];
    char* big=new char[length+1];
    strcpy(char_array, little_endian.c_str());
    big[6]=char_array[0];
    big[7]=char_array[1];
    big[4]=char_array[2];
    big[5]=char_array[3];
    big[2]=char_array[4];
    big[3]=char_array[5];
    big[0]=char_array[6];
    big[1]=char_array[7];
    big[length]='\0';


    long literal=strtol(big,NULL,16);
    std::stringstream stream;
    stream<<std::hex<<*gprC;
    std::string result(stream.str());
    memorija[*gprA+literal]=result;

  }
  if(oc=="9" && mod=="0" && regC=="0" && D=="000"){
    cout<<"csrrd ";
    long* a=getRegister(regA);
    long* b=getStatusRegister(regB);
    *a=(*b);
  }
  if(oc=="9" && mod=="4" && regC=="0" && D=="000"){
    cout<<"csrwr ";
    long* a=getStatusRegister(regA);
    long* b=getRegister(regB);
    *a=(*b);
  }

}
void Emulator::izvrsavaj(){
  string oc;
  string mod;
  string regA;
  string regB;
  string regC;
  string D;
  string d1;
  string d2;
  string d3;
  string instrukcija;
  while(getFleg()){

    instrukcija=dohvatiIzMemorije(r15);
    r15+=4;
    
    const int length = instrukcija.length();
    char* char_array = new char[length + 1];
    strcpy(char_array, instrukcija.c_str());
    oc=char_array[0]+'\0';
    mod=char_array[1]+'\0';
    regA=char_array[2]+'\0';
    regB=char_array[3]+'\0';
    regC=char_array[4]+'\0';
    d1=char_array[5]+'\0';
    d2=char_array[6]+'\0';
    d3=char_array[7]+'\0';
    D=d1+d2+d3;
    izvrsiInstrukciju(oc,mod,regA,regB,regC,D);
    
  /*if(oc=="0"){//OVO IZMN=ENITI KAD SE DODA HALT
    
  }*/
  }
}
void Emulator::ucitajZapisUMemoriju(vector<string>zapis){
  
  string adresa=zapis[0];
  string podatak=zapis[1];
  ucitaj(adresa,podatak);
}
void Emulator::ispisiMemoriju(){
  map<long,string> mem=getMap();
  for (auto itr = mem.begin(); itr != mem.end(); ++itr) {
        cout << itr->first<< ' ' << itr->second << '\n';
    }
}
void Emulator::ucitajIzUlaznogFajla(char* imeFajla){
  ifstream ulaz;
  ulaz.open(imeFajla);
  string linija;
  
  while(getline(ulaz,linija)){
    linija+='\n';
   
    const int length = linija.length();
    char* char_array = new char[length + 1];
    strcpy(char_array, linija.c_str());
    char * p;
    vector<string> zapis;
    p = strtok(char_array, ":"); 
    while (p != NULL) {//izdvoji token sa sve |
        string token=p;
        if(token.back()=='\n'){
          string promenjeno=token.substr(0, token.size()-1);
           zapis.push_back(promenjeno);
        }else{
           zapis.push_back(token);
        }
       

        p = strtok(NULL, ":");
    }
    ucitajZapisUMemoriju(zapis);
  }
}
int main(int argc,char* argv[]){
  char* imeUlaznogFajla=argv[1];
  char* p1=strstr(argv[1],".hex");
  if(p1==nullptr){
    cout<<"Ulazni fajl mora da bude .hex!"<<endl;
  }else{
    Emulator* e=new Emulator("0x40000000");
    e->ucitajIzUlaznogFajla(imeUlaznogFajla);
    e->izvrsavaj();
    }
}