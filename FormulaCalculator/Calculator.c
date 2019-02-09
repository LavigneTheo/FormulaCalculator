#include "Calculator.h"

/* Détermine s'il le caractère est un opérateur
 *
 */
Boolean isOperator(char c) {
	char operators[] = "+-*/";
	Boolean isOpe = FALSE;
	for (int i = 0; i < 4; i++)
		if (operators[i] == c)
			isOpe = TRUE;
	return isOpe;
}

int getOperatorIndex(char* expr)
{
	int i = 0;
	while (isOperator(*(expr + i)) == FALSE)
		i++;
	return i;
}

char* getOperator(char* expr) {
	int i = getOperatorIndex(expr);
	char* ope = malloc(sizeof(char) * 2);
	*(ope + 1) = '\0';
	*ope = *(expr + i);
	return ope;
}

NodeCalculator* createFirstElem(char* subExpr, List* listOperatorPtr) {
	char** value = malloc(sizeof(char*));
	getValue(subExpr, 0, strlen(subExpr) - 1, value);
	NodeCalculator* left = createNodeCalculator(*value, listOperatorPtr);
	free(value);
	return left;
}

NodeCalculator* createSecondElem(char* subExpr, List* listOperatorPtr) {
	char** value = malloc(sizeof(char*));
	int start = getOperatorIndex(subExpr);
	getValue(subExpr, start, strlen(subExpr) - 1, value);
	NodeCalculator* right = createNodeCalculator(*value, listOperatorPtr);
	free(value);
	return right;
}

/* Place les enfant de l'opérateur du bon coté.
 * Pour cela il faut s'assuré que si il y a un flag 'a', il se place à gauche et on a droite.
 *
 */
void placeChildren(NodeCalculator* summit, NodeCalculator* first, NodeCalculator* second) {
	if (isOperator(*first->value)) {
		summit->left = first;
		summit->right = second;
		summit->reverse = FALSE;
	}
	else {
		summit->left = second;
		summit->right = first;
		summit->reverse = TRUE;
	}
}

/* Creer un triangle composé d'un opéateur comme sommet. Les enfants peuvent être
 * un nombre ou au autre opérateur
 *
 */
NodeCalculator* createTriangle(char* subExpr, List* listOperatorPtr) {
	NodeCalculator* firstElem = createFirstElem(subExpr, listOperatorPtr);

	char* value = getOperator(subExpr);
	NodeCalculator* summit = createNodeCalculator(value, listOperatorPtr);

	NodeCalculator* secondElem = createSecondElem(subExpr, listOperatorPtr);

	placeChildren(summit, firstElem, secondElem);
	
	return summit;
}

void freeTreeCalculator(NodeCalculator* node) {
	if (node->left != NULL)
		freeTreeCalculator(node->right);
	if (node->right != NULL)
		freeTreeCalculator(node->left);
	free(node->value);
	free(node);
}




NodeCalculator* createTreeFromExpr(char* expr) {
	List* listOperatorPtr = get_list(NULL, sizeof(NodeCalculator*));
	NodeCalculator* summit = NULL;
	while (exprFinished(expr) == FALSE)
	{
		int start = 0, end;
		limitTriangle(expr, &start, &end);

		char* subExpr = cutStr(expr, start, end);
		summit = createTriangle(subExpr, listOperatorPtr);
		free(subExpr);
		
		char* tmp = updateStr(expr, start, end, listOperatorPtr->length - 1);
		free(expr);
		expr = tmp;
	}
	free(listOperatorPtr);
	return summit;
}

void addToRPNList(NodeCalculator* node, List* listNode) {
	if (node->left != NULL)
		addToRPNList(node->left, listNode);
	if (node->right != NULL)
		addToRPNList(node->right, listNode);
	append(listNode, &node->value);
}

void printPostFix(NodeCalculator* node) {
	if (node->right != NULL)
		printPostFix(node->right);
	if (node->left != NULL)
		printPostFix(node->left);
	printf("%s\n", node->value);
}

char* treeToRpn(NodeCalculator* summit) {
	List* listNode = get_list(NULL, sizeof(char**));
	addToRPNList(summit, listNode);
	char* strRPN = listStrToStr(listNode);
	free_list(listNode);
	return strRPN;
}

/* Determine si l'expression à été entiermeent traitée
 *
 */
Boolean exprFinished(char* expr) {
	if (*expr != 'a')
		return FALSE;
	int index = 0;
	while (index < strlen(expr))
		if(isOperator(*(expr + index++)))
			return FALSE;
	return TRUE;
}

/* Détermine s'il le caractère est un opérateur prioritaire
 *
 */
Boolean isStrongOperator(char c) {
	char operators[] = "*/";
	Boolean isOpe = FALSE;
	for (int i = 0; i < 2; i++)
		if (operators[i] == c)
			isOpe = TRUE;
	return isOpe;
}

/* Détzrmine si un caractère est numérique (0-9)
 *
 *
 */
Boolean isNumeric(char c) {
	if ((int)c >= 48 && (int)c <= 57)
		return TRUE;
	else
		return FALSE;
}

/* Retourne un entier à partir d'un flag ptr
 *
 *
 */
int getIndex(char* expr) {
	char* value = cutStr(expr, 1, strlen(expr) - 1);
	int res = strToInt(value);
	free(value);
	return res;
}

/* Convertit une chaîn en entier de type INT
 *
 */
