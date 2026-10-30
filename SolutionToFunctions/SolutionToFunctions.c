#include "Cal.h"

char expression[maxExpressionLength + 1];

#define ApproxPrecision 0.000001
//The approximate precision of the solution (more accurate for less decimals)

#define AccuratePrecision 0.0001

//This variable takes the result of operations so we can check
//if the operation was successful or not.
//This variable shall be given several names
int returnValue = 0;
#define valuesTaken returnValue
#define calculateReturn returnValue
#define findSolutionReturn returnValue

bool findSolution(
	char *expr,
	double startingValue,
	double minValue,
	double maxValue,
	double *solution
){
	//This function Looks for a solution to the expression and writes it to solution.
	//Using NEWTON-RAPHSON METHOD
	//returns true if operation is successful
	
	//We need a duplicate so we can still identify X later from the original
	char duplicateExpr[maxExpressionLength + 1];
	
	double y1,y2;
	double x1,x2;
	double grad;
	
	char *charXLocation;
	size_t loopCount = 0;
	
	x1 = startingValue;
	do{//Executes for a predefined number of cycles
	//If result wasn't found before the maximum, it returns false
	
		if(x1 < minValue || x1 > maxValue){
			return false;
		}
		x2 = x1 - ApproxPrecision;
		
		strset(duplicateExpr,0);
		strcpy(duplicateExpr,expr);
		
		//We replace EVERY character 'X' by x1
		while( (charXLocation = strchr(duplicateExpr,'X')) != NULL){
			replaceWithValue(duplicateExpr,charXLocation,charXLocation,x1);
		}
		
		//We find Y1 using X1        
		calculateReturn = calculate(duplicateExpr,&y1);
		if(calculateReturn == 0){
			return false;
		}
		
		strset(duplicateExpr,0);
		strcpy(duplicateExpr,expr);
		
		//We replace EVERY character 'X' by x2
		while( (charXLocation = strchr(duplicateExpr,'X')) != NULL){
			replaceWithValue(duplicateExpr,charXLocation,charXLocation,x2);
		}
		
		//We find Y2 using X2
		calculateReturn = calculate(duplicateExpr,&y2);
		if(calculateReturn == 0){
			return false;
		}
		
		//If the results don't have the same sign, it means we have correctly
		//approximated the answer.
		if( (y1 * y2) <= 0){
			if(y1 == 0){
				*solution = x1;
			}else if(y2 == 0){
				*solution = x2;
			}else{
				*solution = x1;
			}
			return true;
		}
		
		grad = (y2-y1) / (x2-x1);
		x1 = x1 - y1 / grad;

		loopCount++;
	}while(loopCount <= 2000);
	
	return false;
}

bool isApproxPresent(double *presentValues, double testedValue, int N_Of_Values){
	//Checks if an approximate of the testedValue is already present in
	//An array of values
	size_t iter;
	
	for(iter = 0; iter < N_Of_Values; iter++){
		if( fabs(testedValue - presentValues[iter]) <= AccuratePrecision ){
			return true;
		}
	}
	return false;
}

void initializeDblArray(double *Array){
	size_t iter;
	
	for(iter = 0; iter < sizeof(Array); iter++){
		Array[iter] = 0;
	}
}

int main(void){
	
	double minValue, maxValue;
	double X;
	
	double ansHolder, ansTable[53];
	int ansCounter;
	
	char garbageHolder[maxExpressionLength + 1];
	//Used to discard of any nonsense coming from the user
	
	start:
	initializeDblArray(ansTable);
	ansCounter = 0;
	strset(expression, 0);
	
	printf("\nEnter a function:\n");
	do{
		fgets(expression,sizeof(expression),stdin);
		strupr(expression);
	}while(expression[0] == '\n');
	
	if(!strcmp(expression,"EXIT\n")){
		return 0;
	}
	
	//We verify that the variable X is found in the expresion
	if(strchr(expression,'X') == NULL){
		printf("Variable X not found.\n");
		goto start;
	}
	
	printf("Enter the range (minimum then maximum):\n");
	
	//User must enter the two values correctly
	valuesTaken = scanf("%lf %lf",&minValue,&maxValue);
	
	if(valuesTaken != 2){
		//This happens if scanf could not interprete the values (nonsense)
		printf("Value not recognized\n");
		
		//We take the nonsense and discard of it
		fgets(garbageHolder,sizeof(garbageHolder),stdin);
		strset(garbageHolder,0);
		
		goto start;
	}
	if(maxValue < minValue){
		printf("Invalid inputs\n");
		goto start;
	}
	
	//We select different starting points in a loop to extract answers, filtering duplicates.
	//To make sure the values don't always hit turning points in the expression,(which causes a bug)
	// we divide the searching range by a prime number (53 in this case)
	for(X = minValue; X <= maxValue; X += (maxValue - minValue) / 53){
		
		findSolutionReturn = findSolution(expression,X,minValue,maxValue,&ansHolder);

		if(findSolutionReturn == 0){
			continue;
		}
		
		if(!isApproxPresent(ansTable,ansHolder,ansCounter)){
			//If the solution isn't the same as a previously found one
			printf("Solution = %.4lf\n",ansHolder);
			
			ansTable[ansCounter] = ansHolder;
			ansCounter++;
		}
	}
	if(ansCounter == 0){
		printf("No Solution Found...\n");
	}
	goto start;
}