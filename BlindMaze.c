#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Maze_struct {
	
	int width;
	int height;
	int mazeArray[100][100][5]; //max size of array is 100. uninitialized values are 0 by default
	//Each maze will just be a chunk of that array (Dimensions: width by height)
	
	/*		
	              up = 0
		             ^
	   left = 3	   <-+->    right = 1
		             v
	             down = 2
	*/
	
} Maze;

Maze GenerateMaze(int mazeWidth, int mazeHeight);
void NavigateMaze(Maze maze);
void PrintMaze(Maze *maze);
void PrintMazeFormat(Maze *maze);
void Open(Maze *maze, int x, int y, int dir);
int isdigit(int c);

int main(void)
{	
	int x, y, n;
	char c;
	
	do{
		if(!c)
		printf("Enter width of maze\n");
		n = scanf("%d", &x);
		if (n == 0)
		{
			printf("%s\n","Invalid Input, please enter a number greater than 3");
			do
			{
    			c = getchar();
 			}
  		while (!isdigit(c));
	  	ungetc(c, stdin);
 		 //consume non-numeric chars from buffer
		}

	}while(n==0);
	
	do{
		printf("Enter height of maze: ");
		n = scanf("%d", &y);
	}while(n==0 || y < 3);
	
	Maze maze = GenerateMaze(x, y);
	//PrintMaze(&maze);
	PrintMazeFormat(&maze);
	NavigateMaze(maze);
	
	return 0;
}

Maze GenerateMaze(int mazeWidth, int mazeHeight)
{
	Maze maze;
	maze.width = mazeWidth;
	maze.height = mazeHeight;
	time_t t;
	srand((unsigned) time(&t));
	
	//---fill maze with walls
	for(int i = 0; i < maze.width; i++){
		for(int j = 0; j < maze.height; j++){
			maze.mazeArray[i][j][0] = 1;
			maze.mazeArray[i][j][1] = 1;
			maze.mazeArray[i][j][2] = 1;
			maze.mazeArray[i][j][3] = 1;
		}
	}
	
	//------------GENERATE MAZE-------------------
	
	int r1, r2, res1, p_Dir1 = 1;  // variables for start path
	int r4, r5, res2, p_Dir2 = 3;  // variables for end path
	int n = 0, r3;
	int path1x = 0, path1y = 0, path2x = maze.width - 1, path2y = maze.height - 1;
	int isconnected = 0;
	
	//--The bigger the maze, the more likely the path will go down and right
	int r3_max;
	if((maze.width + maze.height) / 2 > 60){
		r3_max = 3;
	}else if((maze.width + maze.height) / 2 > 40){
		r3_max = 4;
	}else if((maze.width + maze.height) / 2 > 20){
		r3_max = 5;
	}else{
		r3_max = 6;
	}

	do{
		
		while(n < (maze.width + maze.height) / 2 * 5){
			
			n++;
			r1 = rand()%4;
			r2 = rand()%4;
			r3 = rand()%r3_max;
			r4 = rand()%4;
			r5 = rand()%4;
			
			//----Tendancy to go the same direction repeatedly/Tendancy to go down and right (start path)
			if(r1 == p_Dir1 || r2 == p_Dir1){
				res1 = p_Dir1;
			}else{
				res1 = r1;
				if(r3 == 0)
					res1 = 1;
				if(r3 == 1)
					res1 = 2;
			}
			
			//----Tendancy to go the same direction repeatedly/Tendancy to go up and left (end path)
			if(r4 == p_Dir2 || r5 == p_Dir2){
				res2 = p_Dir2;
			}else{
				res2 = r4;
				if(r3 == 0)
					res2 = 1;
				if(r3 == 1)
					res2 = 2;
			}
			
			//if either path tries to go back on itself
			if(res1 == (p_Dir1 + 2)%4)
				res1 = -1;
			
			if(res2 == (p_Dir2 + 2)%4)
				res2 = -1;
			
			//printf("> %d ", res1);
			maze.mazeArray[path1x][path1y][4] = 1;
			maze.mazeArray[path2x][path2y][4] = 2;
			
			//check and create path1
			if(path1x >= 0 && path1x < maze.width && path1y >= 0 && path1y < maze.height){
				switch(res1){
					case 0:
						if(path1y != 0){
							if(maze.mazeArray[path1x][path1y-1][4] != 1){
								Open(&maze, path1x, path1y, res1);
								path1y -= 1;
								p_Dir1 = res1;
							}
						}
						break;
					case 1:
						if(path1x != maze.width - 1){
							if(maze.mazeArray[path1x+1][path1y][4] != 1){
								Open(&maze, path1x, path1y, res1);
								path1x += 1;
								p_Dir1 = res1;
							}
						}
						break;
					case 2:
					if(path1y != maze.height - 1){
							if(maze.mazeArray[path1x][path1y+1][4] != 1){
								Open(&maze, path1x, path1y, res1);
								path1y += 1;
								p_Dir1 = res1;
							}
						}
						break;
					case 3:
						if(path1x != 0){
							if(maze.mazeArray[path1x-1][path1y][4] != 1){
								Open(&maze, path1x, path1y, res1);
								path1x -= 1;
								p_Dir1 = res1;
							}
						}
						break;
					default:
						break;
				}
			}
			
			//path2
			if(path2x >= 0 && path2x < maze.width && path2y >= 0 && path2y < maze.height){
				switch(res2){
					case 0:
						if(path2y != 0){
							if(maze.mazeArray[path2x][path2y-1][4] != 2){
								Open(&maze, path2x, path2y, res2);
								path2y -= 1;
								p_Dir2 = res2;
							}
						}
						break;
					case 1:
						if(path2x != maze.width - 1){
							if(maze.mazeArray[path2x+1][path2y][4] != 2){
								Open(&maze, path2x, path2y, res2);
								path2x += 1;
								p_Dir2 = res2;
							}
						}
						break;
					case 2:
						if(path2y != maze.height - 1){
							if(maze.mazeArray[path2x][path2y+1][4] != 2){
								Open(&maze, path2x, path2y, res2);
								path2y += 1;
								p_Dir2 = res2;
							}
						}
						break;
					case 3:
						if(path2x != 0){
							if(maze.mazeArray[path2x-1][path2y][4] != 2){
								Open(&maze, path2x, path2y, res2);
								path2x -= 1;
								p_Dir2 = res2;
							}
						}
						break;
					default:
						break;
				}
			}
			
			if(maze.mazeArray[path1x][path1y][4] == 2 || maze.mazeArray[path2x][path2y][4] == 1){
				isconnected = 1;
				break;
			}
		}
		printf("%d", isconnected);
		PrintMazeFormat(&maze);
	}while(isconnected != 1);
	
	//------------END OF ALGORYTHM------------------
	
	
	//set the outer walls of the maze to 1 (closed)
	for(int i = 0; i < maze.width; i++){
		maze.mazeArray[i][0][0] = 1;
		maze.mazeArray[i][maze.height - 1][2] = 1;
	}
	
	for(int i = 0; i < maze.height; i++){
		maze.mazeArray[0][i][3] = 1;
		maze.mazeArray[maze.width - 1][i][1] = 1;
	}
	
	printf("\nMaze Generated!\n");
	
	return maze;
}

