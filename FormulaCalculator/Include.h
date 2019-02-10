#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "List.h"


typedef enum Boolean {
	TRUE = 1,
	FALSE = 0
}Boolean;

char* getFileName(char* path);
char* askExpr();

Boolean assertEqualsBool(Boolean var1, Boolean var2, char* path, long line);
Boolean assertEqualsStr(char* var1, char* var2, char* path, long line);
Boolean assertEqualsInt(int expected, int value, char* path, long line);
Boolean assertNotEqualsInt(int expected, int value, char* path, long line);
Boolean assertEqualsFloat(float expected, float value, char* path, long line);
Boolean assertNull(void* value, char* path, long line);
