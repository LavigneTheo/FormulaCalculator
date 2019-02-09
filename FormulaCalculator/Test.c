#include "Test.h"

void launchTests() {
	isOperatorT();
	limitTriangleT();
	testList();
	isStrongOperatorT();
	isNumericT();
	cutStrT();
	getValueT();
	getIndexT();
	strToIntT();
	createNodeCalculatorT();
	intToStrT();
	rewriteStrT();
	exprFinishedT();
	getOperatorIndexT();
	getOperatorT();
	createRightChildT();
	createFirstChildT();
	updateStrT();
	listTotalCharT();
	listStrToStrT();
	treeToRpnT();
	placeChildrenT();
	getNodeIntValueT();
	ProcessNodeValueT();
	processTreeT();
	assertEqualsFloatT();
	getFloatValuesT();
}

Boolean getFloatValuesT() {
	float num1 = 0, num2 = 0;

	char* expr = createMallocStr("2+3");
	NodeCalculator* summit = createTreeFromExpr(expr);
	
	getFloatValues(summit, &num1, &num2);
	assertEqualsFloat(2.f, num1, __FILE__, __LINE__);
	assertEqualsFloat(3.f, num2, __FILE__, __LINE__);
	freeTreeCalculator(summit);

	expr = createMallocStr("2* 3");
	summit = createTreeFromExpr(expr);
	summit->reverse = FALSE;

	getFloatValues(summit, &num1, &num2);
	assertEqualsFloat(3.f, num1, __FILE__, __LINE__);
	assertEqualsFloat(2.f, num2, __FILE__, __LINE__);
	freeTreeCalculator(summit);
}

Boolean processTreeT() {
	char* expr = createMallocStr("2+3");
	NodeCalculator* summit = createTreeFromExpr(expr);

	processTree(summit);
	assertEqualsFloat(5, summit->totalValue, __FILE__, __LINE__);
	freeTreeCalculator(summit);

	expr = createMallocStr("2+3*(5+2)");
	summit = createTreeFromExpr(expr);

	processTree(summit);
	assertEqualsFloat(23, summit->totalValue, __FILE__, __LINE__);
	freeTreeCalculator(summit);

	expr = createMallocStr("  ((1+2)- (1+2)/3) * 2");
	summit = createTreeFromExpr(expr);

	processTree(summit);
	assertEqualsFloat(4, summit->totalValue, __FILE__, __LINE__);
	freeTreeCalculator(summit);

	expr = createMallocStr("  (( 2 +(8-1) * 10 - 2)- ((7-0) * 2 -2)/12) * 2");
	summit = createTreeFromExpr(expr);
	printf("%s\n", treeToRpn(summit));

	processTree(summit);
	assertEqualsFloat(138, summit->totalValue, __FILE__, __LINE__);
	freeTreeCalculator(summit);
}


Boolean getNodeIntValueT() {
	NodeCalculator* node = malloc(sizeof(NodeCalculator));
	node->value = "100";
	node->operator = FALSE;
	assertEqualsInt(100, getNodeFloatValue(node), __FILE__, __LINE__);
	free(node);

	node = malloc(sizeof(NodeCalculator));
	node->operator = TRUE;
	node->totalValue = 298;
	assertEqualsInt(298, getNodeFloatValue(node), __FILE__, __LINE__);
	free(node);
}

Boolean ProcessNodeValueT() {
	char* expr = createMallocStr("2+3");
	NodeCalculator* summit = createTreeFromExpr(expr);

	ProcessNodeValue(summit);
	assertEqualsFloat(5.f, summit->totalValue, __FILE__, __LINE__);
	freeTreeCalculator(summit);


	expr = createMallocStr("2*13");
	summit = createTreeFromExpr(expr);
	
	processTree(summit);
	assertEqualsFloat(26.f, summit->totalValue, __FILE__, __LINE__);
	freeTreeCalculator(summit);

	expr = createMallocStr("10/2");
	summit = createTreeFromExpr(expr);

	processTree(summit);
	assertEqualsFloat(5.f, summit->totalValue, __FILE__, __LINE__);
	freeTreeCalculator(summit);

}

