#include <stdio.h>
#include <stdlib.h>
	
#define LTR 1		// 1 - menor que o da direita
#define GTL	2		// 2 - maior que o da esquerda
#define LTL 4		// 4 - menor que o da esquerda
#define GTR	8		// 8 - maior que o da direita
#define LTD 16		// 16 - menor que o de baixo
#define GTU 32		// 32 - maior que o de cima
#define LTU 64		// 64 - menor que o de cima
#define GTD	128		// 128 - maior que o de baixo
int counter = 0; 		
typedef struct BOARD_ {
    int size, restrictionsNum;
    int **values, **restrictions, ***possibilities;
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
    board->restrictions = (int**) malloc (sizeof(int*)*size);
    for (i=0; i<size; i++){
        board->values[i] = (int*) malloc (sizeof(int)*size);
        board->restrictions[i] = (int*) malloc (sizeof(int)*size);
    }    
    
    return board;
}

BOARD **getInputs (int *inputSize){
    int i, x, y, size, restrictionsNum, x1, y1, x2, y2;
    
    scanf ("%d", inputSize);
    
    BOARD **inputs = (BOARD**) malloc (sizeof(BOARD*) * (*inputSize));
    
    for (i=0; i<*inputSize; i++){
        
        scanf ("%d %d", &size, &restrictionsNum);
        
        inputs[i] = inicializeBoard(size, restrictionsNum);

        for (x=0; x<inputs[i]->size; x++){
            for (y=0; y<inputs[i]->size; y++){
                scanf ("%d", &(inputs[i]->values[y][x]));
            }
        }
        
		
        for (x=0; x<inputs[i]->restrictionsNum; x++){
			scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
			x1--;
			x2--;
			y1--;
			y2--;
			if(x1 == x2){
				if(y1 < y2){
					inputs[i]->restrictions[y1][x1] += LTR;
					inputs[i]->restrictions[y2][x2] += GTL;
				}
				else{
					inputs[i]->restrictions[y1][x1] += LTL;
					inputs[i]->restrictions[y2][x2] += GTR;
				}
			}
			else if (y1 == y2){
				if(x1 < x2){
					inputs[i]->restrictions[y1][x1] += LTD;
					inputs[i]->restrictions[y2][x2] += GTU;
				}
				else{
					inputs[i]->restrictions[y1][x1] += LTU;
					inputs[i]->restrictions[y2][x2] += GTD;
				}
			}

        }
    }
    
    return inputs;
}
//fill possibilities[x][y][n]  with 1 if n is not a valid value in a blank
void calculatePossibilities(BOARD *board){
	int i, j, k;
	board->possibilities = (int***) malloc(sizeof(int**)*board->size);
	for(i = 0; i < board->size; i++){
		board->possibilities[i] = (int**) malloc(sizeof(int*)*board->size);
		for(j = 0; j < board->size; j++){
			board->possibilities[i][j] = (int*) calloc(sizeof(int),board->size);
		}
	}
	for(i = 0; i < board->size; i++){
		for(j = 0; j < board->size; j++){
			if(board->values[i][j] != 0){
				for(k = 0; k < board->size; k++){
					board->possibilities[i][k][board->values[i][j]] = 1;
					board->possibilities[k][j][board->values[i][j]]= 1;
				}
			}
		}
	}

}
void updatePossibilities(BOARD *board, int x, int y){
	int i;
	for(i = 0; i < board->size; i++){
		board->possibilities[i][y][board->values[x][y]] = 0;
		board->possibilities[x][i][board->values[x][y]] = 0;
	}
	if(board->restrictions[x][y] != 0){
		if((board->restrictions[x][y] & LTR) == LTR){
			for(i = board->values[x][y]	- 1; i >= 0; i--){
				board->possibilities[x+1][y][i] = 1;
			}
		}
		if((board->restrictions[x][y] & GTR) == GTR){
			for(i = board->values[x][y]	+ 1; i < board->size; i++){
				board->possibilities[x+1][y][i] = 1;
			}
		}
		if((board->restrictions[x][y] & LTL) == LTL){
			for(i = board->values[x][y]	- 1; i >= 0; i--){
				board->possibilities[x-1][y][i] = 1;
			}
		}
		if((board->restrictions[x][y] & GTL) == GTL){
			for(i = board->values[x][y]	+ 1; i < board->size; i++){
				board->possibilities[x-1][y][i] = 1;
			}
		}
		if((board->restrictions[x][y] & LTU) == LTU){
			for(i = board->values[x][y]	- 1; i >= 0; i--){
				board->possibilities[x][y-1][i] = 1;
			}
		}
		if((board->restrictions[x][y] & GTU) == GTU){
			for(i = board->values[x][y]	+ 1; i < board->size; i++){
				board->possibilities[x][y-1][i] = 1;
			}
		}
		if((board->restrictions[x][y] & LTD) == LTD){
			for(i = board->values[x][y]	- 1; i >= 0; i--){
				board->possibilities[x][y+1][i] = 1;
			}
		}
		if((board->restrictions[x][y] & GTD) == GTD){
			for(i = board->values[x][y]	+ 1; i < board->size; i++){
				board->possibilities[x][y+1][i] = 1;
			}
		}
	}
}
//get the next possibility for the position (x,y)
int getNextPossibility(BOARD *board, int x, int y){
	int i;
	for(i = board->values[x][y] + 1; i < board->size; i++){
		if(board->possibilities[x][y][i] == 1) return i;
	}
	return -1;
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
	if(board->restrictions[x][y] != 0){
		if((board->restrictions[x][y] & LTR) == LTR) 
			if(board->values[x][y] > board->values[x+1][y] && board->values[x+1][y] != 0) return 0;
		if((board->restrictions[x][y] & GTL) == GTL) 
			if(board->values[x][y] < board->values[x-1][y] && board->values[x-1][y] != 0) return 0;
		if((board->restrictions[x][y] & LTL) == LTL) 
			if(board->values[x][y] > board->values[x-1][y] && board->values[x-1][y] != 0) return 0;
		if((board->restrictions[x][y] & GTR) == GTR) 
			if(board->values[x][y] < board->values[x+1][y] && board->values[x+1][y] != 0) return 0;
		if((board->restrictions[x][y] & GTU) == GTU) 
			if(board->values[x][y] < board->values[x][y-1] && board->values[x][y-1] != 0) return 0;
		if((board->restrictions[x][y] & LTD) == LTD)                              
			if(board->values[x][y] > board->values[x][y+1] && board->values[x][y+1] != 0) return 0;
		if((board->restrictions[x][y] & GTD) == GTD)                              
			if(board->values[x][y] < board->values[x][y+1] && board->values[x][y+1] != 0) return 0;
		if((board->restrictions[x][y] & LTU) == LTU)                              
			if(board->values[x][y] > board->values[x][y-1] && board->values[x][y-1] != 0) return 0;
	}
    return 1;
}
int* nextBlank(BOARD *board, int x, int y){
	int i, j;
	int *next;
	for(i = y; i < board->size; i++){
		for(j = x; j < board->size; j++){
			if(board->values[j][i] == 0){
				next = (int*) malloc(sizeof(int)*2);
				next[0] = j;
				next[1] = i;
				return next;
			}
		}
		x=0;
	}
	return NULL;
		
}
int recursiveBacktrack(BOARD* board, int x, int y){
	counter++;
	int i, result;
	int *next = nextBlank(board, x, y);	
	if(next == NULL){
		return 1;
	}
	printf("%d %d\n", next[0], next[1]);
	for(i = 0; i < board->size; i++){
		board->values[next[0]][next[1]]++;
		printBoard(board);
		printf("Testando %d\n", board->values[next[0]][next[1]]);
		if(verifPosition(board, next[0], next[1])){
			printf("valido\n");
			result = recursiveBacktrack(board, next[0], next[1]);
			if(result == 1){
				free(next);
				return 1;
			}
		}
		
	}
	printf("invalido\n");
	board->values[next[0]][next[1]] = 0;
	free(next);
	return 0;
}
int main(){
    BOARD **inputs;
    int i, inputSize, heuristic;
    
    inputs = getInputs (&inputSize);
    
    do{
        printf ("Select a option:\n1: Simple Backtracking\n2: Backtracking with Forward Checking\n3: Backtracking with Forward Checking and MVR\n");

        scanf ("%d", &heuristic);
		//calculatePossibilities(inputs[i]);

        for (i=0; i<inputSize; i++){
            switch (heuristic){
                case 1: recursiveBacktrack(inputs[i], 0, 0); break;
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
