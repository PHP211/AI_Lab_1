#include <stdio.h>
#include <stdlib.h>

#define MaxLength 100

typedef struct{
	int tusi; //so luong tu si
	int quy;  //so luong thuyen
	char pos; //vi tri thuyen: 'A' hoac 'B'
}State;

typedef struct Node{
	struct Node* parent;
	State state;
	int no_function;
}Node;

void print_State(State state){
	printf("(%d,%d,%c)\n", state.tusi, state.quy, state.pos);
}

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
		printf("Error! Stack is Full\n");
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
	return a.pos == b.pos && a.quy == b.quy && a.tusi == b.tusi;
}

int findState(State state, Stack stack){
	while(!empty_Stack(stack)){
		if(compareState(state, top(stack)->state)){
			return 1;
		}
		pop(&stack);
	}
	return 0;
}

int alive(State state){
	if(state.tusi < state.quy && state.tusi != 0) return 0;
	if(3-state.tusi < 3-state.quy && 3-state.tusi != 0) return 0;
	return 1;
}

int goalcheck(State state){
	return state.tusi == 0 && state.quy == 0;
}

//OPERATORS
int tusi1(State cur, State *result){
	int i;
    if ((cur.pos == 'A') ? (cur.tusi >= 1) : (cur.tusi <= 2))
    {
        int i = cur.pos == 'A' ? -1 : 1;
        result->tusi = cur.tusi + i;
        result->quy = cur.quy;
        result->pos = cur.pos == 'A' ? 'B' : 'A';
        return 1;
    }
    return 0;
}

int quy1(State cur, State *result){
	int i;
    if ((cur.pos == 'A') ? (cur.quy >= 1) : (cur.quy <= 2))
    {
        int i = cur.pos == 'A' ? -1 : 1;
        result->tusi = cur.tusi;
        result->quy = cur.quy + i;
        result->pos = cur.pos == 'A' ? 'B' : 'A';
        return 1;
    }
    return 0;
}

int tusi2(State cur, State *result){
	int i;
    if ((cur.pos == 'A') ? (cur.tusi >= 2) : (cur.tusi <= 1))
    {
        int i = cur.pos == 'A' ? -1 : 1;
        result->tusi = cur.tusi + i*2;
        result->quy = cur.quy;
        result->pos = cur.pos == 'A' ? 'B' : 'A';
        return 1;
    }
    return 0;
}

int quy2(State cur, State *result){
	int i;
    if ((cur.pos == 'A') ? (cur.quy >= 2) : (cur.quy <= 1))
    {
        int i = cur.pos == 'A' ? -1 : 1;
        result->tusi = cur.tusi;
        result->quy = cur.quy + i*2;
        result->pos = cur.pos == 'A' ? 'B' : 'A';
        return 1;
    }
    return 0;
}

int mixed(State cur, State *result){
	int i;
    if ((cur.pos == 'A') ? (cur.quy >= 1 && cur.tusi >= 1) : (cur.quy <= 2 && cur.tusi <= 2))
    {
        int i = cur.pos == 'A' ? -1 : 1;
        result->tusi = cur.tusi + i;
        result->quy = cur.quy + i;
        result->pos = cur.pos == 'A' ? 'B' : 'A';
        return 1;
    }
    return 0;
}

int calloperators(State cur, State *result, int option){
	switch(option){
		case 1: return tusi1(cur, result);
		case 2: return quy1(cur, result);
		case 3: return tusi2(cur, result);
		case 4: return quy2(cur, result);
		case 5: return mixed(cur, result);
		default:
			printf("Error!");
			return 0;
	}
}

const char *action[] = {"First state", "Chuyen 1 tu si", "Chuyen 1 quy", 
					"Chuyen 2 tu si", "Chuyen 2 quy", "Chuyen 1 tu si 1 quy"};	
					
Node* DFS(State state){
	Stack open, close;
	makeNull_Stack(&open);
	makeNull_Stack(&close);
	
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_function = 0;
	push(root, &open);
	
	while(!empty_Stack(open)){
		Node* node = top(open);
		pop(&open);
		push(node, &close);
		if(goalcheck(node->state))
			return node;
		int opt;
		for(opt = 1; opt <= 5; opt++){
			State newstate;
			if(calloperators(node->state, &newstate, opt)){
				if((findState(newstate, open) || findState(newstate, close)) || !alive(newstate)){
					continue;
				}
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->no_function = opt;
				newNode->parent = node;
				push(newNode, &open);
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	Stack stackPrint;
	makeNull_Stack(&stackPrint);
	while(node->parent != NULL){
		push(node, &stackPrint);
		node = node->parent;
	}
	push(node, &stackPrint);
	int no_action = 0;
	while(!empty_Stack(stackPrint)){
		printf("Action %d: %s\n", no_action, action[top(stackPrint)->no_function]);
		print_State(top(stackPrint)->state);
		pop(&stackPrint);
		no_action++;
	}
}

int main(){
	State cur = {3,3,'A'};
	State result = {0,0,'A'};
//	int i;
//	for(i = 1; i <= 5; i++){
//		if(calloperators(cur, &result, i)){
//			print_State(result);
//		}
//	}
	Node *p = DFS(cur);
    print_WaysToGetGoal(p);
	return 0;
}
