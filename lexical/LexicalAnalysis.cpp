#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>
#include "LexicalAnalysis.h"
#include "symbolTable.h"

using namespace std;

LexicalAnalysis::LexicalAnalysis(const char* filename, bool DEBug, SymbolTable* st) : m_line(1), symbolTable(st) {
    showPrints = DEBug;
    m_line = 1;
    if(DEBug)
        printf("Lexical: File> %s", filename);
    m_file = fopen(filename, "r");
    if (!m_file)
        throw std::string("Unable to open file: %s", filename);
        
}

LexicalAnalysis::~LexicalAnalysis() {
    fclose(m_file);
}

int LexicalAnalysis::line() const {
    return m_line;
}

struct Lexeme LexicalAnalysis::nextToken() {
    struct Lexeme lex;
    const int sb_find = -1;
    const int tkn_defined = -2;
    int state = 1;
    int c;
    while (state != sb_find && state != tkn_defined) {
        c = getc(m_file);     

        //if (showPrints)
        //    printf("\tstate: %02d, (char) '%c', %d ASCII\n", state, (char) c, c);

        switch (state) {
            case 1:{
                if(c == '\t' || c == '\r' || c == '\n' || c == ' '){
                    state = 1;
                    if(c == '\n'){
                        m_line++;
                        /*if(showPrints){
                            printf("\t\tm_lines %d\n", m_line);
                        }*/
                    }
                }else if (isdigit(c)){
                    lex.token += (char) c;
                    state = 2;
                }else if (c == '{'){
                    lex.token += (char) c;
                    state = 4;
                }else if(c == ';'
                    || c == '('
                    || c == ')'
                    || c == ','
                    || c == '!'
                    || c == '+'
                    || c == '-'
                    || c == '*'
                ){
                    lex.token += (char) c;
                    state = sb_find;
                }else if(c == '='){
                    lex.token += (char) c;
                    state = 5;
                }else if(c == '>'){
                    lex.token += (char) c;
                    state = 6;
                }else if(c == '<'){
                    lex.token += (char) c;
                    state = 7;
                }else if(c == '&'){
                    lex.token += (char) c;
                    state = 8;
                }else if(c == '|'){
                    lex.token += (char) c;
                    state = 9;
                }else if(isalpha(c)){
                    lex.token += (char) c;
                    state = 10;
                }else if(c == '/'){
                    state = 11;
                }else if(c == '.' || c == 34){
                    lex.type = TKN_INVALID_TOKEN;
                    state = tkn_defined;
                }else if(c == -1){
                    ungetc(c, m_file);
                    lex.type = TKN_END_OF_FILE;
                    state = tkn_defined;
                }
                break;
            }
            case 2:{ // get digit from until '.', else ret int
                if (isdigit(c)){ 
                    lex.token += (char) c;
                    state = 2;
                } else if (isalpha(c)){
                    ungetc(c,m_file);
                    lex.token += (char) c;
                    lex.type = TKN_INVALID_TOKEN;
                    state = tkn_defined;
                } else if (c == '.'){
                    lex.token += (char) c;
                    state = 3;
                } else {
                    ungetc(c, m_file);
                    lex.type = TKN_INT;
                    state = tkn_defined;
                }
                break;
            }
            case 3:{// after '.' get digit until other then return
                if (isdigit(c)){
                    lex.token += (char) c;
                    state = 3;
                } else {
                    if(isdigit(lex.token[lex.token.length() - 1])){
                        ungetc(c, m_file);
                        lex.type = TKN_FLOAT;
                        state = tkn_defined;
                    } else {
                        ungetc(c,m_file);
                        lex.token += (char) c;
                        lex.type = TKN_INVALID_TOKEN;
                        state = tkn_defined;
                    }
                    
                }
                break;
            }
            case 4:{
                if(c >= 32 && c<= 254 && c != '}'){
                    lex.token += (char) c;
                } else if(c == '\n'){
                    lex.token += (char) c;
                    m_line++;
                    /*if(showPrints){
                        printf("\t\tm_lines %d\n", m_line);
                    }*/
                } else if (c == '}'){
                    lex.token += (char) c;
                    lex.type = TKN_STRING;
                    state = tkn_defined;
                } else if (c == -1){
                    ungetc(c, m_file);
                    lex.type = TKN_UNEXPECTED_EOF;
                    state = tkn_defined;
                }
                break;
            }
            case 5:{
                if(c == '='){
                    lex.token += (char) c;
                    lex.type = TKN_EQ;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.type = TKN_ASSIGN;
                    state = tkn_defined;
                }
                break;
            }
            case 6:{
                if(c == '='){
                    lex.token += (char) c;
                    lex.type = TKN_GE;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.type = TKN_GT;
                    state = tkn_defined;
                }
                break;
            }
            case 7:{
                if(c == '='){
                    lex.token += (char) c;
                    lex.type = TKN_LE;
                    state = tkn_defined;
                }else if (c == '>'){
                    lex.token += (char) c;
                    lex.type = TKN_NE;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.type = TKN_LT;
                    state = tkn_defined;
                }
                break;
            }
            case 8:{
                if(c == '&'){
                    lex.token += (char) c;
                    lex.type = TKN_AND;
                    state = tkn_defined;
                }else if (c == -1){
                    ungetc(c, m_file);
                    lex.type = TKN_UNEXPECTED_EOF;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.token += (char) c;
                    lex.type = TKN_INVALID_TOKEN;
                    state = tkn_defined;
                }
                break;
            }
            case 9:{
                if(c == '|'){
                    lex.token += (char) c;
                    lex.type = TKN_OR;
                    state = tkn_defined;
                }else if (c == -1){
                    ungetc(c, m_file);
                    lex.type = TKN_UNEXPECTED_EOF;
                    state = tkn_defined;
                }else{
                    ungetc(c, m_file);
                    lex.token += (char) c;
                    lex.type = TKN_INVALID_TOKEN;
                    state = tkn_defined;
                }
                break;
            }
            case 10:{
                if(isalnum(c) || c=='_'){
                    lex.token += (char) c;
                    state = 10;
                }
                else{
                    ungetc(c,m_file);
                    state = sb_find;
                }
                break;
            }
            case 11:{
                if(c == '*'){
                    state = 12;
                }
                else if(c == '/'){
                    state = 13;
                }else{
                    ungetc(c, m_file);
                    lex.token = '/';
                    lex.type = TKN_DIV;
                    state = tkn_defined;
                }   
                break;
            }
            case 12:{
                if(c == '*'){
                    state = 14;
                }else if(c == '\n'){
                    m_line++;
                    /*if(showPrints){
                        printf("\t\tm_lines %d\n", m_line);
                    }*/
                }
                if(c == -1){
                    lex.type = TKN_UNEXPECTED_EOF;
                    state = tkn_defined;
                }
                break;
            }
            case 13:{
                if(c=='\n'){
                    state = 1;
                    m_line++;
                    /*if(showPrints){
                        printf("\t\tm_lines %d\n", m_line);
                    }*/
                }
                break;
            }
            case 14:{
                if(c=='/'){
                    state = 1;
                }else{
                    state = 12;
                }
                break;
            }
            default: {
                assert(false);
            }
        }
    }
    if(state==sb_find){
        lex.type = symbolTable->findType(lex.token);
        if(lex.type==TKN_ID){
            symbolTable->addEntry(lex.token, lex.type, TKN_UNASSINGD);
        }
    }
    /*if (showPrints)
        cout << "\t\tm_st[\'" << lex.token << "\']: " << lex.type << " _ " << tt2str(TokenType(lex.type)) << endl;*/
        
    return lex;
}
