#include <stdio.h>

typedef struct{
    int trangthaily[6]; //0 Up 1 Ngua
}State;
//Operator
int LatLy(int state)
{
    if(state > 1 || state < 0) return -1;
    return state = state == 0 ? 1 : 0;
}
int UpLy(State now,State* result,int start)
{
    int i;
    if(start+3 > 6 || start < 0 ) return 0;
    for(i=0;i<6;i++)
    {
        if(i<start+3 && i>= start) result->trangthaily[i] = LatLy(now.trangthaily[i]);
        else result->trangthaily[i] = now.trangthaily[i];
    }
    return 1;
}
int call_operator(State now,State *result,int option)
{
    switch(option)
    {
        case 0 : return UpLy(now,result,0);
        case 1 : return UpLy(now,result,1);
        case 2 : return UpLy(now,result,2);
        case 3 : return UpLy(now,result,3);
        default : 
        printf("Option Fail ! \n");
        return 0;
    }
}
void show(State result)
{
    int i;
    printf("Trang Thai Cac Ly : \n");
    for(i=0;i<6;i++)
        printf(" %s ",result.trangthaily[i] == 1 ? "Ngua" : "Up");
    printf("\n");
}
const char* action[]= {"Lat Ly thu 1-2-3","Lat Ly thu 2-3-4"
                        ,"Lat Ly thu 3-4-5","Lat Ly thu 4-5-6"};


int main()
{
    State start;
    int i;
    for(i=0;i<6;i++) start.trangthaily[i] = i%2?1:0;
    show(start);
    return 0;
}
