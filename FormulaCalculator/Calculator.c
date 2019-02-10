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

/* Coupe une cha�ne sur un intervalle donn�e et renvoie le r�sultat.
 * Params - char* expr : cha�ne � couper
 *		  - int start : d�but de la nouvelle cha�ne
 *		  - int end : fin de la nouvelle cha�ne
 * Return - char* : cha�ne entre les borne start et end
 */
char* cutStr(char* expr, int start, int end) {
	char* res = malloc(sizeof(char) * (end - start) + 2);
	*(res + end - start + 1) = '\0';
	for (int i = 0; i <= (end - start); i++)
		*(res + i) = *(expr + i + start);
	return res;
}

 /* A partir d'une cha�ne de caract�re, isole la valeur du noeud � creer.
  * Cette valeur peut �tre un nombre ou un 'flag' commencant par un 'a'. Les deux
  * boucles while servent � �liminer les espaces et parenth�se pour ne garder qu'un nombre ou flag.
  * Params - char* expr : cha�ne � � traiter
  *		   - int start : ou commence la recherche du nombre / flag dans la cha�ne.
  *		   - int end : ou se fini la recherche du nombre / flag dans la cha�ne.
  *		   - char** value : RETURN
  * Return - char** value : le r�sultat est stock�e dans la variable 'value' pass�e en param�tre.
  *							Elle contient une cha�ne contenant un flag ou un nombre.
  */
void getValue(char* expr, int start, int end, char** value) {
	while (isNumeric(*(expr + start)) == FALSE && *(expr + start) != 'a')
		start++;
	end = start;
	while (isNumeric(*(expr + end + 1)) == TRUE)
		end++;
	*value = cutStr(expr, start, end);
}

/* Convertit un entier en cha�ne
 * Params - int value : valeur � convertir en cha�ne
 * Return - char* : l'�quivalent en cha�ne de caract�re du nombre pass� en param�tre
 */
char* intToStr(int value) {
	char* res = malloc(sizeof(char) * 13);
	sprintf(res, "%d", value);
	return res;
}

 /* D�termine l'intervalle sur lequelle r��crire la formule.
  * Le premier if traite le cas ou l'on garde une parenth�se en 1er caract�re : '(6 * 3'
  * Le premier if traite le cas ou l'on garde une parenth�re pr�sente en dernier caract�re : '6 * 3)'
  * La boucle while traite le cas ou plusieurs parenth�ses sont pr�sente en d�but d'�xpr�ssion. Dans ce
  * on ne souhaite n'est supprim� qu'une : '((9+1)'
  * Appele ensuite la fonction de r��criture sur l'interval obtenu
  * Params - char* expr : cha�ne contenant l'expression
  *		   - int start : d�but de l'intervalle sur lequel r��crire l'expression
  *		   - int start : fin de l'intervalle sur lequel r��crire l'expression
  *		   - int index : index auxquel � �t� ins�r� l'op�rateur dans la liste, lors de la r��criture,
						 l'expression sera remplacer par un flag commencant par 'a' suivi de l'index : 'a18'
  * Return - char* : la cha�ne mise � jours
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

/* Remplace une partir de l'expression pass� en param�tre par un flag. ex : 'a18'
 * Params - char* expr : l'expression � modifier
 *		  - int start : d�but de l'interval sur lequel r��crire la fonction
 *		  - int end : fin de l'interval sur lequel r��crire la fonction
 *		  - int index : index auxquel � �t� ins�r� l'op�rateur dans la liste
 * Return - char* : expression mise � jours, o� l'interval � �t� remplacer par un flag 
 */
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

/* Calcul r�cursivement l'expression stocker dans l'arbre binaire. On va syst�matiquement
 * sur les noeuds situ�s � gauche, en revanche on ne descent � droite que dans es cas o� 
 * le noeud est un op�rateur.
 * On calcul ensuite la valeur du noued ssi le noeud est un op�rateur.
 * En fin de processus le sommet de l'arbre contient dans le champ 'totalValue' la valeur de 
 * l'expression stock�e.
 * Params - NodeCalculator* node : sommet de l'arbre
 * Return - void
 */
void processTree(NodeCalculator* node) {
	if (node->left != NULL)
		processTree(node->left);
	if (node->right != NULL && isOperator(*node->right->value))
		processTree(node->right);
	if (isOperator(*node->value))
		ProcessNodeValue(node);
}

/* Retourne la valeur du noeud pass� en param�tre. Si c'est un op�rateur, sa valeur est
 * stock� dans le champ 'totalValue', sinon on convertit le value du noeud en entier puis
 * on le cast en float.
 * Params - NodeCalculator* node : noeud dont on veut obtenir la valeur
 * Return - float : valeur num�rique du noeud
 */
