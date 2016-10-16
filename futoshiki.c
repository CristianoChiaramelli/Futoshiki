#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LTR 1		// 1 - menor que o da direita
#define GTL	2		// 2 - maior que o da esquerda
#define LTL 4		// 4 - menor que o da esquerda
#define GTR	8		// 8 - maior que o da direita
#define LTD 16		// 16 - menor que o de baixo
#define GTU 32		// 32 - maior que o de cima
#define LTU 64		// 64 - menor que o de cima
#define GTD	128		// 128 - maior que o de baixo

#define POSSIBLE 0
#define IMPOSSIBLE 1

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

void printPossibilities (BOARD *board){
	/*
	printf ("\nPOSSIBILITIES NOW ->\n");
	int i,j,k;

	for (i=0; i<board->size; i++){
		for (j=0; j<board->size; j++){
			printf ("(%d,%d)(", j, i);
			for (k=0; k<board->size+1; k++){
				printf ("%d ", board->possibilities[j][i][k]);
			}
			printf (")");
		}
		printf ("\n");
	}
	printf ("\n");*/
}

int *getNextVariable (BOARD *board, int x, int y){
	int *variable = (int*) malloc (sizeof(int)*2);
	int i, j, lower = 10;
	for (i=0; i<board->size; i++){
		for (j=0; j<board->size; j++){
			if (board->values[i][j] == 0 && board->possibilities[i][j][0] < lower){
				lower = board->possibilities[i][j][0];
				variable[0] = i;
				variable[1] = j;

				if (lower==1)
					return variable;
			}
		}
	}
	if (lower == 10){
		free (variable);
		variable = NULL;
	}

	return variable;
}


