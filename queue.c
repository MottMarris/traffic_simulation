#include <queue.h>

void queue(NODE **tail, int val)
{
	NODE *new_node;
	if (!(new_node = malloc(sizeof(NODE))))
	{
		fprintf(stderr, "No memory.\n");
		exit(1);
	}

	new_node->value = val;

	if (*tail == NULL)
		new_node->nextNode = NULL;
	else
		new_node->nextNode = *tail;
	*tail = new_node;
}

int dequeue(NODE **tail)
{
	NODE *current, *prev = NULL;
	int tail_value;

	if (*tail == NULL) return -1;

	current = *tail;

	while (current->nextNode != NULL) {
		prev = current;
		current = current->nextNode;
	}

	tail_value = current->value;
	free(current);
	if(prev)
		prev->nextNode = NULL;
	else
      *tail = NULL;
	return tail_value;
}
