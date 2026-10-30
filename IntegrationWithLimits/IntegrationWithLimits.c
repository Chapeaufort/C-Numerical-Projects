#include "Cal.h"

char expression[maxExpressionLength + 1];

//This variable takes the result of operations so we can check
//if the operation was successful or not.
//This variable shall be given several names
int returnValue = 0;
#define valuesTaken returnValue
#define calculateReturn returnValue
#define limitIntegralReturn returnValue

#define isEven(value) (!(value % 2))
#define isOdd(value) (value % 2)

#define N_OF_STEPS 60000

#define FIRST_VALUE 0
#define LAST_VALUE 60000

bool limitIntegral(char *expr, double lowerLimit, double higherLimit, double *answer){
	//This function takes an expression and calculates the integral (with limits)
	//uses SIMPSON'S 1/3 RULE
	//returns true if operation is successful
	
	double totalSum = 0;
	double X, ansHolder;
	int valueNumber = 0;
	
	//We need a duplicate so we can still identify X later from the original
	char duplicateExpr[maxExpressionLength + 1];
	char *charXLocation;
	
	double h = (higherLimit - lowerLimit) / N_OF_STEPS;
	
	for(X = lowerLimit; X <= higherLimit; X += h){
		
		strset(duplicateExpr,0);
		strcpy(duplicateExpr,expr);
		
		//We replace EVERY character 'X' by X
		while( (charXLocation = strchr(duplicateExpr,'X')) != NULL){
			replaceWithValue(duplicateExpr,charXLocation,charXLocation,X);
		}
		
		calculateReturn = calculate(duplicateExpr,&ansHolder);
		if(calculateReturn == 0){
			return false;
		}
		
		if(valueNumber == FIRST_VALUE || valueNumber == LAST_VALUE){
			totalSum += ansHolder;
			
		}else if(isOdd(valueNumber)){
			totalSum += 4 * ansHolder;
			
		}else if(isEven(valueNumber)){
			totalSum += 2 * ansHolder;
			
		}
		valueNumber++;
	}
	*answer = (h / 3) * totalSum;
	return true;
}

int main(void){
	
	double lowerLimit, higherLimit;
	double answer;
	
	char garbageHolder[maxExpressionLength + 1];
	//Used to discard of any nonsense coming from the user
	
	start:
	strset(expression,0);
	
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
	
	printf("Enter the limits (lower then higher):\n");
	
	//User must enter the two values correctly
	valuesTaken = scanf("%lf %lf",&lowerLimit,&higherLimit);
	
	if(valuesTaken != 2){
		//This happens if scanf could not interprete the values (nonsense)
		printf("Value not recognized\n");
		
		//We take the nonsense and discard of it
		fgets(garbageHolder,sizeof(garbageHolder),stdin);
		strset(garbageHolder,0);
		
		goto start;
	}
	if(higherLimit < lowerLimit){
		printf("Invalid inputs\n");
		goto start;
	}
	
	//We calculate the integral
	limitIntegralReturn = limitIntegral(expression,lowerLimit,higherLimit,&answer);
	if(limitIntegralReturn == 0){
		printf("Integral could not be calculated\n");
		goto start;
	}
	printf("The result is : %.4lf\n",answer);
	goto  start;
}