int updatePossibilities(BOARD *board, int x, int y){
	int i, j;
	if (board->values[x][y]==0) return 1;

	if(board->restrictions[x][y] != 0){
		//printf ("Restriction: %d\n", board->restrictions[x][y]);
 		if((board->restrictions[x][y] & LTD) == LTD){
 			//printf ("RESTRICTION LTU (%d,%d) > (%d,%d)\n", x,y+1,x,y);
 			//If it must be lesser than the value above, i check the upper line
	 		if (y+1 < board->size){
	 			for(j = board->values[x][y]; j >= 1; j--){
	 				if (board->possibilities[x][y+1][j] != IMPOSSIBLE){
	 					board->possibilities[x][y+1][j] = IMPOSSIBLE;
	 					board->possibilities[x][y+1][0]--;
	 				}
	 			}	 	 	
	 			if (board->values[x][y+1] != 0){
	 				for(j = board->values[x][y+1]; j <= board->size; j++){
		 				if (board->possibilities[x][y][j] != IMPOSSIBLE){
		 					board->possibilities[x][y][j] = IMPOSSIBLE;
		 					board->possibilities[x][y][0]--;
		 				}
		 			}
	 			}
	 		}
 		}

 		if((board->restrictions[x][y] & GTD) == GTD){
 			//printf ("RESTRICTION GTU (%d,%d) > (%d,%d)\n", x,y,x,y+1);
	 		if (y+1 < board->size){
	 			for(j = board->values[x][y]; j <= board->size; j++){
	 				if (board->possibilities[x][y+1][j] != IMPOSSIBLE){
	 					board->possibilities[x][y+1][j] = IMPOSSIBLE;
	 					board->possibilities[x][y+1][0]--;
	 				}
	 			}
	 			if (board->values[x][y+1] != 0){
	 				for(j = board->values[x][y+1]; j >= 1; j--){
		 				if (board->possibilities[x][y][j] != IMPOSSIBLE){
		 					board->possibilities[x][y][j] = IMPOSSIBLE;
		 					board->possibilities[x][y][0]--;
		 				}
	 				}
		 		}
 			}
 		}

 		if((board->restrictions[x][y] & LTU) == LTU){
 			//printf ("RESTRICTION LTD (%d,%d) > (%d,%d)\n", x,y-1,x,y);
 			if (y-1 >= 0){
	 			for(j = board->values[x][y]; j >= 1; j--){
	 				if (board->possibilities[x][y-1][j] != IMPOSSIBLE){
	 					board->possibilities[x][y-1][j] = IMPOSSIBLE;
	 					board->possibilities[x][y-1][0]--;
	 				}
	 			}
	 			if (board->values[x][y-1] != 0){
	 				for(j = board->values[x][y-1]; j <= board->size; j++){
		 				if (board->possibilities[x][y][j] != IMPOSSIBLE){
		 					board->possibilities[x][y][j] = IMPOSSIBLE;
		 					board->possibilities[x][y][0]--;
		 				}
		 			}
		 		}
 			}
 		}

 		if((board->restrictions[x][y] & GTU) == GTU){
 			//printf ("RESTRICTION GTD (%d,%d) > (%d,%d)\n", x,y,x,y-1);
 			if (y-1 >= 0){
	 			for(j = board->values[x][y]; j <= board->size; j++){
	 				if (board->possibilities[x][y-1][j] != IMPOSSIBLE){
	 					board->possibilities[x][y-1][j] = IMPOSSIBLE;
	 					board->possibilities[x][y-1][0]--;
	 				}	
	 			}	 	
	 			if (board->values[x][y-1] != 0){
	 				for(j = board->values[x][y-1]; j >= 1; j--){
		 				if (board->possibilities[x][y][j] != IMPOSSIBLE){
		 					board->possibilities[x][y][j] = IMPOSSIBLE;
		 					board->possibilities[x][y][0]--;
		 				}
		 			}
	 			}	
 			}
 		}

 		if((board->restrictions[x][y] & LTR) == LTR){
 			//printf ("RESTRICTION LTR (%d,%d) > (%d,%d)\n", x+1,y,x,y);
 			if (x+1 < board->size){
	 			for(j = board->values[x][y]; j >= 1; j--){ //[x][y] smaller values
	 				if (board->possibilities[x+1][y][j] != IMPOSSIBLE){
	 					board->possibilities[x+1][y][j] = IMPOSSIBLE;
	 					board->possibilities[x+1][y][0]--;
	 				}
	 			}	 	 	
	 			if (board->values[x+1][y] != 0){ //[x+1][y] greater values
	 				for(j = board->values[x+1][y]; j <= board->size; j++){
		 				if (board->possibilities[x][y][j] != IMPOSSIBLE){
		 					board->possibilities[x][y][j] = IMPOSSIBLE;
		 					board->possibilities[x][y][0]--;
		 				}
	 				}
		 		}
 			}
 		}

 		if((board->restrictions[x][y] & GTR) == GTR){
 			//printf ("RESTRICTION GTR (%d,%d) > (%d,%d)\n", x,y,x+1,y);
 			if (x+1 < board->size){
	 			for(j = board->values[x][y]; j <= board->size; j++){
	 				if (board->possibilities[x+1][y][j] != IMPOSSIBLE){
	 					board->possibilities[x+1][y][j] = IMPOSSIBLE;
	 					board->possibilities[x+1][y][0]--;
	 				}
	 			}	 	
	 			if (board->values[x+1][y] != 0){
	 				for(j = board->values[x+1][y]; j >= 1; j--){
		 				if (board->possibilities[x][y][j] != IMPOSSIBLE){
		 					board->possibilities[x][y][j] = IMPOSSIBLE;
		 					board->possibilities[x][y][0]--;
		 				}
		 			}
		 		}
 			}	
 		}

 		if((board->restrictions[x][y] & LTL) == LTL){
 			//printf ("RESTRICTION LTL (%d,%d) > (%d,%d)\n", x-1,y,x,y); 			
 			if (x-1 >= 0){
	 			for(j = board->values[x][y]; j >= 1; j--){ //[x][y] smaller values
	 				if (board->possibilities[x-1][y][j] != IMPOSSIBLE){
	 					board->possibilities[x-1][y][j] = IMPOSSIBLE;
	 					board->possibilities[x-1][y][0]--;
	 				}
	 			}	 	 	
	 			if (board->values[x-1][y] != 0){ //[x+1][y] greater values
	 				for(j = board->values[x-1][y]; j <= board->size; j++){
		 				if (board->possibilities[x][y][j] != IMPOSSIBLE){
		 					board->possibilities[x][y][j] = IMPOSSIBLE;
		 					board->possibilities[x][y][0]--;
		 				}
		 			}
 				}
 			}
 		}

 		if((board->restrictions[x][y] & GTL) == GTL){
 			//printf ("RESTRICTION GTL (%d,%d) > (%d,%d)\n", x,y,x-1,y);
 			if (x-1 >= 0){
	 			for(j = board->values[x][y]; j <= board->size; j++){
	 				if (board->possibilities[x-1][y][j] != IMPOSSIBLE){
	 					board->possibilities[x-1][y][j] = IMPOSSIBLE;
	 					board->possibilities[x-1][y][0]--;
	 				}
	 			}	 	
	 			if (board->values[x-1][y] != 0){
	 				for(j = board->values[x-1][y]; j >= 1; j--){
		 				if (board->possibilities[x][y][j] != IMPOSSIBLE){
		 					board->possibilities[x][y][j] = IMPOSSIBLE;
		 					board->possibilities[x][y][0]--;
		 				}
		 			}
 				}
 			}	
 		}
	}

	for(i = 0; i < board->size; i++){
		if (board->possibilities[x][i][board->values[x][y]] != IMPOSSIBLE){
			board->possibilities[x][i][board->values[x][y]] = IMPOSSIBLE; //The same for its line 'x'
			board->possibilities[x][i][0]--;

			if (board->possibilities[x][i][0] <= 0 && board->values[x][i] == 0){
				//printf ("Shit on (%d,%d):\n", x, i);
				//printPossibilities(board);
				return 0; //SHIT HAPPENS
			}
			if (i+1 < board->size && board->possibilities[x][i+1][0] <= 0 && board->values[x][i+1] == 0){
				//printf ("Shit on (%d,%d):\n", x, i+1);
				//printPossibilities(board);
				return 0; //SHIT HAPPENS
			}
			if (i-1 >= 0 && board->possibilities[x][i-1][0] <= 0 && board->values[x][i-1] == 0){
				// printf ("Shit on (%d,%d):\n", x, i-1);
				//printPossibilities(board);
				return 0; //SHIT HAPPENS
			}
		}
	}

	//Iterate thought the line y
	for (i=0; i < board->size; i++){
		if (i==x) continue;
		if (board->possibilities[i][y][board->values[x][y]] != IMPOSSIBLE){
			board->possibilities[i][y][board->values[x][y]] = IMPOSSIBLE; //All in the columm 'y' with that possibility will be IMPOSSIBLE
			board->possibilities[i][y][0]--;

			if (board->possibilities[i][y][0] <= 0 && board->values[i][y]==0){
				// printf ("Shit on (%d,%d):\n", i, y);
				// printPossibilities(board);
				return 0; //SHIT HAPPENS
			}	
			if (i+1<board->size && board->possibilities[i+1][y][0] <= 0 && board->values[i+1][y]==0){
				// printf ("Shit on (%d,%d):\n", i+1, y);
				// printPossibilities(board);
				return 0; //SHIT HAPPENS
			}	
			if (i-1>= 0 && board->possibilities[i-1][y][0] <= 0 && board->values[i-1][y]==0){
				// printf ("Shit on (%d,%d):\n", i-1, y);
				// printPossibilities(board);
				return 0; //SHIT HAPPENS
			}			
		}
	}


	printPossibilities(board);

	return 1;
}


