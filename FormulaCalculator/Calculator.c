#include "Calculator.h"

/* Détermine si le caractère passé en paramètre est un opérateur
 * Params - char : le caractère à tester
 * Return - Boolean : TRUE si c'est un opérateur sinon false
 */
Boolean isOperator(char c) {
	char operators[] = "+-*/";
	Boolean isOpe = FALSE;
	for (int i = 0; i < 4; i++)
		if (operators[i] == c)
			isOpe = TRUE;
	return isOpe;
}

/* Détermine l'index auquel se trouve l'operateur dans une chaîne
 * Params - char* : la chaîne sur laquelle faire la recherche
 * Return - Integer : index ou se trouve l'opérateur dans la chaîne
 */
int getOperatorIndex(char* expr)
{
	int i = 0;
	while (isOperator(*(expr + i)) == FALSE)
		i++;
	return i;
}

/* Détermine l'opérateur dans une chaîne, renvoi un pointeur vers un caractère 
 * Params - char* : la chaîne où chercher l'opérateur
 * Return - char* : pointeur vers le caractère représentant l'opérateur
 */
char* getOperator(char* expr) {
	int i = getOperatorIndex(expr);
	char* ope = malloc(sizeof(char) * 2);
	*(ope + 1) = '\0';
	*ope = *(expr + i);
	return ope;
}

/* Creer le noeud a partir du premier terme d'une opération mathématique
 * ex : '3 + 1', 3 est le premier élément 
 * L'expression contient toujours un seul et unique opérateur
 * Params - char* : chaine contenant l'opération
 * Return - NodeCalculator* : structure à stocker dans l'arbre binaire
 */
NodeCalculator* createFirstElem(char* subExpr, List* listOperatorPtr) {
	char** value = malloc(sizeof(char*));
	getValue(subExpr, 0, strlen(subExpr) - 1, value);
	NodeCalculator* left = createNodeCalculator(*value, listOperatorPtr);
	free(value);
	return left;
}

/* Creer le noeud a partir du second terme d'une opération mathématique
 * ex : '3 + 1', 1 est le second élément
 * L'expression contient toujours un seul et unique opérateur
 * Params - char* : chaine contenant l'opération
 * Return - NodeCalculator* : structure à stocker dans l'arbre binaire
 */
NodeCalculator* createSecondElem(char* subExpr, List* listOperatorPtr) {
	char** value = malloc(sizeof(char*));
	int start = getOperatorIndex(subExpr);
	getValue(subExpr, start, strlen(subExpr) - 1, value);
	NodeCalculator* right = createNodeCalculator(*value, listOperatorPtr);
	free(value);
	return right;
}



/* Place les enfants d'un opérateur.
 * Si l'élément de gauche n'est pas un  opérateur, on met le second terme à gauche. Cela enmpèche
 * de se retrouver dans une situation où l'on aurait à un nombre, et à droite un opérateur avec lui'même
 * d'autre enfants qui déséquilibrerai l'arbre.
 * Lorsque l'inverse les enfant, on met le champ 'reverse' de la structure de l'opérateur à 'TRUE', sinon
 * les opérations non commutatives (-, / etc) ne pourrait être calculer correctement à cause de l'inversion.
 * Params - NodeCalculator* summit : l'opérateur de l'opération, potentiel sommet de l'arbre
 *			NodeCalculator* first : le premier terme de l'opération
 *			NodeCalculator* second : le second terme de l'opération
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


/* Creer un triangle composé d'un opérateur comme sommet. Les enfants peuvent être
 * un nombre ou au autre opérateur.
 * L'opération passée en paramètre contient un seul et unique opérateur.
 * Params - char* subExpr : opération a effectuer
 *			List* listOperatorPtr : liste permettant de stocker les opérateurs présent dans l'arbre
 * Return - NodeCalculator* : structure contenant l'opérateur, potentiel sommet de l'arbre
 */
