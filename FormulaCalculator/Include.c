#include "Include.h"

char* getFileName(char* path) {
	int length = strlen(path);
	int index = length - 1;
	while (*(path + index) != '\\')
		index--;
	char* fileName = malloc(sizeof(char) * (length - index + 1));
	*(fileName + length - index) = '\0';
	for (int i = 0; i < length - index; i++)
		*(fileName + i) = *(path + i + index + 1);

	return fileName;
}


Boolean assertEqualsBool(Boolean expected, Boolean value, char* path, char* line) {
	if (expected != value) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of equal BOOLEAN failed in '%s' at line %d\n", fileName, line);
		printf("Got : %d, Expected : %d\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertEqualsStr(char* expected, char* value, char* path, char* line) {
	if (strcmp(expected, value) != 0) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of equal STRING failed in '%s' at line %d\n", fileName, line);
		printf("Got : '%s', Expected : '%s'\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertEqualsInt(int expected, int value, char* path, char* line) {
	if (expected != value) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of equal INT failed in '%s' at line %d\n", fileName, line);
		printf("Got : %d, Expected : %d\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertNotEqualsInt(int expected, int value, char* path, char* line) {
	if (expected == value) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of NOT equal INT failed in '%s' at line %d\n", fileName, line);
		printf("Got : %d, Should not be : %d\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertNull(void* value, char* path, char* line) {
	if (value != NULL) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion NULL failed in '%s' at line %d\n", fileName, line);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}