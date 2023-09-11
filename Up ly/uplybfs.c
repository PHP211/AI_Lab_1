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
		if(compareState(get_Front(open)->state, state)){
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
		for(opt = 1; opt <= 4; opt++){
			State newState;
			initState(&newState);
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
	Node *p = BFS(cur);
	print_WaysToGetGoal(p);
	return 0;
}
