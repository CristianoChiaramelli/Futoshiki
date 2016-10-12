#include <stdio.h>
#include <stdlib.h>

typedef struct BOARD_ {
    int size, restrictionsNum;
    int **values, **restrictions;
} BOARD;

void printBoard(BOARD *board){
	int i, j;

	for (i=0; i<board->size; i++){
		for (j=0; j<board->size; j++){
			printf ("%d ", board->values[j][i]);
		}
		printf ("\n");
	}
}

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
    printf ("Checking (%d,%d)\n", x, y);
    if (y >= board->size) {
    	printf ("End of the board. y>= board->size\n");
    	return; //End of the board
    }
   	
    if (board->values[x][y] != 0){
    	printf ("This position has a fixed value\n");
        if (x >= board->size-1){
            simpleBacktrack(board, 0, y+1);
        } else {
            simpleBacktrack(board, x+1, y);
        }
        return;
    }
    
    int i, irregular;
    for (i=0; i<board->size; i++){ //Tries values until finds one
        board->values[x][y]++;
        printf ("Trying value %d\n", board->values[x][y]);
        irregular = verifPosition (board, x, y);

        //This value is OK.
        if (irregular == 1){
        	printf("This value is OK\n");
	        if (x >= board->size-1){
	            simpleBacktrack(board, 0, y+1);
	        } else {
	            simpleBacktrack(board, x+1, y);
	        }
	        return;
        }
        printf ("This value is IRREGULAR\n");
    }
    
    //If code gets here, this position can't have any value.
    //Then it goes back, and tries changing other positions before
    printf ("This position cant have any value. Lets go back\n");
    if (x <= 0){
    	if (y <= 0){
    		//NO SOLUTION??
    	} else {
        	simpleBacktrack(board, 0, y-1);
    	}
    } else {
        simpleBacktrack(board, x-1, y);
    }
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

        for (i=0; i<inputSize; i++){
        	printf ("%d\n", i+1);
            printBoard(inputs[i]);
        }
    } while (heuristic > 3 || heuristic < 1);
}