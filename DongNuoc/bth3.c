#include <stdio.h>
#include <stdlib.h>

#define tankcapacity_X 9
#define tankcapacity_Y 4
#define empty 0
#define goal 6
#define MaxLength 100

typedef struct{
	int x;
	int y;
}State;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
}

void print_State(State state){
	printf("\n    X:%d---Y:%d", state.x, state.y);
}

int goalcheck(State state){
	return (state.x == goal || state.y == goal);
}

int max(int x, int y){
	if(x > y) return x;
	return y;
}
int min(int x, int y){
	if(x < y) return x;
	return y;
}

int pourWaterFullX(State cur_state, State *result){
	if(cur_state.x < tankcapacity_X){
		result->x = tankcapacity_X;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterFullY(State cur_state, State *result){
	if(cur_state.y < tankcapacity_Y){
		result->y = tankcapacity_Y;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourWaterEmptyX(State cur_state, State *result){
	if(cur_state.x > 0){
		result->x = empty;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterEmptyY(State cur_state, State *result){
	if(cur_state.y > 0){
		result->y = empty;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

int pourWaterXY(State cur_state, State *result){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), empty);
		result->y = min(cur_state.y + cur_state.x, tankcapacity_Y);
		return 1;
	}
	return 0;
}

int pourWaterYX(State cur_state, State *result){
	if(cur_state.y > 0 && cur_state.x < tankcapacity_X){
		result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), empty);
		result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
		return 1;
	}
	return 0;
}

int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1: return pourWaterFullX(cur_state, result);
		case 2: return pourWaterFullY(cur_state, result);
		case 3: return pourWaterEmptyX(cur_state, result);
		case 4: return pourWaterEmptyY(cur_state, result);
		case 5: return pourWaterXY(cur_state, result);
		case 6: return pourWaterYX(cur_state, result);
		default: printf("Error Calls Operators");
			return 0;
	}
}

const char* action[] = {"First State", "Pour Water Full X", "Pour Water Full Y", "Pour Water Empty X", "Pour Water Empty Y", "Pour Water X to Y", "Pour Water Y to X"};

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

int compareStates(State a, State b){
	return (a.x == b.x && a.y == b.y);
}




typedef struct{
	Node* Elements[MaxLength];
	int front, rear;
}Queue;

void makeNull_Queue(Queue *queue){
	queue->front = -1;
	queue->rear = -1;
}

int empty_Queue(Queue queue){
	return queue.front == -1;
}

int full_Queue(Queue queue){
	return ((queue.rear-queue.front+1)%MaxLength)==0;
}

Node* get_Front(Queue queue){
	if(empty_Queue(queue))
		printf("Queue is empty!");
	else return queue.Elements[queue.front];
}

void del_Queue(Queue *queue){
	if(!empty_Queue(*queue)){
		if(queue->front == queue->rear)
			makeNull_Queue(queue);
		else queue->front = (queue->front+1)%MaxLength;	
	}
	else printf("Error!");
}

void push_Queue(Node* x, Queue *queue){
	if(!full_Queue(*queue)){
		if(empty_Queue(*queue)){
			queue->front = 0;
		}
		queue->rear = (queue->rear+1)%MaxLength;
		queue->Elements[queue->rear] = x;
	}
}

int find_State(State state, Queue open){
	while(!empty_Queue(open)){
		if(compareStates(get_Front(open)->state, state)){
			return 1;
		}
		del_Queue(&open);
	}
	return 0;
}

Node* BFS(State state){
	Queue open;
	Queue close;
	makeNull_Queue(&open);
	makeNull_Queue(&close);
	
	Node* root=(Node*)malloc(sizeof(root));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	push_Queue(root, &open);
	
	while(!empty_Queue(open)){
		Node* node = get_Front(open);
		del_Queue(&open);
		if(goalcheck(node->state)){
			return node;
		}
		int opt;
		for(opt = 1; opt <= 6; opt++){
			State newState;
			makeNullState(&newState);
			if(call_operator(node->state, &newState, opt)){
				if(find_State(newState, open) || find_State(newState, close))
					continue;
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->Parent = node;
				newNode->no_function = opt;
				push_Queue(newNode, &open);
			}
		}
	}
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
	State cur_state = {0, 0};
	print_WaysToGetGoal(BFS(cur_state));
	return 0;
}





