%{
  #include "inc/pomocni.h"
  #include "parser.tab.h"
%}

%option noyywrap


%%

"#"[^\n\r]* { return KOMENTAR; }
("."global) { return GLOBAL; }
("."extern) { return EXTERN; }
("."section) { return SECTION; }
("."word) { return WORD; }
("."skip) { return SKIP; }
("."end) { return END; }
([a-zA-Z][a-zA-Z0-9_]*":")$ { yylval.sym=copy_string(yytext);return LABELA; }
(halt|int|iret|ret) {yylval.instrukcija=copy_string(yytext); return NBEZARG;}
(beq|bne|bgt) {yylval.instrukcija=copy_string(yytext); return BRANCH;}
(call) { return CALL;}
(jmp) { return JMP;}
(push|pop|not) {yylval.instrukcija=copy_string(yytext); return NJEDANARG;}
(xchg|add|sub|mul|div|and|or|xor|shl|shr) {yylval.instrukcija=copy_string(yytext); return NDVAARG;}
(csrrd) {yylval.instrukcija=copy_string(yytext); return CSRRD;}
(csrwr) {yylval.instrukcija=copy_string(yytext); return CSRWR;}
(ld) {yylval.instrukcija=copy_string(yytext); return LD;}
(st) {yylval.instrukcija=copy_string(yytext); return ST;}
(("%"r)([1-9]|1[0-5])) {  yylval.brregistra = copy_string(yytext+2); return REGISTAR; }
(("%")(sp|pc)) {  yylval.brregistra = copy_string(yytext+1); return REGISTAR; }
(("%")(status|handler|cause)) {  yylval.brregistra = copy_string(yytext+1); return PROGREGISTAR; }
[a-zA-Z_][a-zA-Z0-9_]* {yylval.sym=copy_string(yytext); return SIMBOL;}
[a-zA-Z_][a-zA-Z0-9_]*(", "[a-zA-Z_][a-zA-Z0-9_]*)+ {yylval.lista=yytext; return LISTA;}
[0-9]+ {yylval.num=atoi(yytext);return DECIMALLITERAL;}
("$"[0-9]+) {yylval.sym=copy_string(yytext+1);return LITERALIMMDECIMAL;}
("$"(0x)[0-9a-fA-F]+) {yylval.sym=copy_string(yytext+1);return LITERALIMMHEX;}
("$"[a-zA-Z_][a-zA-Z0-9_]*) {yylval.sym=copy_string(yytext+1);return SYMBOLIMM;}
((0x)[0-9a-fA-F]+) { yylval.sym=copy_string(yytext);return HEXLITERAL;}
(([0-9]+)|((0x)[0-9a-fA-F]+))(", "(([0-9]+)|((0x)[0-9a-fA-F]+)))* {yylval.lista=yytext; return LISTALITERALA;}
";" { return TACKAZAREZ; }
"[" {return LEFTBRACKET;}
"]" {return RIGHTBRACKET;}
"+" {yylval.sym=copy_string(yytext);return PLUSZNAK;}
"," { return ZAREZ; }
\n { return EOL; }
. {}

%%




