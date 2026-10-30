#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define maxExpressionLength 7500

double result;
double ans = 0;

bool expressionOversized(char *expr){
	if(strlen(expr) > maxExpressionLength){
		return true;
	}
	return false;
}

#define NumberOfBlocks 100
#define sizeOfFunction 7
typedef struct _block{
	char sign;
	char function[sizeOfFunction];
	double value;
}block;

void initializeBlock(block *data){
	data->sign = 0;
	strset(data->function,0);
	data->value = 0;
}

bool bracketCount(char *expr, size_t *N_BRACKET){
	//Counts the number of brackets present
	//returns false in case of bad bracket syntax detection
	size_t iter;
	size_t size = strlen(expr);
	size_t N_OP_BRACKET, N_CL_BRACKET;
	
	N_OP_BRACKET = N_CL_BRACKET = 0;
	
	for(iter = 0; iter < size; iter++){
		if(expr[iter] == '('){
			N_OP_BRACKET++;
			
		}else if(expr[iter] == ')'){
			N_CL_BRACKET++;
			
		}if(N_CL_BRACKET > N_OP_BRACKET){
			break;
		}
	}
	if(N_CL_BRACKET != N_OP_BRACKET){
		return false;
	}
	*N_BRACKET = N_OP_BRACKET;
	return true;
}

void innermostBracketExpression(char *expr, char **start, char **finish){
	//writes the start and end point of the sub-expression found in
	//the innermost bracket of the main expression
	*finish = strchr(expr,')');
	if(*finish == NULL){
		*start = expr;
		*finish = expr + strlen(expr);
	}else{
		*start = *finish;
		while( *((*start) - 1) != '(' ){
			(*start)--;
		}
	}
}

void expressionIntoBlocks(char *start, char *finish, block (*data)[NumberOfBlocks + 1], short *counter){
	char *position;
	short blockCounter = 0;
	
	for(position = start; position < finish; position++){
		while(*position == ' ' && position < finish){
			//shift position to the next character that ISN'T empty space
			position++;
		}
		if(position >= finish){
			break;
		}
		switch( *position ){
			case 'A' ... 'Z':
				if(!memcmp(position,"ANS",3)){
					(*data)[blockCounter].value = ans;
					blockCounter++;
				}else{
					if((*data)[blockCounter].function[0]){
						//If a function is present in the current block,
						//Add the next function in the next block
						blockCounter++;
					}if(!((*data)[blockCounter].sign)){
						//If the Sign is absent (number inserted before function),
						//The operation is multiplication
						(*data)[blockCounter].sign = '&';
					}
					sscanf(position,"%s",(*data)[blockCounter].function);
				}
			break;
			
			case '0' ... '9':
				(*data)[blockCounter].value = strtod(position,NULL);
				blockCounter++;
			break;
			
			default:
				//Since we checked the case for a character and a number,
				//we're left with signs handling
				if(*position != '\n'){
					switch(*(position + 1)){
						//If the sign comes with a number, it is the number's sign
						case '0' ... '9':
							(*data)[blockCounter].value = strtod(position,NULL);
							blockCounter++;
						break;
						
						//If the sign comes without a number, it is just a sign
						default:
						if((*data)[blockCounter].sign){
							blockCounter++;
						}
						(*data)[blockCounter].sign = *position;
						break;
					}
				}
			break;
		}
		while(*position != ' ' && position < finish){
			//shift position to the next character that IS empty space
			position++;
		}
		if(blockCounter > NumberOfBlocks + 1){
			//The highest allowed value for blockCounter is NumberOfBlocks + 1
			//because the last block MUST be empty (correct syntax)
			break;
		}
	}
	*counter = blockCounter;
}

bool isSyntaxIncorrect(block *data, short counter){
	size_t iter;
	for(iter = 0; iter < counter; iter++){
		if(!(data[iter].sign)){
			return true;
		}
	}
	return false;
}

