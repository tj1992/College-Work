// Program for sorting a linked list using Quicksort
#include <stdio.h>
#include <stdlib.h>

typedef struct node__ {
	int data;
	struct node__* next;
} Node;

Node* create_node (int data) {
	Node* temp = NULL;
	if ((temp = malloc (sizeof (Node))) == NULL) {
		fprintf (stderr, "Node creation failed for data: %d\n", data);
		return NULL;
	}
	temp->data = data;
	temp->next = NULL;
	return temp;
}

Node* append_node (Node* front, Node* node) {
	Node* temp = front;
	if (front == NULL)
		front = node;
	else {
		while (temp->next != NULL)
			temp = temp->next;

		temp->next = node;
	}
	return front;
}

void swap_data (Node* i, Node* j) {
	int temp = i->data;
	i->data = j->data;
	j->data = temp;
}

void print_list (Node* front, Node* end) {
	if (front == NULL)
		return;
	Node* i;
	for (i = front; i != end->next; i = i->next)
		printf ("%d ", i->data);
	putchar ('\n');
}

void quick_sort (Node* front, Node* end) {
	if (front == NULL || end == NULL || front == end->next)
		return;
	Node* i, *pivot, *last, *prev;
	pivot = front;
	last = front;
	prev = NULL;
	for (i = front->next; i != end->next; i = i->next) {
		if (i->data < pivot->data) {
			prev = last;
			last = last->next;
			swap_data (i, last);
		}
	}
	swap_data (pivot, last);
	quick_sort (front, prev);
	quick_sort (last->next, end);
}

int main () {
	Node* front = NULL;
	Node* end;
	Node* i;
	int data;

	while (scanf ("%d", &data) > 0) {
		end = create_node (data);
		front = append_node (front, end);
	}
	//print_list (front, end);
	quick_sort (front, end);
	print_list (front, end);

	return 0;
}
