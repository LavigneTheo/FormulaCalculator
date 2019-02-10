#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef void(*free_function)(void*);

typedef struct List {
	int length;
	int data_size;

	free_function free_data;

	struct Node* head;
	struct Node* tail;
}List;

typedef struct Node
{
	void* data;

	struct Node* next;
	struct Node* prev;
}Node;


List* get_list(free_function free_data, int data_size);
void free_node(List* list, Node* node);

int append(List* list, void* data);
void prepend(List* list, void* data);
void insert(List* list, void* data, int index);

void* get_at(List* list, unsigned int index);
void pop(List* list, int index, void* data);

Node* get_node_at(List* list, unsigned int index);

void free_list(List* list);

int listTotalChar(List* listOfStr);
char* listStrToStr(List* list);

char* listCharToStr(List* list);