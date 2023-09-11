#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int tusi;
	int quy;
	char thuyen;
}State;

//void init_State(State *state){
//	state->tusi = 3;
//	state->quy = 3;
//	state->thuyen = 'A';
//}

int tusi1(State cur, State *result){
	if(cur.tusi != 0){
		int i;
		if(cur.thuyen == 'A') i = -1;
		else i = 1;
		result->tusi = cur.tusi + i;
		result->quy = cur.quy;
		if(cur.thuyen == 'A') result->thuyen = 'B';
		else result->thuyen = 'A';
		return 1;
	}
	return 0;
}

int quy1(State cur, State *result){
	if(cur.quy != 0){
		int i;
		if(cur.thuyen == 'A') i = -1;
		else i = 1;
		result->tusi = cur.tusi;
		result->quy = cur.quy + i;
		if(cur.thuyen == 'A') result->thuyen = 'B';
		else result->thuyen = 'A';
		return 1;
	}
	return 0;
}

int tusi2(State cur, State *result){
	if(cur.tusi != 0){
		int i;
		if(cur.thuyen == 'A') i = -1;
		else i = 1;
		result->tusi = cur.tusi + i*2;
		result->quy = cur.quy;
		if(cur.thuyen == 'A') result->thuyen = 'B';
		else result->thuyen = 'A';
		return 1;
	}
	return 0;
}

int quy2(State cur, State *result){
	if(cur.quy != 0){
		int i;
		if(cur.thuyen == 'A') i = -1;
		else i = 1;
		result->tusi = cur.tusi;
		result->quy = cur.quy + i*2;
		if(cur.thuyen == 'A') result->thuyen = 'B';
		else result->thuyen = 'A';
		return 1;
	}
	return 0;
}

int mixed(State cur, State *result){
	if(cur.quy != 0 && cur.tusi != 0){
		int i;
		if(cur.thuyen == 'A') i = -1;
		else i = 1;
		result->tusi = cur.tusi + i;
		result->quy = cur.quy + i;
		if(cur.thuyen == 'A') result->thuyen = 'B';
		else result->thuyen = 'A';
		return 1;
	}
	return 0;
}

int call_operator(State cur, State *result, int option){
	switch(option){
		case 1: return tusi1(cur, result);
		case 2: return quy1(cur, result);
		case 3: return tusi2(cur, result);
		case 4: return quy2(cur, result);
		case 5: return mixed(cur, result);
		default: printf("Error!");
			return 0;
 	}
}

const char *action[] = {"First state", "Chuyen 1 tu si", "Chuyen 1 quy", 
					"Chuyen 2 tu si", "Chuyen 2 quy", "Chuyen 1 tu si 1 quy"};

void print_State(State state){
	printf("\n(%d,%d,%c)", state.tusi, state.quy, state.thuyen);
}

int aliveState(State state){
	if(state.quy != state.tusi && state.tusi != 0) return 0;
	return 1;
}

int goalcheck(State state){
	return state.quy == 0 && state.tusi == 0;
}

typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;

typedef struct{
	Node* Elements[100];
	int top_idx;
}Stack;


void makeNull_Stack(Stack *stack){
	stack->top_idx = 100;
}

int empty_Stack(Stack stack){
	return stack.top_idx == 100;
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

int compareStates(State a, State b){
	return a.quy == b.quy && a.tusi == b.tusi && a.thuyen == b.thuyen;
}

int find_State(State state, Stack openStack){
	while(!empty_Stack(openStack)){
		if(compareStates(top(openStack)->state, state)){
			return 1;
		}
		pop(&openStack);
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
		print_State(node->state);
		printf("")
//		push(node, &close);
		if(goalcheck(node->state)){
			return node;
		}
		int opt;
		for(opt = 1; opt <= 5; opt++){
			State newstate;
			if(call_operator(node->state, &newstate, opt)){
				if(find_State(newstate, open) || find_State(newstate, close))
					if(!aliveState(newstate))
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
		printf("\nAction %d: %s", no_action, action[top(stackPrint)->no_function]);
		print_State(top(stackPrint)->state);
		pop(&stackPrint);
		no_action++;
	}
}

int main(){
	State cur;
	cur.quy = 3;
	cur.tusi = 3;
	cur.thuyen = 'A';
	Node* p = DFS(cur);
	print_WaysToGetGoal(p);
	return 0;
}
