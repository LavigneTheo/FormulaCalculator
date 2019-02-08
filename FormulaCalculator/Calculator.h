#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Include.h"

typedef struct NodeCalculator {
	struct NodeCalculator* right;
	struct NodeCalculator* left;

	Boolean operator;
	char* value;
	
	Boolean processed;
	int totalValue;
}NodeCalculator;


Boolean isOperator(char c);
Boolean isStrongOperator(char c);
Boolean isNumeric(char c);
char* cutStr(char* expr, int start, int end);
void getValue(char* expr, int start, int end, char** value);
int getIndex(char* expr);
int strToInt(char* expr);
char* intToStr(int value);
Boolean exprFinished(char* expr);
int getOperatorIndex(char* expr);
char* getOperator(char* expr);
char* updateStr(char* expr, int start, int end, int index);
char* treeToRpn(NodeCalculator* summit);
void printPostFix(NodeCalculator* node);

NodeCalculator* createSecondElem(char* subExpr, List* listOperatorPtr);
NodeCalculator* createFirstElem(char* subExpr, List* listOperatorPtr);
void placeChildren(NodeCalculator* summit, NodeCalculator* first, NodeCalculator* second);
NodeCalculator* createTriangle(char* subExpr, List* listOperatorPtr);


void limitTriangle(char* expr, int* start, int* end);
NodeCalculator* createNodeCalculator(char* expr, List* listPtr);
char* rewriteStr(char* expr, int start, int end, int index);
NodeCalculator* createTreeFromExpr(char* expr);



