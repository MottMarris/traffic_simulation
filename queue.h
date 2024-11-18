#include <stdio.h>
#include <stdlib.h>

struct queueNode
{
	int value;
	struct queueNode *nextNode;
};
typedef struct queueNode NODE;

void queue(NODE **tail, int val);
int dequeue(NODE **tail);
