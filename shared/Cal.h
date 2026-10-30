#ifndef _CALCULATOR_PROTOTYPE_
#define _CALCULATOR_PROTOTYPE_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define maxExpressionLength 7500

void replaceWithValue(char *expr, char *start, char *finish, double value);
bool calculate(char *expr, double *answer);

#endif