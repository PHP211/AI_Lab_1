#include <stdio.h>
#include <stdlib.h>
#include<stack>

using namespace std;

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

int compareState(State a, State b){
	int i;
	for(i = 0; i <= 5; i++)
		if(a.ly[i] != b.ly[i])
			return 0;
	return 1;
}

int find_State(State state, stack<Node*> openStack){
	while(!openStack.empty()){
		if(compareState(openStack.top()->state, state)){
			return 1;
		}
		openStack.pop();
	}
	return 0;
}

Node* DFS(State state){
	stack<Node*> open;
	stack<Node*> close;
	
	Node* root = (Node*)malloc(sizeof(Node));
	root->Parent = NULL;
	root->no_function = 0;
	root->state = state;
	open.push(root);
	
	while(!open.empty()){
		Node* node= open.top();
		open.pop();
		close.push(node);
		
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
				
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->Parent = node;
				newNode->no_function = opt;
				newNode->state = newstate;
				open.push(newNode); 
			}
		}
	}
	return NULL;
}

void print_Ways(Node* node){
	stack<Node*> stackPrint;
	while(node->Parent != NULL){
		stackPrint.push(node);
		node = node->Parent;
	}
	stackPrint.push(node);
	
	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\nAction %d: %s\n", no_action, action[stackPrint.top()->no_function]);
		print_State(stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}


int main(){
	State cur, result;
	initState(&cur);
	Node *p = DFS(cur);
	print_Ways(p);
	return 0;
}
