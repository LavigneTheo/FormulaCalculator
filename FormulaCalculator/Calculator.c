#include "Calculator.h"

/* D�termine si le caract�re pass� en param�tre est un op�rateur
 * Params - char : le caract�re � tester
 * Return - Boolean : TRUE si c'est un op�rateur sinon false
 */
Boolean isOperator(char c) {
	char operators[] = "+-*/";
	Boolean isOpe = FALSE;
	for (int i = 0; i < 4; i++)
		if (operators[i] == c)
			isOpe = TRUE;
	return isOpe;
}

/* D�termine l'index auquel se trouve l'operateur dans une cha�ne
 * Params - char* : la cha�ne sur laquelle faire la recherche
 * Return - Integer : index ou se trouve l'op�rateur dans la cha�ne
 */
int getOperatorIndex(char* expr)
{
	int i = 0;
	while (isOperator(*(expr + i)) == FALSE)
		i++;
	return i;
}

/* D�termine l'op�rateur dans une cha�ne, renvoi un pointeur vers un caract�re 
 * Params - char* : la cha�ne o� chercher l'op�rateur
 * Return - char* : pointeur vers le caract�re repr�sentant l'op�rateur
 */
char* getOperator(char* expr) {
	int i = getOperatorIndex(expr);
	char* ope = malloc(sizeof(char) * 2);
	*(ope + 1) = '\0';
	*ope = *(expr + i);
	return ope;
}

/* Creer le noeud a partir du premier terme d'une op�ration math�matique
 * ex : '3 + 1', 3 est le premier �l�ment 
 * L'expression contient toujours un seul et unique op�rateur
 * Params - char* : chaine contenant l'op�ration
 * Return - NodeCalculator* : structure � stocker dans l'arbre binaire
 */
NodeCalculator* createFirstElem(char* subExpr, List* listOperatorPtr) {
	char** value = malloc(sizeof(char*));
	getValue(subExpr, 0, strlen(subExpr) - 1, value);
	NodeCalculator* left = createNodeCalculator(*value, listOperatorPtr);
	free(value);
	return left;
}

/* Creer le noeud a partir du second terme d'une op�ration math�matique
 * ex : '3 + 1', 1 est le second �l�ment
 * L'expression contient toujours un seul et unique op�rateur
 * Params - char* : chaine contenant l'op�ration
 * Return - NodeCalculator* : structure � stocker dans l'arbre binaire
 */
NodeCalculator* createSecondElem(char* subExpr, List* listOperatorPtr) {
	char** value = malloc(sizeof(char*));
	int start = getOperatorIndex(subExpr);
	getValue(subExpr, start, strlen(subExpr) - 1, value);
	NodeCalculator* right = createNodeCalculator(*value, listOperatorPtr);
	free(value);
	return right;
}



/* Place les enfants d'un op�rateur.
 * Si l'�l�ment de gauche n'est pas un  op�rateur, on met le second terme � gauche. Cela enmp�che
 * de se retrouver dans une situation o� l'on aurait � un nombre, et � droite un op�rateur avec lui'm�me
 * d'autre enfants qui d�s�quilibrerai l'arbre.
 * Lorsque l'inverse les enfant, on met le champ 'reverse' de la structure de l'op�rateur � 'TRUE', sinon
 * les op�rations non commutatives (-, / etc) ne pourrait �tre calculer correctement � cause de l'inversion.
 * Params - NodeCalculator* summit : l'op�rateur de l'op�ration, potentiel sommet de l'arbre
 *			NodeCalculator* first : le premier terme de l'op�ration
 *			NodeCalculator* second : le second terme de l'op�ration
 * Return - void
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


/* Creer un triangle compos� d'un op�rateur comme sommet. Les enfants peuvent �tre
 * un nombre ou au autre op�rateur.
 * L'op�ration pass�e en param�tre contient un seul et unique op�rateur.
 * Params - char* subExpr : op�ration a effectuer
 *			List* listOperatorPtr : liste permettant de stocker les op�rateurs pr�sent dans l'arbre
 * Return - NodeCalculator* : structure contenant l'op�rateur, potentiel sommet de l'arbre
 */
NodeCalculator* createTriangle(char* subExpr, List* listOperatorPtr) {
	NodeCalculator* firstElem = createFirstElem(subExpr, listOperatorPtr);

	char* value = getOperator(subExpr);
	NodeCalculator* summit = createNodeCalculator(value, listOperatorPtr);

	NodeCalculator* secondElem = createSecondElem(subExpr, listOperatorPtr);

	placeChildren(summit, firstElem, secondElem);
	
	return summit;
}

/* Supprime r�cursivement les structures pr�sente dans l'arbre binaire
 * Params - NodeCalculator* node : sommet de l'arbre
 * Return - void
 */
void freeTreeCalculator(NodeCalculator* node) {
	if (node->left != NULL)
		freeTreeCalculator(node->right);
	if (node->right != NULL)
		freeTreeCalculator(node->left);
	free(node->value);
	free(node);
}