//fill possibilities[x][y][n]  with 1 if n is not a valid value in a blank
void calculatePossibilities(BOARD *board){
	int i, j;
	board->possibilities = (int***) malloc(sizeof(int**)*board->size);
	for(i = 0; i < board->size; i++){
		board->possibilities[i] = (int**) malloc(sizeof(int*)*board->size+1);
		for(j = 0; j < board->size; j++){
			board->possibilities[i][j] = (int*) calloc(board->size+1, sizeof(int));
			board->possibilities[i][j][0] = board->size;
		}
	}

	for (i=0; i<board->size; i++){
		for (j=0; j<board->size; j++){
			updatePossibilities(board, i, j);
		}
	}
}

int ***copyPossibilities(BOARD *board){
int i, j, ***newPossibilities = (int***) malloc(sizeof(int**) * board->size);
	for(i = 0; i < board->size; i++){
		newPossibilities[i] = (int**) malloc(sizeof(int*)*board->size+1);
		for(j = 0; j < board->size; j++){
			newPossibilities[i][j] = (int*) calloc((board->size+1), sizeof(int));
			newPossibilities[i][j] = memcpy(newPossibilities[i][j], board->possibilities[i][j], (board->size+1)*sizeof(int));
		}
	}
	if (newPossibilities == NULL){
		printf ("\n BACKUP FAILED\n");
	}
	return newPossibilities;
}

