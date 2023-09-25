%{
extern int yylex(void);
void yyerror(char *s){}
#include "inc/asembler.hpp"
%}



%union {
  long num;
  char* sym;
  char* brregistra;
  char* instrukcija;
  char* lista;
}

%token EOL
%token<num> NUMBER
%type <num> exp
%token PLUS
%token<brregistra> REGISTAR
%token<brregistra> PROGREGISTAR
%token TACKAZAREZ
%token <instrukcija> NBEZARG
%token <instrukcija> NJEDANARG
%token <instrukcija> NDVAARG
%token <instrukcija> CSRRD
%token <instrukcija> CSRWR
%token GLOBAL
%token<sym> SIMBOL
%token<lista> LISTA
%token<lista> LISTALITERALA
%token ZAREZ
%token EXTERN
%token SECTION
%token WORD
%token<sym> HEXLITERAL
%token<num> DECIMALLITERAL
%token SKIP
%token END
%token<sym> LABELA
%token KOMENTAR
%type direktiva
%type instrukcija
%token CALL
%token JMP
%token<instrukcija> BRANCH
%token<sym> LITERALIMMDECIMAL
%token<sym> LITERALIMMHEX
%token<instrukcija> LD
%token<instrukcija> ST
%token<sym> SYMBOLIMM
%token RIGHTBRACKET
%token LEFTBRACKET
%token<sym> PLUSZNAK

%%

input: 
 exp EOL {/**/}
| EOL;

exp: 
 NUMBER { $$ = $1; }
| KOMENTAR {}
| exp PLUS exp { $$ = $1 + $3; }
| SIMBOL { dodajSimbol($1);}
| LABELA { printf($1); dodajLabelu($1); }
| direktiva {}
| instrukcija {}
| LABELA direktiva {/* istestirati */};


direktiva:
 GLOBAL LISTA { printf("global lista simbola\n"); dodajGlobalListaSimbola($2);}
| EXTERN SIMBOL { printf("extern simbol\n"); dodajExternSimbol($2);}
| EXTERN LISTA { printf("extern lista simbola\n"); dodajExternListaSimbola($2);}
| SECTION SIMBOL { printf("%s\n",$2); dodajSekciju($2); }
| WORD SIMBOL { printf("%s\n",$2); dodajWordSimbol($2);}
| WORD DECIMALLITERAL {printf("dec literal\n");printf("%d\n",$2); dodajWordDecimalLiteral($2);}
| WORD HEXLITERAL {printf("hex literal\n");printf("%s\n",$2);dodajWordHexLiteral($2);}
| WORD LISTA { printf("word lista simbola\n"); dodajWordListaSimbola($2);}
| WORD LISTALITERALA { printf("word lista literala\n"); dodajWordListaLiterala($2);}
| SKIP DECIMALLITERAL {dodajSkipDecimal($2);printf("SKIP ");printf("%d\n",$2);}
| SKIP HEXLITERAL {dodajSkipHex($2);}
| END {dodajEnd();};

instrukcija:
 NBEZARG { dodajNBezArg($1); printf("NBEZARG\n");}
| NJEDANARG REGISTAR {dodajNJedanArgument($1,$2);}
| NDVAARG REGISTAR ZAREZ REGISTAR {dodajNDvaArgumenta($1,$2,$4);}
| CSRRD PROGREGISTAR ZAREZ REGISTAR {dodajCsrrd($1,$2,$4);}
| CSRWR REGISTAR ZAREZ PROGREGISTAR {dodajCsrwr($1,$2,$4);}
| CALL DECIMALLITERAL { dodajCallDecimal($2);}
| CALL HEXLITERAL {dodajCallHex($2);}
| CALL SIMBOL {dodajCallSimbol($2);}
| JMP DECIMALLITERAL { dodajJmpDecimal($2);printf("%d\n",$2);}
| JMP HEXLITERAL {dodajJmpHex($2);}
| JMP SIMBOL {dodajJmpSimbol($2);}
| BRANCH REGISTAR ZAREZ REGISTAR ZAREZ DECIMALLITERAL { dodajBranchDecimal($1,$2,$4,$6);}
| BRANCH REGISTAR ZAREZ REGISTAR ZAREZ HEXLITERAL { dodajBranchHex($1,$2,$4,$6);}
| BRANCH REGISTAR ZAREZ REGISTAR ZAREZ SIMBOL { dodajBranchSimbol($1,$2,$4,$6);}
| LD LITERALIMMDECIMAL ZAREZ REGISTAR { dodajLdImmDecimal($2,$4);}
| LD LITERALIMMHEX ZAREZ REGISTAR { dodajLdImmHex($2,$4);}
| LD SYMBOLIMM ZAREZ REGISTAR { dodajLdSymbolImm($2,$4);}
| LD DECIMALLITERAL ZAREZ REGISTAR {dodajLdDecimal($2,$4);}
| LD HEXLITERAL ZAREZ REGISTAR {dodajLdHex($2,$4);}
| LD SIMBOL ZAREZ REGISTAR {dodajLdSimbol($2,$4);}
| LD REGISTAR ZAREZ REGISTAR {dodajLdRegDir($2,$4);}
| LD LEFTBRACKET REGISTAR RIGHTBRACKET ZAREZ REGISTAR {dodajLdRegInd($3,$6);printf("OK");}
| LD LEFTBRACKET REGISTAR PLUSZNAK DECIMALLITERAL RIGHTBRACKET ZAREZ REGISTAR {dodajLdRegistarDecimalLiteral($3,$5,$8);}
| LD LEFTBRACKET REGISTAR PLUSZNAK HEXLITERAL RIGHTBRACKET ZAREZ REGISTAR {dodajLdRegistarHexLiteral($3,$5,$8);}
| LD LEFTBRACKET REGISTAR PLUSZNAK SIMBOL RIGHTBRACKET ZAREZ REGISTAR { dodajLdRegistarSimbol($3,$5,$8);}
| ST REGISTAR ZAREZ DECIMALLITERAL {dodajStDecimalLiteral($2,$4);}
| ST REGISTAR ZAREZ HEXLITERAL {dodajStHexLiteral($2,$4);}
| ST REGISTAR ZAREZ SIMBOL {dodajStSimbol($2,$4);}
| ST REGISTAR ZAREZ LEFTBRACKET REGISTAR RIGHTBRACKET{dodajStRegInd($2,$5);}
| ST REGISTAR ZAREZ LEFTBRACKET REGISTAR PLUSZNAK DECIMALLITERAL RIGHTBRACKET{dodajStRegistarDecimalLiteral($2,$5,$7);}
| ST REGISTAR ZAREZ LEFTBRACKET REGISTAR PLUSZNAK HEXLITERAL RIGHTBRACKET{dodajStRegistarHexLiteral($2,$5,$7);}
| ST REGISTAR ZAREZ LEFTBRACKET REGISTAR PLUSZNAK SIMBOL RIGHTBRACKET{dodajStRegistarSimbol($2,$5,$7);};

 
%%



/*int yyerror(char* s){
  printf("ERROR: %s\n",s);
  return 0;


}*/
