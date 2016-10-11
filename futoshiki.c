#include <stdio.h>

#include <stdlib.h>


typedef struct BOARD_ {
    int size, restrictionsNum;
    int **values, **restrictions;
} BOARD;


BOARD *inicializeBoard(int size, int restrictionsNum){
    int i;
    BOARD *board = (BOARD*) malloc (sizeof(BOARD));
    board->size = size;
    board->restrictionsNum = restrictionsNum;
    
    board->values = (int**) malloc (sizeof(int*)*size);
    for (i=0; i<size; i++){
        board->values[i] = (int*) malloc (sizeof(int)*size);
    }    
    
    board->restrictions = (int**) malloc (sizeof(int*)*restrictionsNum);
    for (i=0; i<restrictionsNum; i++){
        board->restrictions[i] = (int*) malloc (sizeof(int)*4);
    }
    
    return board;
}

BOARD **getInputs (int *inputSize){
    int i, x, y, size, restrictionsNum;
    
    scanf ("%d", inputSize);
    
    BOARD **inputs = (BOARD**) malloc (sizeof(BOARD*) * (*inputSize));
    
    for (i=0; i<*inputSize; i++){
        
        scanf ("%d %d", &size, &restrictionsNum);
        
        inputs[i] = inicializeBoard(size, restrictionsNum);

        for (x=0; x<inputs[i]->size; x++){
            for (y=0; y<inputs[i]->size; y++){
                scanf ("%d", &(inputs[i]->values[x][y]));
            }
        }
        
        for (x=0; x<inputs[i]->restrictionsNum; x++){
            for (y=0; y<4; y++){
                scanf ("%d", &(inputs[i]->restrictions[x][y]));
            }
        }
    }
    
    return inputs;
}

//Returns 1 if that value could be in tha position x,y. Returns 0 otherwise
int verifPosition (BOARD *board, int x, int y){
    
    int i, j;
    
    for (i=0; i<board->size; i++){
        if (i!=x){
            if (board->values[i][y] == board->values[x][y]){
                return 0;
            }
        }
        if (i!=y){
            if (board->values[x][i] == board->values[x][y]){
                return 0;
            }
        }
    }
    
    for (i=0; i<board->restrictionsNum; i++){
        if (board->restrictions[i][0] == x && board->restrictions[i][1] == y){
            int posX = board->restrictions[i][2], posY = board->restrictions[i][3];

            if (board->values[posX][posY] !=0 && board->values[x][y] >= board->values[posX][posY]){
                return 0;
            }
        } else if (board->restrictions[i][2] == x && board->restrictions[i][3] == y){
            int posX = board->restrictions[i][0], posY = board->restrictions[i][1];

            if (board->values[posX][posY] !=0 && board->values[x][y] <= board->values[posX][posY]){
                return 0;
            }
        }
    }
    return 1;
}

void simpleBacktrack(BOARD *board, int x, int y){
    
    if (y >= board->size) return; //End of the board
   
    if (board->values[x][y] != 0){
        if (x >= board->size){
            simpleBacktrack(board, 0, y+1);
        } else {
            simpleBacktrack(board, x+1, y);
        }
        return;
    }
    
    do{ //Tries values until finds one
        board->values[x][y]++;
        verifPosition (board, x, y);
        
    } while (irregular);
        
        
    board->values[x][y]
}

int main(){
    BOARD **inputs;
    int i, inputSize, heuristic;
    
    inputs = getInputs (&inputSize);
    
    do{
        printf ("Select a option:\n1: Simple Backtracking\n2: Backtracking with Forward Checking\n3: Backtracking with Forward Checking and MVR\n");
        scanf ("%d", &heuristic);
        for (i=0; i<inputSize; i++){
            switch (heuristic){
                case 1: simpleBacktrack(inputs[i], 0, 0); break;
                //case 2: forwardCheckBacktrack(inputs[i]); break;
                //case 3: mvrForwardCheckBacktrack(inputs[i]); break;
                default: printf("Select one of the options 1,2 or 3\n");
            }
        }
    } while (heuristic > 3 || heuristic < 1);
}