NodeCalculator* createTriangle(char* subExpr, List* listOperatorPtr) {
	NodeCalculator* firstElem = createFirstElem(subExpr, listOperatorPtr);

	char* value = getOperator(subExpr);
	NodeCalculator* summit = createNodeCalculator(value, listOperatorPtr);

	NodeCalculator* secondElem = createSecondElem(subExpr, listOperatorPtr);

	placeChildren(summit, firstElem, secondElem);
	
	return summit;
}

/* Supprime récursivement les structures présente dans l'arbre binaire
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

/* Creer un arbre binaire à partir d'une expression mathématique.
 * A chaque tours de boucle, 'limitTriangle' permet de récupéré les indexes
 * début et de fin de la prochaîne sous expression à traité. Cette sous-expression
 * permet ensuite de creer le potentiel sommet de l'arbre.
 * A chaque tours la chaine de l'expression est réécrite.
 * Params - char* expr : l'expression dans son entieretée à traiter
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
 * Le parcours est post fixé.
 * Params - NodeCalculator* expr : noed à traiter
 *			List* listNode : liste dans laquelle insérer la valeur du noeud
 * Return - void
 */
void addToRPNList(NodeCalculator* node, List* listNode) {
	if (node->left != NULL)
		addToRPNList(node->left, listNode);
	if (node->right != NULL)
		addToRPNList(node->right, listNode);
	append(listNode, &node->value);
}

/* Imprime l'arbre en post fixé dans la console
 * Params - NodeCalculator* expr : noed à traiter
 * Return - void
 */
void printPostFix(NodeCalculator* node) {
	if (node->right != NULL)
		printPostFix(node->right);
	if (node->left != NULL)
		printPostFix(node->left);
	printf("%s\n", node->value);
}

/* Permet d'obtenir la notation RPN à partir de l'arbre. 
 * Params - NodeCalculator* summit : sommet de l'arbre
 * Return - char* : notation RPN stocké dans une chaine de charactères
 */
char* treeToRpn(NodeCalculator* summit) {
	List* listNode = get_list(NULL, sizeof(char**));
	addToRPNList(summit, listNode);
	char* strRPN = listStrToStr(listNode);
	free_list(listNode);
	return strRPN;
}

 /* Determine si l'expression à été entiermeent traitée. L'expression est entièrement traité si il
  * ne reste plus qu'un terme commencant par 'a'
  * Params - char* expr : expression à vérifier
  * Return - Boolean : 'TRUE' si l'expression à été entierement traitée
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
  * Params - char c : charactère à tester
  * Return - Boolean : 'TRUE' si c'est un opérateur prioritaire
  */
Boolean isStrongOperator(char c) {
	char operators[] = "*/";
	Boolean isOpe = FALSE;
	for (int i = 0; i < 2; i++)
		if (operators[i] == c)
			isOpe = TRUE;
	return isOpe;
}

/* Détermine si le caractère est numérique (0-9)
  * Params - char c : charactère à tester
  * Return - Boolean : 'TRUE' si c'est un opérateur prioritaire
  */
Boolean isNumeric(char c) {
	if ((int)c >= 48 && (int)c <= 57)
		return TRUE;
	else
		return FALSE;
}

/* Retourne un entier à partie d'un flag de type 'a...'
 * ex : 'a12' => 12
 * Params - char* expr : flag a traiter
 * Return - Integer : entier obtenu à partir du flag
 */
int getIndex(char* expr) {
	char* value = cutStr(expr, 1, strlen(expr) - 1);
	int res = strToInt(value);
	free(value);
	return res;
}

/* Convertit une chaîne en entier
 * Params - char* expr : chaîne à convertir
 * Return - Integer : entier obtenu à partir de la chaîne
 */