Boolean assertEqualsFloatT() {
	assertEqualsFloat(10.56788f, 10.56788888f, __FILE__, __LINE__);
	assertEqualsFloat(9.9, 9.9, __FILE__, __LINE__);
	assertEqualsFloat(10.99999, 10.999991, __FILE__, __LINE__);
}

Boolean placeChildrenT() {
	NodeCalculator* summit = malloc(sizeof(NodeCalculator));
	NodeCalculator* el1 = malloc(sizeof(NodeCalculator));
	NodeCalculator* el2 = malloc(sizeof(NodeCalculator));

	el1->value = "+";
	el2->value = "788";
	placeChildren(summit, el1, el2);

	assertEqualsStr("+", summit->left->value, __FILE__, __LINE__);
	assertEqualsStr("788", summit->right->value, __FILE__, __LINE__);
	free(summit);
	free(el1);
	free(el2);

	summit = malloc(sizeof(NodeCalculator));
	el1 = malloc(sizeof(NodeCalculator));
	el2 = malloc(sizeof(NodeCalculator));

	el1->value = "78";
	el2->value = "/";
	placeChildren(summit, el1, el2);

	assertEqualsStr("/", summit->left->value, __FILE__, __LINE__);
	assertEqualsStr("78", summit->right->value, __FILE__, __LINE__);
	free(summit);
	free(el1);
	free(el2);

}

Boolean getOperatorIndexT() {
	assertEqualsInt(1, getOperatorIndex("2*3"), __FILE__, __LINE__);
	assertEqualsInt(6, getOperatorIndex("  2987*093"), __FILE__, __LINE__);
	assertEqualsInt(6, getOperatorIndex(" 9876 / 3"), __FILE__, __LINE__);
}

Boolean treeToRpnT() {
	char* expr = createMallocStr(" 8 + 7 * ( 89 + 2 ) ");
	NodeCalculator* summit =  createTreeFromExpr(expr);
	char* treePostFix = treeToRpn(summit);
	assertEqualsStr("289+7*8+", treePostFix, __FILE__, __LINE__);
	free(treePostFix);
	freeTreeCalculator(summit);


	expr = createMallocStr("(89 +0) * 9 +1");
	summit = createTreeFromExpr(expr);
	treePostFix = treeToRpn(summit);
	assertEqualsStr("089+9*1+", treePostFix, __FILE__, __LINE__);
	free(treePostFix);
	freeTreeCalculator(summit);

	expr = createMallocStr(" 8 + (78*(8* (98 /2) + 2) * 2) + 2 ");
	summit = createTreeFromExpr(expr);
	treePostFix = treeToRpn(summit);
	assertEqualsStr("298/8*2+78*2*2+8+", treePostFix, __FILE__, __LINE__);
	free(treePostFix);
	freeTreeCalculator(summit);

	expr = createMallocStr("(8+2*(2+3))*2*(2+3)*(2+3)");
	summit = createTreeFromExpr(expr);
	treePostFix = treeToRpn(summit);
	assertEqualsStr("32+2*8+2*32+*32+*", treePostFix, __FILE__, __LINE__);
	free(treePostFix);
	freeTreeCalculator(summit);
}

char* createMallocStr(char* str) {
	char* dynaStr = malloc(sizeof(char) * (strlen(str) + 1));
	*(dynaStr + strlen(str)) = '\0';
	for (int i = 0; i < strlen(dynaStr); i++)
		*(dynaStr + i) = *(str + i);
	return dynaStr;
}

Boolean listStrToStrT() {
	List* list = get_list(NULL, sizeof(sizeof(char*)));

	char* str1 = createMallocStr("lol");
	append(list, &str1);
	str1 = createMallocStr(" 78 ");
	append(list, &str1);
	str1 = createMallocStr(" Merde");
	append(list, &str1);

	char* res = listStrToStr(list);
	assertEqualsStr("lol 78  Merde", res, __FILE__, __LINE__);
	free(res);

	free_list(list);
}

Boolean listTotalCharT() {
	List* list = get_list(NULL, sizeof(sizeof(char*)));
	
	char* str1 = createMallocStr("lol");
	append(list, &str1);
	str1 = createMallocStr(" 78 ");
	append(list, &str1);
	str1 = createMallocStr(" Merde");
	append(list, &str1);

	assertEqualsInt(13, listTotalChar(list), __FILE__, __LINE__);

	free_list(list);
}


