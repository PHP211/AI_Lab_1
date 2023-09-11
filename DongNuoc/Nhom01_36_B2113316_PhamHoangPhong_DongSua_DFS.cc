#include<stdio.h>
#include<stdlib.h>
#include<stack>

using namespace std;

#define tankcapacity_X 10
#define tankcapacity_Y 5
#define tankcapacity_Z 6
#define goal 8
#define MaxLength 100

typedef struct{
	int x;
	int y;
	int z;
}State;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
	state->z = 0;
}

void print_State(State state){
	printf("\n    X:%d   Y:%d   Z:%d", state.x, state.y, state.z);
}

int goalcheck(State state){
	return (state.x == goal);
}

int max(int x, int y){
	if(x > y) return x;
	return y;
}
int min(int x, int y){
	if(x < y) return x;
	return y;
}

int pourWaterXY(State cur_state, State *result){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), 0);
		result->y = min(cur_state.y + cur_state.x, tankcapacity_Y);
		result->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterXZ(State cur_state, State *result){
	if(cur_state.x > 0 && cur_state.z < tankcapacity_Z){
		result->x = max(cur_state.x - (tankcapacity_Z - cur_state.z), 0);
		result->z = min(cur_state.z + cur_state.x, tankcapacity_Z);
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterYX(State cur_state, State *result){
	if(cur_state.y > 0 && cur_state.x < tankcapacity_X){
		result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), 0);
		result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
		result->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterYZ(State cur_state, State *result){
	if(cur_state.y > 0 && cur_state.z < tankcapacity_Z){
		result->y = max(cur_state.y - (tankcapacity_Z - cur_state.z), 0);
		result->z = min(cur_state.y + cur_state.z, tankcapacity_Z);
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourWaterZY(State cur_state, State *result){
	if(cur_state.z > 0 && cur_state.y < tankcapacity_Y){
		result->z = max(cur_state.z - (tankcapacity_Y - cur_state.y), 0);
		result->y = min(cur_state.z + cur_state.y, tankcapacity_Y);
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourWaterZX(State cur_state, State *result){
	if(cur_state.z > 0 && cur_state.x < tankcapacity_X){
		result->z = max(cur_state.z - (tankcapacity_X - cur_state.x), 0);
		result->x = min(cur_state.z + cur_state.x, tankcapacity_X);
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1: return pourWaterXY(cur_state, result);
		case 2: return pourWaterXZ(cur_state, result);
		case 3: return pourWaterYX(cur_state, result);
		case 4: return pourWaterYZ(cur_state, result);
		case 5: return pourWaterZX(cur_state, result);
		case 6: return pourWaterZY(cur_state, result);
		default: printf("Error Calls Operators");
			return 0;
	}
}

const char* action[] = {"First State","Pour Water X to Y", "Pour Water X to Z", "Pour Water Y to X", 
						"Pour Water Y to Z", "Pour Water Z to X", "Pour Water Z to Y"};

typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;


int compareStates(State a, State b){
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

int find_State(State state, stack<Node*> openStack){
	while(!openStack.empty()){
		if(compareStates(openStack.top()->state, state)){
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
		for(opt = 1; opt <= 6; opt++){
			State newstate;
			makeNullState(&newstate);
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
		printf("\nAction %d: %s", no_action, action[stackPrint.top()->no_function]);
		print_State(stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}

int main(){
	State cur_state = {10, 0, 0};
	Node* p = DFS(cur_state);
	print_Ways(p);
	return 0;
}