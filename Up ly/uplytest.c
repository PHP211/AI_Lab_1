#include <stdio.h>
#include <stdlib.h>

#define Maxlength 100

typedef struct
{
    int state_ly[6]; // 0 up, 1 ngua
} State;

typedef struct Node
{
    State state;
    struct Node *Parent;
    int no_function;
} Node;

typedef struct
{
    Node *Element[Maxlength];
    int Top_idx;
} Stack;

void makeNullState(State *state)
{
    for (int i = 1; i <= 6; i++)
    {
        if (i % 2 == 0)
            state->state_ly[i] = 0;
        else
            state->state_ly[i] = 1;
    }
}

void makeNull_Stack(Stack *stack)
{
    stack->Top_idx = Maxlength;
}

int empty_Stack(Stack stack)
{
    return stack.Top_idx == Maxlength;
}

int full_Stack(Stack stack)
{
    return stack.Top_idx == 0;
}

Node *top(Stack stack)
{
    if (!empty_Stack(stack))
        return stack.Element[stack.Top_idx];
    return NULL;
}

void pop(Stack *stack)
{
    if (!empty_Stack(*stack))
        stack->Top_idx += 1;
    else
        printf("Error! Stack is empty!");
}

void push(Node *x, Stack *stack)
{
    if (full_Stack(*stack))
        printf("Error! Stack is full!");
    else
    {
        stack->Top_idx -= 1;
        stack->Element[stack->Top_idx] = x;
    }
}

int compareStates(State A, State B)
{
    for (int i = 1; i <= 6; i++)
        if (A.state_ly[i] != B.state_ly[i])
            return 0;
    return 1;
}

int find_State(State state, Stack openStack)
{
    while (!empty_Stack(openStack))
    {
        if (compareStates(top(openStack)->state, state))
            return 1;
        pop(&openStack);
    }
    return 0;
}

int goalcheck(State state)
{
    for (int i = 1; i <= 6; i++)
        if (state.state_ly[i] == 1)
            return 0;
    return 1;
}

int Lat(int state)
{
    if (state > 1 || state < 0)
        return -1;
    if (state == 0)
        return state = 1;
    else
        return state = 0;
}

int Up(State cur_state, State *result, int start)
{
    int i;
    if (start + 3 > 6 || start < 0)
        return 0;
    for (i = 1; i <= 6; i++)
    {
        if (i < start + 3 && i >= start)
            result->state_ly[i] = Lat(cur_state.state_ly[i]);
        else
            result->state_ly[i] = cur_state.state_ly[i];
    }
    return 1;
}

int call_operators(State cur_state, State *result, int option)
{
    switch (option)
    {
    case 1:
        return Up(cur_state, result, 1);
    case 2:
        return Up(cur_state, result, 2);
    case 3:
        return Up(cur_state, result, 3);
    case 4:
        return Up(cur_state, result, 4);
    default:
        printf("Error Operators");
    }
    return 0;
}

Node *DFS_Algorithm(State state)
{
    Stack Open_DFS;
    Stack Close_DFS;
    makeNull_Stack(&Open_DFS);
    makeNull_Stack(&Close_DFS);
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    push(root, &Open_DFS);
    while (!empty_Stack(Open_DFS))
    {
        Node *node = top(Open_DFS);
        pop(&Open_DFS);
        push(node, &Close_DFS);
        if (goalcheck(node->state))
            return node;
        int opt;
        for (opt = 1; opt <= 4; opt++)
        {
            State newState;
            makeNullState(&newState);
            if (call_operators(node->state, &newState, opt))
            {
                if (find_State(newState, Close_DFS) || find_State(newState, Open_DFS))
                    continue;
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->Parent = node;
                newNode->no_function = opt;
                push(newNode, &Open_DFS);
            }
        }
    }
    return NULL;
}

const char *action[] = {"First state", "Lat ly 1-2-3", "Lat ly 2-3-4", "Lat ly 3-4-5", "Lat ly 4-5-6"};

void print_State(State state)
{
    for (int i = 1; i <= 6; i++)
    {
        if (state.state_ly[i] == 1)
            printf("Ngua   ");
        else
            printf("Up   ");
    }
    printf("\n");
}

void print_WaysToGetGoal(Node *node)
{
    Stack stackPrint;
    makeNull_Stack(&stackPrint);
    while (node->Parent != NULL)
    {
        push(node, &stackPrint);
        node = node->Parent;
    }
    push(node, &stackPrint);
    int no_action = 0;
    while (!empty_Stack(stackPrint))
    {
        printf("\nAction %d: %s\n", no_action, action[top(stackPrint)->no_function]);
        print_State(top(stackPrint)->state);
        pop(&stackPrint);
        no_action++;
    }
}

int main()
{
    State cur_state;
    makeNullState(&cur_state);
    Node *p = DFS_Algorithm(cur_state);
    print_WaysToGetGoal(p);
    return 0;
}