int strToInt(char* expr) {
	char* end;
	return strtol(expr, &end, 10);
}

/* Créé un noued prèt à être inséré dans l'arbre binaire.
 * Prend en ompte le fait le caractère peut être un opérateur
 *
 */
NodeCalculator* createNodeCalculator(char* expr, List* listPtr) {
	if (*expr == 'a') {
		int index = getIndex(expr);
		return  *(NodeCalculator**)get_at(listPtr, index);
	}
	else {
		NodeCalculator* node = malloc(sizeof(NodeCalculator));
		node->value = expr;
		node->left = NULL;
		node->right = NULL;
		if (isOperator(*expr)) {
			node->operator = TRUE;
			append(listPtr, &node);
		}
		else
			node->operator = FALSE;
		return node;
	}
}

/* Renvoie une chaîne équivalent au contenue de l'interval passé en apramètre
 *
 *
 *
 */
char* cutStr(char* expr, int start, int end) {
	char* res = malloc(sizeof(char) * (end - start) + 2);
	*(res + end - start + 1) = '\0';
	for (int i = 0; i <= (end - start); i++)
		*(res + i) = *(expr + i + start);
	return res;
}

/* Isole un nombre dans une chaine de charactère, commence par élémininer les
 * les charactères qui ne sont pas numérique pour ne garder que les chiffre.
 *
 */
void getValue(char* expr, int start, int end, char** value) {
	while (isNumeric(*(expr + start)) == FALSE && *(expr + start) != 'a')
		start++;
	end = start;
	while (isNumeric(*(expr + end + 1)) == TRUE)
		end++;
	*value = cutStr(expr, start, end);
}

char* intToStr(int value) {
	char* res = malloc(sizeof(char) * 13);
	sprintf(res, "%d", value);
	return res;
}

/* Détermine l'index de début fin de réécriture de la chaine.
 * Fait également atentition a ne supprimer qu'une parenthèse ouvrante dans le cas
 * de parenthèses multiples ex : '((2+2)...'
 */
char* updateStr(char* expr, int start, int end, int index) {
	if (*(expr + start) == '(' && *(expr + end) != ')')
		start++;
	else if (*(expr + start) != '(' && *(expr + end) == ')')
		end--;
	else
		while (*(expr + start + 1) == '(')
			start++;
	return rewriteStr(expr, start, end, index);
}

char* rewriteStr(char* expr, int start, int end, int index) {
	char* tmp = intToStr(index);
	int newSize = strlen(tmp) + 1 + strlen(expr) - end + start;
	char* res = malloc(sizeof(char) * newSize);
	*(res + newSize - 1) = '\0';
	for (int i = 0; i < start; i++)
		*(res + i) = *(expr + i);
	*(res + start) = 'a';
	for (int i = start + 1; i < start + 1 + strlen(tmp); i++)
		*(res + i) = *(tmp + i - start - 1);
	for (int i = end + 1; i < strlen(expr); i++) {
		int indexRes = i - end + start + strlen(tmp);
		*(res + indexRes) = *(expr + i);
	}
	return res;
}

void processTree(NodeCalculator* node) {
	if (node->left != NULL)
		processTree(node->left);
	if (node->right != NULL && isOperator(*node->right->value))
		processTree(node->right);
	if (isOperator(*node->value))
		ProcessNodeValue(node);
}

float getNodeFloatValue(NodeCalculator* node) {
	if (node->operator == TRUE)
		return node->totalValue;
	return  (float)strToInt(node->value);
}

void getFloatValues(NodeCalculator* node, float* num1, float* num2) {
	if (!node->reverse) {
		*num1 = getNodeFloatValue(node->left);
		*num2 = getNodeFloatValue(node->right);
	}
	else {
		*num2 = getNodeFloatValue(node->left);
		*num1 = getNodeFloatValue(node->right);
	}
}

void ProcessNodeValue(NodeCalculator* node) {
	float a = 0, b = 0;
	getFloatValues(node, &a, &b);

	float value = 0;
	if (*node->value == '*')
		value = a * b;
	if (*node->value == '/')
		value = a / b;
	if (*node->value == '+')
		value = a + b;
	if (*node->value == '-')
		value = a - b;
	node->totalValue = value;
}

/* Retourne les borne la prochaine sous exression à inséré dans l'arbre. Le retour se fait par
 * le biais des paramètres start et end. Start doit être initialisé au bon index avant l'appel
 * de la fontion.
 *
 *
 */
void limitTriangle(char* expr, int* start, int* end) {
	*end = *start;
	Boolean found = FALSE, foundOperator = FALSE, strongOperator;
	int indexLastOperator;
	while (!found && *end < strlen(expr))
	{
		if (!foundOperator) {
			if (isOperator(*(expr + *end))) {
				foundOperator = TRUE;
				strongOperator = isStrongOperator(*(expr + *end));
				indexLastOperator = *end;
			}
		}
		else {
			if (*(expr + *end) == '(') {
				*start = *end;
				foundOperator = FALSE;
			}
			else if (*(expr + *end) == ')') {
				found = TRUE;
			}
			else  if (isOperator(*(expr + *end))) {
				if (strongOperator) {
					(*end)--;
				}
				else {
					*start = *end = indexLastOperator + 1;
					limitTriangle(expr, start, end);
				}
				found = TRUE;
			}
			else if (*end + 1 >= strlen(expr)) {
				found = TRUE;
			}
		}
		if (!found)
			(*end)++;
	}
}