Boolean updateStrT() {
	char* res = updateStr("778 +(90 * 876 + 9)  ", 5, 14, 19);
	assertEqualsStr("778 +(a19+ 9)  ", res, __FILE__, __LINE__);

	res = updateStr("778 +(876 + 9)  ", 5, 13, 19);
	assertEqualsStr("778 +a19  ", res, __FILE__, __LINE__);

	res = updateStr("(9+1)", 0, 4, 19);
	assertEqualsStr("a19", res, __FILE__, __LINE__);
}

Boolean createTriangleT() {
	List* list = get_list(NULL, sizeof(NodeCalculator*));

	NodeCalculator* summit = createTriangle("876 *90", list);
	assertEqualsStr("*", summit->value, __FILE__, __LINE__);
	assertEqualsStr("876", summit->left->value, __FILE__, __LINE__);
	assertEqualsStr("90", summit->right->value, __FILE__, __LINE__);
	assertEqualsStr("*", (*(NodeCalculator**)get_at(list, 0))->value, __FILE__, __LINE__);


	summit = createTriangle(" 450 /   54", list);
	assertEqualsStr("/", summit->value, __FILE__, __LINE__);
	assertEqualsStr("450", summit->left->value, __FILE__, __LINE__);
	assertEqualsStr("54", summit->right->value, __FILE__, __LINE__);
	assertEqualsStr("/", (*(NodeCalculator**)get_at(list, 1))->value, __FILE__, __LINE__);

	summit = createTriangle(" a0 +   a1", list);
	assertEqualsStr("/", summit->value, __FILE__, __LINE__);
	assertEqualsStr("*", summit->left->value, __FILE__, __LINE__);
	assertEqualsStr("/", summit->right->value, __FILE__, __LINE__);
	assertEqualsStr("+", (*(NodeCalculator**)get_at(list, 2))->value, __FILE__, __LINE__);

	freeTreeCalculator(summit);

	free_list(list);
}