void NavigateMaze(Maze maze)
{
	char dir; // wasd
	int posx = 0, posy = 0; //to keep track of the position in the maze
	
	printf("\nNavigating maze:   (Type w, a, s, d.  Type q to quit.)\n");
	
	do{
		
		do{
			scanf("%c", &dir);
		}while(dir!='w' && dir!='a' && dir!='s' && dir!='d' && dir!='q');
		
		printf(" (%d, %d)  ", posx, posy);
		
		switch(dir){
			
			case 'w':
				if(maze.mazeArray[posx][posy][0] != 1){
						posy -= 1;
					}else{
						printf("WALL\n");
					}
					break;
					
				case 'd':
					if(maze.mazeArray[posx][posy][1] != 1){
						posx += 1;
					}else{
						printf("WALL\n");
					}
					break;
					
				case 's':
					if(maze.mazeArray[posx][posy][2] != 1){
						posy += 1;
					}else{
						printf("WALL\n");
					}
					break;
					
				case 'a':
					if(maze.mazeArray[posx][posy][3] != 1){
						posx -= 1;
					}else{
						printf("WALL\n");
					}
					break;
				
		}
			
			
			
	}while(dir!='q');
	
}

//----Prints the maze with all the data
void PrintMaze(Maze *maze){
		for(int i = 0; i < maze->height; i++){
			for(int j = 0; j < maze->width; j++){
				printf("[%d %d %d %d (%d)] ", maze->mazeArray[j][i][0], maze->mazeArray[j][i][1], 
				maze->mazeArray[j][i][2], maze->mazeArray[j][i][3], maze->mazeArray[j][i][4]);
			}
			printf("\n");
		}
		return;
}

//----Prints the maze in a nicer format
void PrintMazeFormat(Maze *maze){
	
	printf("_");
	for(int i = 0; i < maze->width; i++)printf("___");
	printf("\n");
	
	for(int i = 0; i < maze->height; i++){
		
		printf("|");
		
		for(int j = 0; j < maze->width; j++){
			if(maze->mazeArray[j][i][2] == 1) printf("__");
			else printf("  ");
			
			if(maze->mazeArray[j][i][1] == 1) printf("|");
			else printf(" ");
		}
		
		printf("\n");
	}
	
	return;
}

//----Function for breaking walls
void Open(Maze *maze, int x, int y, int dir){
	
	maze->mazeArray[x][y][dir] = 0;
	
	switch(dir){
		case 0:
			maze->mazeArray[x][y-1][2] = 0;
			break;
		case 1:
			maze->mazeArray[x+1][y][3] = 0;
			break;
		case 2:
			maze->mazeArray[x][y+1][0] = 0;
			break;
		case 3:
			maze->mazeArray[x-1][y][1] = 0;
			break;
	}
	
	return;
}