bool executeFunction(char *function, double *value){
	//Breaks the function into a code to directly identify it in a switch statement
	//Because using if else would make the code slow
	
	//Then, executes the corresponding function and replaces value
	//Function returns true if execution is successful
	
	//We'll start by defining the function code constants
	#define COS 837967
	#define SIN 787383
	#define TAN 786584
	
	#define ACOS 83796765
	#define ASIN 78738365
	#define ATAN 78658465
	
	#define SQRT 84828183
	#define CBRT 84826667
	
	#define LOG 717976
	#define LN 7876
	
	#define COSH 72837967
	#define SINH 72787383
	#define TANH 72786584
	
	#define E 69
	
	#define SEC 676983
	#define COSEC 6769837967
	#define COT 847967
	
	#define SECH 72676983
	#define COSECH 726769837967
	#define COTH 72847967
	
	#define ACOSH 7283796765
	#define ASINH 7278738365
	#define ATANH 7278658465
	
	#define NO_FUNCTION 0
	#define PI 3.1415926535897931
	
	#define TO_RADIAN(value) (value * PI / 180)
	#define TO_DEGREE(value) (value * 180 / PI)
	
	unsigned long long code = 0;
	size_t iter;
	//Each character will be a 2 digit value so the next character
	//takes the next 2 digits to make a number (code)
	for(iter = 0; iter < sizeOfFunction; iter++){
		code += function[iter] * pow(100,iter);
	}
	switch(code){
		//Angles are always interpreted in degrees for this calculator
		//Angle conversion is corrected in the code
		case COS:
			*value = cos(TO_RADIAN(*value)); 
		break;
		
		case SIN: 
			*value = sin(TO_RADIAN(*value)); 
		break;
		
		case TAN:
			if(cos(TO_RADIAN(*value)) == 0){
				return false;
			}
			*value = tan(TO_RADIAN(*value));
		break;
		
		case ACOS:
			if(fabs(*value) > 1){
				return false;
			}
			*value = TO_DEGREE(acos(*value));
		break;
		
		case ASIN:
			if(fabs(*value) > 1){
				return false;
			}
			*value = TO_DEGREE(asin(*value));
		break;
		
		case ATAN:
			*value = TO_DEGREE(atan(*value));
		break;
		
		case SQRT:
			if( *value < 0){
				return false;
			}
			*value = sqrt(*value);
		break;
		
		case CBRT:
			*value = cbrt(*value);
		break;
		
		case LOG:
			if( *value <= 0){
				return false;
			}
			*value = log10(*value);
		break;
		
		case LN:
			if( *value <= 0){
				return false;
			}
			*value = log(*value);
		break;
		
		case COSH:
			*value = cosh(*value);
		break;
		
		case SINH:
			*value = sinh(*value);
		break;
		
		case TANH:
			*value = tanh(*value);
		break;
		
		case E:
			*value = pow(10,*value);
		break;
		
		case SEC:
			if(cos(TO_RADIAN(*value)) == 0){
				return false;
			}
			*value = 1 / cos(TO_RADIAN(*value));
		break;
		
		case COSEC:
			if(sin(TO_RADIAN(*value)) == 0){
				return false;
			}
			*value = 1 / sin(TO_RADIAN(*value));
		break;
		
		case COT:
			if(tan(TO_RADIAN(*value)) == 0){
				return false;
			}
			*value = 1 / tan(TO_RADIAN(*value));
		break;
		
		case SECH:
			*value = 1 / cosh(*value);
		break;
		
		case COSECH:
			if(sinh(*value) == 0){
				return false;
			}
			*value = 1 / sinh(*value);
		break;
		
		case COTH:
			if(tanh(*value) == 0){
				return false;
			}
			*value = 1 / tanh(*value);
		break;
		
		case ACOSH:
			if( *value < 1){
				return false;
			}
			*value = log( *value + sqrt( (*value) * (*value) - 1) );
		break;
		
		case ASINH:
			*value = log( *value + sqrt( (*value) * (*value) + 1) );
		break;
		
		case ATANH:
			if(fabs(*value) >= 1){
				return false;
			}
			*value = 0.5 * log( (1 + *value) / (1 - *value) );
		break;
		
		case NO_FUNCTION:
		break;
		
		default:
			return false;
		break;
	}
	return true;
}

bool isSignIncorrect(char sign){
	switch(sign){
		case '+': case '-': case '*': case '/': case '&': case '^':
			return false;
		break;
		default:
			return true;
		break;
	}
}

void replaceWithValue(char *expr, char *start, char *finish, double value){
	char holder[maxExpressionLength + 1];
	char *position;
	
	strset(holder,0);
	
	//Copy the part before the sub-expression
	for(position = expr; position < start - 1; position++){
		memcpy(holder+strlen(holder),position,1);
	}
	
	position--;//(We set position BEFORE the starting bracket)
	while(*position == ' ' && position > expr){
		position--;
	}
	if(position < expr){
		sprintf(holder+strlen(holder)," %.10e",value);
	}else{
		//Look for the entry before the sub-expression
		//If its a digit, we add a multiplication sign before the value
		switch(*position){
			case '0' ... '9':
				sprintf(holder+strlen(holder)," &");
				
			default:
				sprintf(holder+strlen(holder)," %.10e",value);
			break;
		}
	}
	
	//Copy the part after the sub-expresion
	strcpy(holder+strlen(holder),finish+1);
	
	//replaces the expression with the copy
	strset(expr,0);
	strcpy(expr,holder);
}

