#include <stdio.h>
#include <stdlib.h>
#include<stack>

using namespace std;

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
	printf("\n(%d,%d,%c)\n", state.tusi, state.quy, state.pos);
}

int compareStates(State a, State b){
	return a.pos == b.pos && a.quy == b.quy && a.tusi == b.tusi;
}

int findState(State state, stack<Node*> openStack){
	while(!openStack.empty()){
		if(compareStates(openStack.top()->state, state)){
			return 1;
		}
		openStack.pop();
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
	stack<Node*> open;
	stack<Node*> close;
	
	Node* root = (Node*)malloc(sizeof(Node));
	root->parent = NULL;
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
		for(opt = 1; opt <= 5; opt++){
			State newstate;
			if(calloperators(node->state, &newstate, opt)){
				if(findState(newstate, open) || findState(newstate, close) || !alive(newstate))
					continue;
				
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->parent = node;
				newNode->no_function = opt;
				newNode->state = newstate;
				open.push(newNode); 
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	stack<Node*> stackPrint;
	while(node->parent != NULL){
		stackPrint.push(node);
		node = node->parent;
	}
	stackPrint.push(node);
	
	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\nAction %d: %s", no_action, action[stackPrint.top()->no_function]);
		print_State(stackPrint.top()->state);
		stackPrint.pop();
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
