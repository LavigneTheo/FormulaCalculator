#include "List.h"



int append(List* list, void* data) {
	Node* tmp = malloc(sizeof(Node));

	if (list->length == 0) {
		list->head = list->tail = tmp;
	}
	else {
		tmp->prev = list->tail;
		list->tail->next = tmp;
		tmp->next = NULL;
		list->tail = tmp;
	}
	tmp->data = malloc(list->data_size);
	memcpy(tmp->data, data, list->data_size);

	list->length++;
	return list->length - 1;
}

void prepend(List* list, void* data) {
	Node* tmp = malloc(sizeof(Node));

	if (list->length == 0) {
		list->head = list->tail = tmp;
	}
	else {
		tmp->next = list->head;
		list->head->prev = tmp;
		tmp->prev = NULL;
		list->head = tmp;
	}
	tmp->data = malloc(list->data_size);
	tmp->data = memcpy(tmp->data, data, list->data_size);

	list->length++;
}

void insert(List* list, void* data, int index) {
	Node* tmp = malloc(sizeof(Node));

	if (index == 0) {
		prepend(list, data);
	}
	else if (index == list->length) {
		append(list, data);
	}
	else {
		Node* n = get_node_at(list, index);
		tmp->next = n;
		tmp->prev = n->prev;
		n->prev = tmp;
		tmp->prev->next = tmp;

		tmp->data = malloc(list->data_size);
		tmp->data = memcpy(tmp->data, data, list->data_size);
		list->length++;
	}
}

List* get_list(free_function free_data, int data_size) {
	List* list = malloc(sizeof(List));
	list->data_size = data_size;
	list->free_data = free_data;
	list->length = 0;

	return list;
}

Node* get_node_at(List* list, unsigned int index) {
	if (list->length == 0 || index >= list->length)
		return NULL;
	Node* n;
	if ((int)list->length / 2 > index) {
		n = list->head;
		for (int i = 0; i < index; i++)
			n = n->next;
	}
	else {
		n = list->tail;
		for (int i = 0; i < list->length - 1 - index; i++)
			n = n->prev;
	}

	return n;
}

void* get_at(List* list, unsigned int index) {
	Node* n = get_node_at(list, index);
	return (n == NULL) ? NULL : n->data;
}

void free_node(List* list, Node* node) {
	(list->free_data == NULL) ? free(node->data) : list->free_data(node);
	free(node);

	list->length--;
}

void free_list(List* list) {
	Node* n = list->head;
	int liste_size = list->length;
	for (int i = 0; i < liste_size - 1; i++) {
		n = n->next;
		free_node(list, n->prev);
	}
	if (list->length != 0)
		free_node(list, list->tail);
	free(list);
}

void pop(List* list, int index, void* data) {
	if (list->length == 0)
		return NULL;

	Node* n;
	if (list->length == 1) {
		n = list->head;
	}
	else if (index == 0) {
		n = list->head;
		list->head = n->next;
		list->head->prev = NULL;
	}
	else if (index == list->length - 1) {
		n = list->tail;
		list->tail = n->prev;
		list->tail->next = NULL;
	}
	else {
		n = get_node_at(list, index);
		n->prev->next = n->next;
		n->next->prev = n->prev;
	}

	memcpy(data, n->data, list->data_size);

	free_node(list, n);
}

int listTotalChar(List* listOfStr) {
	int sum = 0;
	for (int i = 0; i < listOfStr->length; i++) {
		char** str = get_at(listOfStr, i);
		sum += strlen(*str);
	}
	return sum;
}

char* listStrToStr(List* list) {
	int totalChar = listTotalChar(list);
	char* str = malloc(sizeof(char) * (1 + totalChar));
	*str = '\0';
	for (int i = 0; i < list->length; i++)
		strcat(str, *(char**)get_at(list, i));
	return str;
}