bool calculate(char *expr, double *answer){
	
	block blocks[NumberOfBlocks + 1];
	short blockCounter;
	
	char *currExprStart, *currExprEnd;
	
	size_t iter, numberOfBrackets;
	
	//We start by collecting the expr and turning all characters to uppercase
	//to remove case sensitivity
	strupr(expr);
	 
	//We need to make sure the expr does not go beyond a maximum predefined size
	if(expressionOversized(expr)){
		return false;
	}
	 
	//Now we need to first verify that all brackets in the expr have been
	//correctly setup (no closing before an opening and no opening without closing)
	//We also need to keep track of the number of brackets present to keep track of the
	//number of sub-expressions we will execute
	if(!bracketCount(expr,&numberOfBrackets)){
		return false;
	}
	 
	subExpressionStep:
	//Get the expr of the innermost bracket (starting point and end point)
	//gives the start and end of main expr if no brackets
	innermostBracketExpression(expr,&currExprStart,&currExprEnd);
	 
	//We initialize the blocks before reading the expr
	for(iter = 0; iter < NumberOfBlocks; iter++){
		initializeBlock(&blocks[iter]);
	}
	blocks[0].sign = '+'; //The first number is positive by default unless user enters negative
	 
	//We read the expr and break it into blocks, keeping their count
	expressionIntoBlocks(currExprStart,currExprEnd,&blocks,&blockCounter);
	 
	//Our number of blocks shouldn't be greater than the maximum
	if(blockCounter > NumberOfBlocks + 1){
		return false;
	}
	 
	//We always move to an empty block after putting a value, so :
	//If the last block isn't completely empty, it's an error (missing value)
	if(blocks[blockCounter].function[0] || blocks[blockCounter].sign){
		return false;
	}
	 
	//Syntax Checking
	if(isSyntaxIncorrect(blocks,blockCounter)){
		return false;
	}
	 
	//We iterate through the different blocks, execute the function and check the sign
	for(iter = 0; iter < blockCounter; iter++){
		if(!executeFunction(blocks[iter].function, &blocks[iter].value) || isSignIncorrect(blocks[iter].sign)){
			return false;
		}
	}
	 
	//Since the functions have been executed, we are left with signs and values
	//We now perform BODMAS
	
	//We define the general BODMAS macro
	//It looks for a sign, executes the operation, then erases the block
	//that has been processed, then goes over the blocks once more until
	//none of that sign can be found anymore
	#define BODMASstep(AllBlocks, Sign, Operation, FailCondition)	\
	for(iter = 0; iter < blockCounter; iter++){						\
		if(AllBlocks[iter].sign == Sign){							\
			if(FailCondition){										\
				return false;										\
			}														\
			AllBlocks[iter-1].value = Operation;					\
																	\
			for(iter = iter + 1; iter < blockCounter + 1; iter++){	\
				AllBlocks[iter-1].sign = AllBlocks[iter].sign;		\
				AllBlocks[iter-1].value = AllBlocks[iter].value;	\
			}														\
			blockCounter--;											\
			iter = -1;												\
		}															\
	}
	
	BODMASstep(blocks, '^', pow(blocks[iter-1].value,blocks[iter].value), false);
	BODMASstep(blocks, '&', blocks[iter-1].value * blocks[iter].value, false);
	BODMASstep(blocks, '/', blocks[iter-1].value / blocks[iter].value, blocks[iter].value == 0);
	BODMASstep(blocks, '*', blocks[iter-1].value * blocks[iter].value, false);
	
	#undef BODMASstep												
	
	//Addition and subtraction
	result = 0;
	for(iter = 0; iter < blockCounter; iter++){
		switch(blocks[iter].sign){
			case '+':
				result += blocks[iter].value;
			break;
			
			case '-':
				result -= blocks[iter].value;
			break;
		}
	}
	if(numberOfBrackets){
		//Rewrite the original expr , replacing the innermost bracket with
		//the result of the executed sub-expr
		replaceWithValue(expr,currExprStart,currExprEnd,result);
		numberOfBrackets--;
		goto subExpressionStep;
	}else{
		*answer = result;
		return true;
	}
}

#undef NumberOfBlocks
#undef sizeOfFunction