#include <cstdio>
#include <cctype>
#include <cassert>
#include <iostream>

#include "SemanticAnalysis.h"

SemanticAnalysis::SemanticAnalysis(SymbolTable* st) : symbolTable(st){
}

SemanticAnalysis::~SemanticAnalysis(){}

bool SemanticAnalysis::equalTypes(Lexeme l1, Lexeme l2){
	if(l1.type == l2.type){
		return true;
	}
	return false;
}

//essa aqui é pra pegar TIPO na TS
int getIdTypeST(std::string var){
  return symboltable.find(var);
}