int strToInt(char* expr) {
	char* end;
	return strtol(expr, &end, 10);
}

 /* Créé un noued prèt à être inséré dans l'arbre binaire à partir d'une chaine contenant
  * la valeur du noeud. Si la chaîne commence par 'a', c'est que c'est un opérateur déjà présent
  * dans la liste d'opérateur.
  * Dans les autres cas il faut creer la stucture.
  * Params - char* expr : chaîne contenant la valeur du noeud
  *		   - List* listPtr : liste des opérateurs déjà creer
  * Return - NodeCalculator* : le noeud correspondant à la valeur passée en paramètre
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

/* Coupe une chaîne sur un intervalle donnée et renvoie le résultat.
 * Params - char* expr : chaîne à couper
 *		  - int start : début de la nouvelle chaîne
 *		  - int end : fin de la nouvelle chaîne
 * Return - char* : chaîne entre les borne start et end
 */
char* cutStr(char* expr, int start, int end) {
	char* res = malloc(sizeof(char) * (end - start) + 2);
	*(res + end - start + 1) = '\0';
	for (int i = 0; i <= (end - start); i++)
		*(res + i) = *(expr + i + start);
	return res;
}

 /* A partir d'une chaîne de caractère, isole la valeur du noeud à creer.
  * Cette valeur peut être un nombre ou un 'flag' commencant par un 'a'. Les deux
  * boucles while servent à éliminer les espaces et parenthèse pour ne garder qu'un nombre ou flag.
  * Params - char* expr : chaîne à à traiter
  *		   - int start : ou commence la recherche du nombre / flag dans la chaîne.
  *		   - int end : ou se fini la recherche du nombre / flag dans la chaîne.
  *		   - char** value : RETURN
  * Return - char** value : le résultat est stockée dans la variable 'value' passée en paramètre.
  *							Elle contient une chaîne contenant un flag ou un nombre.
  */
void getValue(char* expr, int start, int end, char** value) {
	while (isNumeric(*(expr + start)) == FALSE && *(expr + start) != 'a')
		start++;
	end = start;
	while (isNumeric(*(expr + end + 1)) == TRUE)
		end++;
	*value = cutStr(expr, start, end);
}

/* Convertit un entier en chaîne
 * Params - int value : valeur à convertir en chaîne
 * Return - char* : l'équivalent en chaîne de caractère du nombre passé en paramètre
 */
char* intToStr(int value) {
	char* res = malloc(sizeof(char) * 13);
	sprintf(res, "%d", value);
	return res;
}

 /* Détermine l'intervalle sur lequelle réécrire la formule.
  * Le premier if traite le cas ou l'on garde une parenthèse en 1er caractère : '(6 * 3'
  * Le premier if traite le cas ou l'on garde une parenthère présente en dernier caractère : '6 * 3)'
  * La boucle while traite le cas ou plusieurs parenthèses sont présente en début d'éxpréssion. Dans ce
  * on ne souhaite n'est supprimé qu'une : '((9+1)'
  * Appele ensuite la fonction de réécriture sur l'interval obtenu
  * Params - char* expr : chaîne contenant l'expression
  *		   - int start : début de l'intervalle sur lequel réécrire l'expression
  *		   - int start : fin de l'intervalle sur lequel réécrire l'expression
  *		   - int index : index auxquel à été inséré l'opérateur dans la liste, lors de la réécriture,
						 l'expression sera remplacer par un flag commencant par 'a' suivi de l'index : 'a18'
  * Return - char* : la chaîne mise à jours
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

/* Remplace une partir de l'expression passé en paramètre par un flag. ex : 'a18'
 * Params - char* expr : l'expression à modifier
 *		  - int start : début de l'interval sur lequel réécrire la fonction
 *		  - int end : fin de l'interval sur lequel réécrire la fonction
 *		  - int index : index auxquel à été inséré l'opérateur dans la liste
 * Return - char* : expression mise à jours, où l'interval à été remplacer par un flag 
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

/* Calcul récursivement l'expression stocker dans l'arbre binaire. On va systèmatiquement
 * sur les noeuds situés à gauche, en revanche on ne descent à droite que dans es cas où 
 * le noeud est un opérateur.
 * On calcul ensuite la valeur du noued ssi le noeud est un opérateur.
 * En fin de processus le sommet de l'arbre contient dans le champ 'totalValue' la valeur de 
 * l'expression stockée.
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

/* Retourne la valeur du noeud passé en paramètre. Si c'est un opérateur, sa valeur est
 * stocké dans le champ 'totalValue', sinon on convertit le value du noeud en entier puis
 * on le cast en float.
 * Params - NodeCalculator* node : noeud dont on veut obtenir la valeur
 * Return - float : valeur numérique du noeud
 */
