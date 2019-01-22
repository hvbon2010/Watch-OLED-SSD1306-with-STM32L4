#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.h"

struct StackNode *newNode(uint8_t data)
{
	struct StackNode *stackNode =  
		(struct StackNode*) malloc(sizeof(struct StackNode));
	stackNode->data = data;
	stackNode->next = NULL;
	return stackNode;
}

int isEmpty(struct StackNode *root)
{
	return !root;
}

void push(struct StackNode **root, uint8_t data)
{
	struct StackNode *stackNode = newNode(data);
	stackNode->next = *root;
	*root = stackNode;
	printf("%d pushed to stack\r\n", data);
}

int pop(struct StackNode **root)
{
	if(isEmpty(*root))
		return INT_MIN;
	struct StackNode *temp = *root;
	*root = (*root)->next;
	uint8_t popped = temp->data;
	free(temp);
	
	return popped;
}

int peek(struct StackNode* root)
{
	if(isEmpty(root))
		return INT_MIN;
	return root->data;
}