//get the next possibility for the position (x,y)
int getNextPossibility(BOARD *board, int x, int y){
	int i;
	for(i = board->values[x][y] + 1; i <= board->size; i++){
		if(board->possibilities[x][y][i] == POSSIBLE) return i;
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
			if(board->values[x][y] >= board->values[x+1][y] && board->values[x+1][y] != 0) return 0;
		if((board->restrictions[x][y] & GTL) == GTL) 
			if(board->values[x][y] <= board->values[x-1][y] && board->values[x-1][y] != 0) return 0;
		if((board->restrictions[x][y] & LTL) == LTL) 
			if(board->values[x][y] >= board->values[x-1][y] && board->values[x-1][y] != 0) return 0;
		if((board->restrictions[x][y] & GTR) == GTR) 
			if(board->values[x][y] <= board->values[x+1][y] && board->values[x+1][y] != 0) return 0;
		if((board->restrictions[x][y] & GTU) == GTU) 
			if(board->values[x][y] <= board->values[x][y-1] && board->values[x][y-1] != 0) return 0;
		if((board->restrictions[x][y] & LTD) == LTD)                              
			if(board->values[x][y] >= board->values[x][y+1] && board->values[x][y+1] != 0) return 0;
		if((board->restrictions[x][y] & GTD) == GTD)                              
			if(board->values[x][y] <= board->values[x][y+1] && board->values[x][y+1] != 0) return 0;
		if((board->restrictions[x][y] & LTU) == LTU)                              
			if(board->values[x][y] >= board->values[x][y-1] && board->values[x][y-1] != 0) return 0;
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
	if (counter >= 1000000){
		return 0;
	}
	
	int i, result;
	int *next = nextBlank(board, x, y);	
	if(next == NULL){
		return 1;
	}
	for(i = 0; i < board->size; i++){
		board->values[next[0]][next[1]]++;

		if(verifPosition(board, next[0], next[1])){
			result = recursiveBacktrack(board, next[0], next[1]);
			if(result == 1){
				free(next);
				return 1;
			}
		}
		
	}
//	printf("invalido\n");
	board->values[next[0]][next[1]] = 0;
	free(next);
	return 0;
}

void freePossibilities (BOARD *board){
	int i, j;
	for (i=0; i<board->size; i++){
		for (j=0; j<board->size; j++){
			free (board->possibilities[i][j]);
		}
		free (board->possibilities[i]);
	}
	free (board->possibilities);
}

int forwardCheckBacktrack(BOARD *board, int x, int y){
	counter++;
	if (counter >= 1000000){
		return 0;
	}

	int i, result;
	int ***possibilitiesCpy = NULL;
	int *next = nextBlank(board, x, y);	
	if(next == NULL){
		return 1;
	}

	for(i = 0; i < board->size; i++){

		board->values[next[0]][next[1]] = getNextPossibility(board, next[0], next[1]);
		// printf ("Board (%d,%d) receives %d\n", next[0], next[1],board->values[next[0]][next[1]]);

		if(board->values[next[0]][next[1]] == -1){ //If there are no possibilities, backtrack
			board->values[next[0]][next[1]] = 0;
			// printf ("No more possibilities for %d,%d. Backtrack\n", next[0], next[1]);
			break;
		}

		possibilitiesCpy = copyPossibilities(board);

		//If update == 0, some place is now without possibilities. Backtrack
		if (updatePossibilities(board, next[0], next[1]) == 0){
			freePossibilities(board);
			board->possibilities = possibilitiesCpy;
			// printf ("Forward alerted shit would happens... Backtrack\n");
			continue;
		}

		result = forwardCheckBacktrack(board, next[0], next[1]);
		if(result == 1){
			free(next);
			return 1;
		} else {
			freePossibilities(board);
			board->possibilities = possibilitiesCpy;
		}
	}

	board->values[next[0]][next[1]] = 0;
	free(next);
	return 0;
}



int mvrForwardCheckBacktrack(BOARD *board, int x, int y){
	counter++;
	if (counter >= 1000000){
		return 0;
	}

	int i, result;
	int ***possibilitiesCpy = NULL;
	int *next = getNextVariable(board, x, y);	
	if(next == NULL){
		return 1;
	}

	for(i = 0; i < board->size; i++){

		board->values[next[0]][next[1]] = getNextPossibility(board, next[0], next[1]);
		// printf ("Board (%d,%d) receives %d\n", next[0], next[1],board->values[next[0]][next[1]]);

		if(board->values[next[0]][next[1]] == -1){ //If there are no possibilities, backtrack
			board->values[next[0]][next[1]] = 0;
			// printf ("No more possibilities for %d,%d. Backtrack\n", next[0], next[1]);
			break;
		}

		possibilitiesCpy = copyPossibilities(board);

		//If update == 0, some place is now without possibilities. Backtrack
		if (updatePossibilities(board, next[0], next[1]) == 0){
			freePossibilities(board);
			board->possibilities = possibilitiesCpy;
			// printf ("Forward alerted shit would happens... Backtrack\n");
			continue;
		}

		result = mvrForwardCheckBacktrack(board, next[0], next[1]);
		if(result == 1){
			free(next);
			return 1;
		} else {
			freePossibilities(board);
			board->possibilities = possibilitiesCpy;
		}
	}

	board->values[next[0]][next[1]] = 0;
	free(next);
	return 0;
}

//Verifies if the final board respects all the conditions
int verifBoard (BOARD *board){
	int i,j;
	for (i=0; i<board->size; i++){
		for (j=0; j<board->size; j++){
			if (board->values[i][j] == 0) return 0;
			else if (verifPosition(board, i, j) == 0) return 0;
		}
	}
	return 1;
}

int main(){
    BOARD **inputs;
    int i, inputSize, heuristic;
    time_t start;
    
    inputs = getInputs (&inputSize);
    
    do{
        printf ("Select a option:\n1: Simple Backtracking\n2: Backtracking with Forward Checking\n3: Backtracking with Forward Checking and MVR\n");

        scanf ("%d", &heuristic);

        start = time(NULL);

        for (i=0; i<inputSize; i++){
            counter = 0;
            switch (heuristic){
                case 1: recursiveBacktrack(inputs[i], 0, 0); break;
                case 2: calculatePossibilities(inputs[i]);
						forwardCheckBacktrack(inputs[i], 0, 0); break;

                case 3: calculatePossibilities(inputs[i]);
                		mvrForwardCheckBacktrack(inputs[i], 0, 0); break;
                default: printf("Select one of the options 1,2 or 3\n");
            }
			printf ("Solved %d with %d recursions\n", i, counter);
        }
        int badCounter = 0;
        for (i=0; i<inputSize; i++){
        	printf ("%d\n", i+1);
            printBoard(inputs[i]);
            if (verifBoard(inputs[i])){
            	printf ("Deu bom\n");
            } else {
				badCounter ++;
				printf ("Deu ruim\n");
            }
        }
        printf ("Right: %d/%d\n", 100-badCounter, 100);
        printf ("Total time: aprox. %ds\n", time(NULL) - start);
    } while (heuristic > 3 || heuristic < 1);
}
