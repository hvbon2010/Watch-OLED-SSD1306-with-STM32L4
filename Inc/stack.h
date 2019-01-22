#ifndef STACK_NODE
#define STACK_NODE

struct StackNode
{
	uint8_t data;
	struct StackNode* next;
};

struct StackNode* newNode(uint8_t data);

int isEmpty(struct StackNode *root);

void push(struct StackNode** root, uint8_t data);

int pop(struct StackNode** root);

int peek(struct StackNode* root);

#endif