float getNodeFloatValue(NodeCalculator* node) {
	if (node->operator == TRUE)
		return node->totalValue;
	return  (float)strToInt(node->value);
}

/* A partir d'un noeud contenant un opérateur, permet d'obtenir les deux nombre 
 * nécéssaires pour éfféctuer l'opération. Si lors de l'insertion, l'ordre à été
 * inversé, on inverse de nouveau les termes. Cela permet au opération non commutative de
 * fonctionner correctement.
 * Params - NodeCalculator* node : opérateur
 *	      - float* num1 : RETURN
 *	      - float* num2 : RETURN
 * Return - float num1 : valeur du premier terme de l'opération
 *		  - float num2 : valeur du second terme de l'opération
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

/* Effectue une opération à partir d'un opérateur et la stocke dans le champ 'totlValue'. 
 * Récupère préalablement les deux termes nécéssaires pour éfféctuer l'opération.
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

 /* Détermine l'interval de la prochaine opération à éffécteur. Dans un premier temps, on cerche un premier opérateur.
  * Lorsque l'on trouve cet opérateur, on garde en mémoire son index, ainsi que la priorité de cet opérteur. 
  * Ensuite c'est la fonction 'firstOperatorFound' qui va déterminer si determine si l'on a éfféctivement trouver une expression
  * valide. Si l'on a pas trouver l'expression en fin de boucle, on teste le caractère suivant.
  * Params - char* expr : expression où trouver une opération
  *		   - int* start : début de l'interval où chercher l'opération
  *		   - int* end : égal à start
  * Return - int* start : début de l'interval où se trouve l'opération
  *		   - int* end : fin de l'interval où se trouve l'opération
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

/* Détermine si le caractère à l'index 'end' donne la fin d'une opération qui peut être traitée. Une opération  peut
 * être traitée si elle respespect l'ordre de priorité (priorité du *  sur le + par ex). 
 * 1er if : le caractère est une parenthèse ouvrante, l'expression précédente n'est pas à traité pour le moment.
 * 2em if : le caractère est une paenthèse fermante, l'expression peut être traitée. 
 * 3eù if : le caractère est un autre opérateur. Si l'opérateur précédent un opérateur fort, l'expression peut être traitée.
 *          Sinon on préfera appélé de nouveau 'limitTriangle' sur un interval excluant les caractère précédent le premier
 *          opérateur ainsi que l'opérateur lui même. Cela traite le cas : '9 + 9 + 8 * 2' où 8 * 2 est l'opération prioritaire.
 * 4em if : On a atteint la fin de la chaîne, on peut traiter l'opération précédente puisqu'il n'y ne peut y avoir des opération
 *          prioritaires ensuite.
 * Params - char* expr : expression où trouver une opération
 *		   - int* start : début de l'interval où chercher l'opération
 *		   - int* end : caractère de la possible fin de l'opération
 *		   - int end : l'index du premier opérateur trouvé
 *		   - Boolean* foundOperator : si on à trouver le premier opérateur de l'expression.
 *		   - Boolean* found : si on à trouver les bornes de l'expression
 *		   - Boolean strongOperator : si le premier opérateur trouvé est un opérateur prioritaire
 * Return - int* start : début de l'interval où se trouve l'opération
 *		  - int* end : fin de l'interval où se trouve l'opération
 *		  - Boolean* foundOperator : si l'on doit trouver de nouveau un premier opérateur, mis à 'FALSE'
 *		  - Boolean* found : si l'on à trouver les bornes de l'opération
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