Boolean createRightChildT() {
	List* list = get_list(NULL, sizeof(NodeCalculator*));

	NodeCalculator* node = createSecondElem("9876+78", list);
	assertEqualsStr("78", node->value, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	free(node);

	node = createSecondElem("8765456/ 78", list);
	assertEqualsStr("78", node->value, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	append(list, &node);

	node = createSecondElem("8765 / a0", list);
	assertEqualsStr("78", node->value, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	free(node);

	free_list(list);
}

Boolean createFirstChildT() {
	List* list = get_list(NULL, sizeof(NodeCalculator*));

	NodeCalculator* node = createFirstElem("9876+78", list);
	assertEqualsStr("9876", node->value, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	free(node);

	node = createFirstElem("8765456/78", list);
	assertEqualsStr("8765456", node->value, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	append(list, &node);

	node = createFirstElem("a0/78", list);
	assertEqualsStr("8765456", node->value, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	append(list, node);

	node = createFirstElem("a0 * 9", list);
	assertEqualsStr("8765456", node->value, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	free(node);

	free_list(list);
}

Boolean exprFinishedT() {
	assertEqualsBool(TRUE, exprFinished("a19"), __FILE__, __LINE__);
	assertEqualsBool(TRUE, exprFinished("a19876"), __FILE__, __LINE__);
	assertEqualsBool(TRUE, exprFinished("a58097"), __FILE__, __LINE__);

	assertEqualsBool(FALSE, exprFinished("a19 + 09"), __FILE__, __LINE__);
	assertEqualsBool(FALSE, exprFinished("a89 + a19"), __FILE__, __LINE__);
	assertEqualsBool(FALSE, exprFinished("8 -a19"), __FILE__, __LINE__);
}

Boolean rewriteStrT() {

	char* res = rewriteStr("3 + 3", 0, 4, 0);
	assertEqualsStr("a0", res, __FILE__, __LINE__);
	free(res);

	res = rewriteStr("3+3+3", 0, 2, 136);
	assertEqualsStr("a136+3", res, __FILE__, __LINE__);
	free(res);

	res = rewriteStr("a387 *8766+(45+0)", 0, 9, 136);
	assertEqualsStr("a136+(45+0)", res, __FILE__, __LINE__);
	free(res);

	res = rewriteStr("3 + 3+7+2+ 5*4 +2", 10, 14, 9987);
	assertEqualsStr("3 + 3+7+2+a9987+2", res, __FILE__, __LINE__);
	free(res);

	res = rewriteStr("3 + 3 -654-(78+(78 * 76))", 15, 23, 9);
	assertEqualsStr("3 + 3 -654-(78+a9)", res, __FILE__, __LINE__);
	free(res);
}

Boolean intToStrT() {
	char* res = intToStr(20);
	assertEqualsStr("20", res, __FILE__, __LINE__);

	res = intToStr(1);
	assertEqualsStr("1", res, __FILE__, __LINE__);

	res = intToStr(0);
	assertEqualsStr("0", res, __FILE__, __LINE__);

	res = intToStr(8697828);
	assertEqualsStr("8697828", res, __FILE__, __LINE__);

	res = intToStr(9862892);
	assertEqualsStr("9862892", res, __FILE__, __LINE__);

	free(res);
}

Boolean createNodeCalculatorT() {
	List* listPtr = get_list(NULL, sizeof(NodeCalculator*));

	NodeCalculator* node = createNodeCalculator("2", listPtr);
	assertEqualsStr("2", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	free(node);

	node = createNodeCalculator("*", listPtr);
	assertEqualsStr("*", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(TRUE, node->operator, __FILE__, __LINE__);
	free(node);

	node = createNodeCalculator("+", listPtr);
	assertEqualsStr("+", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(TRUE, node->operator, __FILE__, __LINE__);
	free(node);

	node = createNodeCalculator("-", listPtr);
	assertEqualsStr("-", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(TRUE, node->operator, __FILE__, __LINE__);
	free(node);

	node = createNodeCalculator("/", listPtr);
	assertEqualsStr("/", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(TRUE, node->operator, __FILE__, __LINE__);
	free(node);

	node = createNodeCalculator("40", listPtr);
	assertEqualsStr("40", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	free(node);

	node = createNodeCalculator("98765", listPtr);
	assertEqualsStr("98765", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(FALSE, node->operator, __FILE__, __LINE__);
	free(node);

	free_list(listPtr);
	listPtr = get_list(NULL, sizeof(NodeCalculator*));

	NodeCalculator* testNode = malloc(sizeof(NodeCalculator));
	testNode->operator = TRUE;
	testNode->value = "+";
	testNode->left = NULL;
	testNode->right = NULL;
	append(listPtr, &testNode);

	node = createNodeCalculator("a0", listPtr);
	assertEqualsStr("+", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(TRUE, node->operator, __FILE__, __LINE__);
	free(node);

	testNode = malloc(sizeof(NodeCalculator));
	testNode->operator = TRUE;
	testNode->value = "*";
	testNode->left = NULL;
	testNode->right = NULL;
	append(listPtr, &testNode);

	node = createNodeCalculator("a1", listPtr);
	assertEqualsStr("*", node->value, __FILE__, __LINE__);
	assertNull(node->right, __FILE__, __LINE__);
	assertNull(node->left, __FILE__, __LINE__);
	assertEqualsBool(TRUE, node->operator, __FILE__, __LINE__);
	free(node);
	
	free_list(listPtr);
	return TRUE;
}

Boolean getIndexT() {
	assertEqualsInt(10, getIndex("a10"), __FILE__, __LINE__);
	assertEqualsInt(1, getIndex("a1"), __FILE__, __LINE__);
	assertEqualsInt(0, getIndex("a0"), __FILE__, __LINE__);
	assertEqualsInt(987656780, getIndex("a987656780"), __FILE__, __LINE__);
}

Boolean getOperatorT() {
	char* ope = getOperator("3*3");
	assertEqualsStr("*", ope, __FILE__, __LINE__);
	free(ope);

	ope = getOperator("3+3");
	assertEqualsStr("+", ope, __FILE__, __LINE__);
	free(ope);

	ope = getOperator("3/3");
	assertEqualsStr("/", ope, __FILE__, __LINE__);
	free(ope);

	ope = getOperator("3-3");
	assertEqualsStr("-", ope, __FILE__, __LINE__);
	free(ope);
}

Boolean strToIntT() {
	assertEqualsInt(10, strToInt("10"), __FILE__, __LINE__);
	assertEqualsInt(567, strToInt("567"), __FILE__, __LINE__);
	assertEqualsInt(999999, strToInt("999999"), __FILE__, __LINE__);
	assertEqualsInt(9876, strToInt("9876"), __FILE__, __LINE__);
}

Boolean getValueT() {
	int start = 0, end = 0;
	char** res = malloc(sizeof(char*));

	getValue("a12 + 23", start, end, res);
	assertEqualsStr("a12", *res, __FILE__, __LINE__);
	free(*res);

	getValue("7*0", start, end, res);
	assertEqualsStr("7", *res, __FILE__, __LINE__);
	free(*res);

	start = 0;
	getValue("+ 23", start, end, res);
	assertEqualsStr("23", *res, __FILE__, __LINE__);
	free(*res);

	start = 0;
	getValue("(276 + 23)", start, end, res);
	assertEqualsStr("276", *res, __FILE__, __LINE__);
	free(*res);

	start = 0;
	getValue("345)", start, end, res);
	assertEqualsStr("345", *res, __FILE__, __LINE__);
	free(*res);

	start = 0;
	getValue("345", start, end, res);
	assertEqualsStr("345", *res, __FILE__, __LINE__);
	free(*res);

	start = 0;
	getValue(" 67 ", start, end, res);
	assertEqualsStr("67", *res, __FILE__, __LINE__);
	free(*res);

	free(res);
	return TRUE;
}

Boolean cutStrT() {
	char* res = cutStr("a12 + 7", 0, 2);
	assertEqualsStr("a12", res, __FILE__, __LINE__);
	free(res);

	res = cutStr("a12 + 7", 6, 6);
	assertEqualsStr("7", res, __FILE__, __LINE__);
	free(res);

	res = cutStr("a12 + 7876  ", 6, 9);
	assertEqualsStr("7876", res, __FILE__, __LINE__);
	free(res);

	res = cutStr("(45 + a123)", 6, 9);
	assertEqualsStr("a123", res, __FILE__, __LINE__);
	free(res);

	return TRUE;
}

Boolean isNumericT() {
	assertEqualsBool(TRUE, isNumeric('0'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('1'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('2'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('3'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('4'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('5'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('6'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('7'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('8'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isNumeric('9'), __FILE__, __LINE__);

	assertEqualsBool(FALSE, isNumeric('/'), __FILE__, __LINE__);
	assertEqualsBool(FALSE, isNumeric(':'), __FILE__, __LINE__);

	return TRUE;
}

Boolean isOperatorT() {
	assertEqualsBool(TRUE, isOperator('+'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isOperator('*'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isOperator('/'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isOperator('-'), __FILE__, __LINE__);

	assertEqualsBool(FALSE, isOperator('^'), __FILE__, __LINE__);
	assertEqualsBool(FALSE, isOperator('a'), __FILE__, __LINE__);
	assertEqualsBool(FALSE, isOperator('9'), __FILE__, __LINE__);
	assertEqualsBool(FALSE, isOperator('%'), __FILE__, __LINE__);
	assertEqualsBool(FALSE, isOperator('.'), __FILE__, __LINE__);

	return TRUE;
}
Boolean isStrongOperatorT() {
	assertEqualsBool(TRUE, isStrongOperator('*'), __FILE__, __LINE__);
	assertEqualsBool(TRUE, isStrongOperator('/'), __FILE__, __LINE__);

	assertEqualsBool(FALSE, isStrongOperator('+'), __FILE__, __LINE__);
	assertEqualsBool(FALSE, isStrongOperator('-'), __FILE__, __LINE__);
	assertEqualsBool(FALSE, isStrongOperator('%'), __FILE__, __LINE__);

	return TRUE;
}


Boolean limitTriangleT() {
	int start = 0, end;

	char expr1[] = "3 + 3";
	limitTriangle(expr1, &start, &end);
	assertEqualsInt(0, start, __FILE__, __LINE__);
	assertEqualsInt(4, end, __FILE__, __LINE__);

	start = 0;
	char expr2[] = "3 + (3 * 1)";
	limitTriangle(expr2, &start, &end);
	assertEqualsInt(4, start, __FILE__, __LINE__);
	assertEqualsInt(10, end, __FILE__, __LINE__);

	start = 0;
	char expr3[] = "3 +3+2";
	limitTriangle(expr3, &start, &end);
	assertEqualsInt(3, start, __FILE__, __LINE__);
	assertEqualsInt(5, end, __FILE__, __LINE__);
	assertNotEqualsInt(1, start, __FILE__, __LINE__);
	assertNotEqualsInt(4, end, __FILE__, __LINE__);

	start = 0;
	char expr4[] = "(3+3 ) * 2";
	limitTriangle(expr4, &start, &end);
	assertEqualsInt(0, start, __FILE__, __LINE__);
	assertEqualsInt(5, end, __FILE__, __LINE__);

	start = 0;
	char expr5[] = "a56+ 7";
	limitTriangle(expr5, &start, &end);
	assertEqualsInt(0, start, __FILE__, __LINE__);
	assertEqualsInt(5, end, __FILE__, __LINE__);

	start = 0;
	char expr6[] = "a56+ (7* 8) ";
	limitTriangle(expr6, &start, &end);
	assertEqualsInt(5, start, __FILE__, __LINE__);
	assertEqualsInt(10, end, __FILE__, __LINE__);

	start = 0;
	char expr7[] = "7 +(8* a76 ) ";
	limitTriangle(expr7, &start, &end);
	assertEqualsInt(3, start, __FILE__, __LINE__);
	assertEqualsInt(11, end, __FILE__, __LINE__);

	start = 0;
	char expr8[] = "7 +7 * 2 * 7 ";
	limitTriangle(expr8, &start, &end);
	assertEqualsInt(3, start, __FILE__, __LINE__);
	assertEqualsInt(8, end, __FILE__, __LINE__);

	start = 0;
	char expr9[] = "7 +7*(2 + 3) ";
	limitTriangle(expr9, &start, &end);
	assertEqualsInt(5, start, __FILE__, __LINE__);
	assertEqualsInt(11, end, __FILE__, __LINE__);

	start = 0;
	char expr10[] = "7 +a2*(2 + a1) ";
	limitTriangle(expr10, &start, &end);
	assertEqualsInt(6, start, __FILE__, __LINE__);
	assertEqualsInt(13, end, __FILE__, __LINE__);

	start = 0;
	char expr11[] = "3 + 3+7+2+ 5*4 +2";
	limitTriangle(expr11, &start, &end);
	assertEqualsInt(10, start, __FILE__, __LINE__);
	assertEqualsInt(14, end, __FILE__, __LINE__);

	start = 0;
	char expr12[] = "8+a5+2";
	limitTriangle(expr12, &start, &end);
	assertEqualsInt(2, start, __FILE__, __LINE__);
	assertEqualsInt(5, end, __FILE__, __LINE__);

	return TRUE;
}

Boolean testList() {
	List* list = get_list(NULL, sizeof(int));
	int data = 0;
	int index = append(list, &data);
	assertEqualsInt(0, index, __FILE__, __LINE__);
	data = 1;
	index = append(list, &data);
	assertEqualsInt(1, index, __FILE__, __LINE__);
	data = 2;
	index = append(list, &data);
	assertEqualsInt(2, index, __FILE__, __LINE__);
	data = 3;
	index = append(list, &data);
	assertEqualsInt(3, index, __FILE__, __LINE__);

	assertEqualsInt(0, *(int*)get_at(list ,0), __FILE__, __LINE__);
	assertEqualsInt(1, *(int*)get_at(list ,1), __FILE__, __LINE__);
	assertEqualsInt(2, *(int*)get_at(list ,2), __FILE__, __LINE__);
	assertEqualsInt(3, *(int*)get_at(list ,3), __FILE__, __LINE__);

	free_list(list);
	return TRUE;
}