/* Creer un arbre binaire � partir d'une expression math�matique.
 * A chaque tours de boucle, 'limitTriangle' permet de r�cup�r� les indexes
 * d�but et de fin de la procha�ne sous expression � trait�. Cette sous-expression
 * permet ensuite de creer le potentiel sommet de l'arbre.
 * A chaque tours la chaine de l'expression est r��crite.
 * Params - char* expr : l'expression dans son entieret�e � traiter
 * Return - NodeCalculator* : sommet de l'arbre
 */
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

/* Ajoute la valeur d'un noeud de l'arbre dans une liste qui permettra lors
 *  de son parcours de d'obtenir la notation RPN de l'expression.
 * Le parcours est post fix�.
 * Params - NodeCalculator* expr : noed � traiter
 *			List* listNode : liste dans laquelle ins�rer la valeur du noeud
 * Return - void
 */
void addToRPNList(NodeCalculator* node, List* listNode) {
	if (node->left != NULL)
		addToRPNList(node->left, listNode);
	if (node->right != NULL)
		addToRPNList(node->right, listNode);
	append(listNode, &node->value);
}

/* Imprime l'arbre en post fix� dans la console
 * Params - NodeCalculator* expr : noed � traiter
 * Return - void
 */
void printPostFix(NodeCalculator* node) {
	if (node->right != NULL)
		printPostFix(node->right);
	if (node->left != NULL)
		printPostFix(node->left);
	printf("%s\n", node->value);
}

/* Permet d'obtenir la notation RPN � partir de l'arbre. 
 * Params - NodeCalculator* summit : sommet de l'arbre
 * Return - char* : notation RPN stock� dans une chaine de charact�res
 */
char* treeToRpn(NodeCalculator* summit) {
	List* listNode = get_list(NULL, sizeof(char**));
	addToRPNList(summit, listNode);
	char* strRPN = listStrToStr(listNode);
	free_list(listNode);
	return strRPN;
}

 /* Determine si l'expression � �t� entiermeent trait�e. L'expression est enti�rement trait� si il
  * ne reste plus qu'un terme commencant par 'a'
  * Params - char* expr : expression � v�rifier
  * Return - Boolean : 'TRUE' si l'expression � �t� entierement trait�e
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


 /* D�termine s'il le caract�re est un op�rateur prioritaire
  * Params - char c : charact�re � tester
  * Return - Boolean : 'TRUE' si c'est un op�rateur prioritaire
  */
Boolean isStrongOperator(char c) {
	char operators[] = "*/";
	Boolean isOpe = FALSE;
	for (int i = 0; i < 2; i++)
		if (operators[i] == c)
			isOpe = TRUE;
	return isOpe;
}

/* D�termine si le caract�re est num�rique (0-9)
  * Params - char c : charact�re � tester
  * Return - Boolean : 'TRUE' si c'est un op�rateur prioritaire
  */
Boolean isNumeric(char c) {
	if ((int)c >= 48 && (int)c <= 57)
		return TRUE;
	else
		return FALSE;
}

/* Retourne un entier � partie d'un flag de type 'a...'
 * ex : 'a12' => 12
 * Params - char* expr : flag a traiter
 * Return - Integer : entier obtenu � partir du flag
 */
int getIndex(char* expr) {
	char* value = cutStr(expr, 1, strlen(expr) - 1);
	int res = strToInt(value);
	free(value);
	return res;
}

/* Convertit une cha�ne en entier
 * Params - char* expr : cha�ne � convertir
 * Return - Integer : entier obtenu � partir de la cha�ne
 */
int strToInt(char* expr) {
	char* end;
	return strtol(expr, &end, 10);
}

 /* Cr�� un noued pr�t � �tre ins�r� dans l'arbre binaire � partir d'une chaine contenant
  * la valeur du noeud. Si la cha�ne commence par 'a', c'est que c'est un op�rateur d�j� pr�sent
  * dans la liste d'op�rateur.
  * Dans les autres cas il faut creer la stucture.
  * Params - char* expr : cha�ne contenant la valeur du noeud
  *		   - List* listPtr : liste des op�rateurs d�j� creer
  * Return - NodeCalculator* : le noeud correspondant � la valeur pass�e en param�tre
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

/* Renvoie une cha�ne �quivalent au contenue de l'interval pass� en apram�tre
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

/* Isole un nombre dans une chaine de charact�re, commence par �l�mininer les
 * les charact�res qui ne sont pas num�rique pour ne garder que les chiffre.
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

/* D�termine l'index de d�but fin de r��criture de la chaine.
 * Fait �galement atentition a ne supprimer qu'une parenth�se ouvrante dans le cas
 * de parenth�ses multiples ex : '((2+2)...'
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

/* Retourne les borne la prochaine sous exression � ins�r� dans l'arbre. Le retour se fait par
 * le biais des param�tres start et end. Start doit �tre initialis� au bon index avant l'appel
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