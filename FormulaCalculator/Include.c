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


Boolean assertEqualsBool(Boolean expected, Boolean value, const char* path, long line) {
	if (expected != value) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of equal BOOLEAN failed in '%s' at line %ld\n", fileName, line);
		printf("Got : %d, Expected : %d\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertEqualsStr(char* expected, char* value, const char* path, long line) {
	if (strcmp(expected, value) != 0) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of equal STRING failed in '%s' at line %ld\n", fileName, line);
		printf("Got : '%s', Expected : '%s'\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertEqualsInt(int expected, int value, const char* path, long line) {
	if (expected != value) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of equal INT failed in '%s' at line %ld\n", fileName, line);
		printf("Got : %d, Expected : %d\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertEqualsFloat(float expected, float value, const char* path, long line) {
	float marginError = 0.00001f;
	if (fabsf(expected - value) > marginError) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of equal FLOAT failed in '%s' at line %ld\n", fileName, line);
		printf("Got : %f, Expected : %f\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertNotEqualsInt(int expected, int value, const char* path, long line) {
	if (expected == value) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion of NOT equal INT failed in '%s' at line %ld\n", fileName, line);
		printf("Got : %d, Should not be : %d\n\n", value, expected);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

Boolean assertNull(void* value, char* const path, long line) {
	if (value != NULL) {
		char* fileName = getFileName(path);
		printf("ERROR : assertion NULL failed in '%s' at line %ld\n", fileName, line);

		free(fileName);
		return FALSE;
	}
	else {
		return TRUE;
	}
}