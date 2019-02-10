#include "Test.h"

void main()
{
	launchTests();
	char* expr = askExpr();
	NodeCalculator* summit = createTreeFromExpr(expr);
	processTree(summit);
	printf("Resultat : %f\n", summit->totalValue);

	freeTreeCalculator(summit);
}
