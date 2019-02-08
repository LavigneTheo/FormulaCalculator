#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"


typedef enum Boolean {
	TRUE = 1,
	FALSE = 0
}Boolean;

char* getFileName(char* path);

Boolean assertEqualsBool(Boolean var1, Boolean var2, char* path, char* line);
Boolean assertEqualsStr(char* var1, char* var2, char* path, char* line);
Boolean assertEqualsInt(int expected, int value, char* path, char* line);
Boolean assertNotEqualsInt(int expected, int value, char* path, char* line);
Boolean assertNull(void* value, char* path, char* line);