float getNodeFloatValue(NodeCalculator* node) {
	if (node->operator == TRUE)
		return node->totalValue;
	return  (float)strToInt(node->value);
}

/* A partir d'un noeud contenant un op�rateur, permet d'obtenir les deux nombre 
 * n�c�ssaires pour �ff�ctuer l'op�ration. Si lors de l'insertion, l'ordre � �t�
 * invers�, on inverse de nouveau les termes. Cela permet au op�ration non commutative de
 * fonctionner correctement.
 * Params - NodeCalculator* node : op�rateur
 *	      - float* num1 : RETURN
 *	      - float* num2 : RETURN
 * Return - float num1 : valeur du premier terme de l'op�ration
 *		  - float num2 : valeur du second terme de l'op�ration
 */
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

/* Effectue une op�ration � partir d'un op�rateur et la stocke dans le champ 'totlValue'. 
 * R�cup�re pr�alablement les deux termes n�c�ssaires pour �ff�ctuer l'op�ration.
 * Params - NodeCalculator* node : noeud dont on veut obtenir la valeur
 * Return - void
 */
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

 /* D�termine l'interval de la prochaine op�ration � �ff�cteur. Dans un premier temps, on cerche un premier op�rateur.
  * Lorsque l'on trouve cet op�rateur, on garde en m�moire son index, ainsi que la priorit� de cet op�rteur. 
  * Ensuite c'est la fonction 'firstOperatorFound' qui va d�terminer si determine si l'on a �ff�ctivement trouver une expression
  * valide. Si l'on a pas trouver l'expression en fin de boucle, on teste le caract�re suivant.
  * Params - char* expr : expression o� trouver une op�ration
  *		   - int* start : d�but de l'interval o� chercher l'op�ration
  *		   - int* end : �gal � start
  * Return - int* start : d�but de l'interval o� se trouve l'op�ration
  *		   - int* end : fin de l'interval o� se trouve l'op�ration
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
			firstOperatorFound(expr, start, end, indexLastOperator, &foundOperator, &found, strongOperator);
		}
		if (!found)
			(*end)++;
	}
}

/* D�termine si le caract�re � l'index 'end' donne la fin d'une op�ration qui peut �tre trait�e. Une op�ration  peut
 * �tre trait�e si elle respespect l'ordre de priorit� (priorit� du *  sur le + par ex). 
 * 1er if : le caract�re est une parenth�se ouvrante, l'expression pr�c�dente n'est pas � trait� pour le moment.
 * 2em if : le caract�re est une paenth�se fermante, l'expression peut �tre trait�e. 
 * 3e� if : le caract�re est un autre op�rateur. Si l'op�rateur pr�c�dent un op�rateur fort, l'expression peut �tre trait�e.
 *          Sinon on pr�fera app�l� de nouveau 'limitTriangle' sur un interval excluant les caract�re pr�c�dent le premier
 *          op�rateur ainsi que l'op�rateur lui m�me. Cela traite le cas : '9 + 9 + 8 * 2' o� 8 * 2 est l'op�ration prioritaire.
 * 4em if : On a atteint la fin de la cha�ne, on peut traiter l'op�ration pr�c�dente puisqu'il n'y ne peut y avoir des op�ration
 *          prioritaires ensuite.
 * Params - char* expr : expression o� trouver une op�ration
 *		   - int* start : d�but de l'interval o� chercher l'op�ration
 *		   - int* end : caract�re de la possible fin de l'op�ration
 *		   - int end : l'index du premier op�rateur trouv�
 *		   - Boolean* foundOperator : si on � trouver le premier op�rateur de l'expression.
 *		   - Boolean* found : si on � trouver les bornes de l'expression
 *		   - Boolean strongOperator : si le premier op�rateur trouv� est un op�rateur prioritaire
 * Return - int* start : d�but de l'interval o� se trouve l'op�ration
 *		  - int* end : fin de l'interval o� se trouve l'op�ration
 *		  - Boolean* foundOperator : si l'on doit trouver de nouveau un premier op�rateur, mis � 'FALSE'
 *		  - Boolean* found : si l'on � trouver les bornes de l'op�ration
 */
void firstOperatorFound(char* expr, int* start, int* end, int indexLastOperator, Boolean* foundOperator, Boolean* found, Boolean strongOperator) {
	if (*(expr + *end) == '(') {
		*start = *end;
		*foundOperator = FALSE;
	}
	else if (*(expr + *end) == ')') {
		*found = TRUE;
	}
	else  if (isOperator(*(expr + *end))) {
		if (strongOperator) {
			(*end)--;
		}
		else {
			*start = *end = indexLastOperator + 1;
			limitTriangle(expr, start, end);
		}
		*found = TRUE;
	}
	else if (*end + 1 >= strlen(expr)) {
		*found = TRUE;
	}
}