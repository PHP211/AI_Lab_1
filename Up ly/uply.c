#include <stdio.h>
#include <stdlib.h>

#define MaxLength 100

typedef struct{
	int ly[6];
}State;

void initState(State *state){
	int i;
	for(i = 0; i <= 5; i++){
		if(i % 2 == 0){
			state->ly[i] = 1;
		}
		else state->ly[i] = 0;
	}
}

int goalcheck(State state){
	int i;
	for(i = 0; i <= 6; i++){
		if(state.ly[i] == 0){
			return 0;
		}
	}
	return 1;
}

void print_State(State state){
	int i;
	for(i = 0; i <= 5; i++){
		if(state.ly[i] == 1) printf("Ngua\t");
		else printf("Up\t");		
	}
	printf("\n");
}

State latly(State *state, int pos){
	if(state->ly[pos] == 1) state->ly[pos] = 0;
	else state->ly[pos] = 1;
	return *state;
}

int robot(State cur, State *result, int start){
	*result = cur;
	if(start+3 > 6 || start < 0) return 0;
	int i;
	for(i = start; i <= start+2; i++){
		*result = latly(result, i);
	}
	return 1;
}

int call_operator(State cur, State *result, int option){
	switch(option){
		case 1: return robot(cur, result, 0);
		case 2: return robot(cur, result, 1);
		case 3: return robot(cur, result, 2);
		case 4: return robot(cur, result, 3);
		default: printf("Call operators error!");
			return 0;
	}
}

const char *action[] = {"First state", "Lat ly 1-2-3", "Lat ly 2-3-4", "Lat ly 3-4-5", "Lat ly 4-5-6"};

typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;

typedef struct{
	Node* Elements[MaxLength];
	int top_idx;
}Stack;

void makeNull_Stack(Stack *stack){
	stack->top_idx = MaxLength;
}

int empty_Stack(Stack stack){
	return stack.top_idx == MaxLength;
}

int full_Stack(Stack stack){
	return stack.top_idx == 0;
}

void push(Node* x, Stack* stack){
	if(full_Stack(*stack)){
		printf("Error! Stack is Full");
	}
	else {
		stack->top_idx -= 1;
		stack->Elements[stack->top_idx] = x;
	}
}

Node* top(Stack stack){
	if(!empty_Stack(stack)){
		return stack.Elements[stack.top_idx];
	}
	return NULL;
}

void pop(Stack *stack){
	if(!empty_Stack(*stack)){
		stack->top_idx += 1;
	}
	else printf("Error! Stack is empty");
}

int compareState(State a, State b){
	int i;
	for(i = 0; i <= 5; i++)
		if(a.ly[i] != b.ly[i])
			return 0;
	return 1;
}

int find_State(State a, Stack stack){
	while(!empty_Stack(stack)){
		if(compareState(a, top(stack)->state)){
			return 1;
		}
		pop(&stack);
	}
	return 0;
}

Node* DFS(State state){
	//khai bao stack open va close
	Stack open;
	Stack close;
	makeNull_Stack(&open);
	makeNull_Stack(&close);
	//tao nut trang thai cha
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	push(root, &open);
	while(!empty_Stack(open)){
		Node* node = top(open);
		pop(&open);
		push(node, &close);
		if(goalcheck(node->state)){
			return node;
		}
		int opt;
		for(opt = 1; opt <= 4; opt++){
			State newstate;
			initState(&newstate);
			if(call_operator(node->state, &newstate, opt)){
				if(find_State(newstate, open) || find_State(newstate, close))
					continue;
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = opt;
				push(newNode, &open);
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	Stack stackPrint;
	makeNull_Stack(&stackPrint);
	while(node->Parent != NULL){
		push(node, &stackPrint);
		node = node->Parent;
	}
	push(node, &stackPrint);
	int no_action = 0;
	while(!empty_Stack(stackPrint)){
		printf("\nAction %d: %s\n", no_action, action[top(stackPrint)->no_function]);
		print_State(top(stackPrint)->state);
		pop(&stackPrint);
		no_action++;
	}
}

int main(){
	State cur, result;
	initState(&cur);
	Node *p = DFS(cur);
	print_WaysToGetGoal(p);
	return 0;
}
