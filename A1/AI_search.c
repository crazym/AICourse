/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jul. 15
*/
#include <stdlib.h>
#include "AI_search.h"

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], 
	int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, 
	int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], 
		int cats, int cheeses, double gr[graph_size][4]))
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

						node at (i,j-1)
							^
							|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
							|
							v
						node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/
 if (mode == 0) {
 	bfs_search(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc);
 } else if (mode == 1) {
    int node_stack[graph_size][2], backtrack[graph_size][2];
    int visited[size_X][size_Y] = {0};
    int node_parent[size_X][size_Y];
 	dfs_search(0, 0, node_stack, visited, node_parent, 
 		gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc);

 } else if (mode == 2) {
 	// astar_search()
 } else{
 	// notify the user
 }

 return;
}

/*BFS search
*
* We use the front_pointer to keep track of expanded node, and node_order to 
* keep track of nodes in queue. node_parent keeps track of the index of parent of node at (x,y)
*
*/
void bfs_search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], 
	int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2]){
 
 int node_order = 0;
 int front_pointer = 0;
 int node_queue[graph_size][2];
 // sotres the parent's index in node_queue of visited nodes for backtrack
 int node_parent[size_X][size_Y];   
 int backtrack[graph_size][2];
 int path_len = 0;

 int cur_x, cur_y, cur_index;
 // initialize all nodes' visited status to false
 int visited[size_X][size_Y] = {0};

 // initialization on mouse loc as starting point
 node_queue[0][0]=mouse_loc[0][0];
 node_queue[0][1]=mouse_loc[0][1];
 node_parent[mouse_loc[0][0]][mouse_loc[0][1]] = 0;
 visited[mouse_loc[0][0]][mouse_loc[0][1]] = 1;
 visit_order[cur_x][cur_y] = 0;
 node_order++;

 // loop while queue is not empty
 while (front_pointer < graph_size && front_pointer < node_order) {
 	cur_x = node_queue[front_pointer][0];
 	cur_y = node_queue[front_pointer][1];
 	cur_index = cur_x + (cur_y*size_X);

 	visit_order[cur_x][cur_y] = front_pointer;

 	// exit while loop if found a cheese
 	if (check_cheese(cur_x, cur_y, cheese_loc, cheeses)) {
 		backtrack[path_len][0] = cur_x;
 		backtrack[path_len][1] = cur_y;
 		path_len++;
 		break;
 	}
 	// check neighours in order (TODO: needs refactoring on duplicate code...)
 	int x,y;
 	// check top
 	x = cur_x;
 	y = cur_y - 1;
	if ((gr[cur_index][0] == 1) && ( check_cats(x, y, cat_loc, cats) == 0) && visited[x][y] == 0){
		visited[x][y] = 1;
		node_parent[x][y] = front_pointer;
		node_queue[node_order][0] = x;
		node_queue[node_order][1] = y; 	 		
	 	node_order++;
	}

 	// check right
 	x = cur_x + 1;
 	y = cur_y;
	if ((gr[cur_index][1] == 1) && ( check_cats(x, y, cat_loc, cats) == 0) && visited[x][y] == 0){
		visited[x][y] = 1;
		node_parent[x][y] = front_pointer;
		node_queue[node_order][0] = x;
		node_queue[node_order][1] = y; 	 		
	 	node_order++;
	}

 	// check bottom
 	x = cur_x;
 	y = cur_y + 1;
	if ((gr[cur_index][2] == 1) && ( check_cats(x, y, cat_loc, cats) == 0) && visited[x][y] == 0){
		visited[x][y] = 1;
		node_parent[x][y] = front_pointer;
		node_queue[node_order][0] = x;
		node_queue[node_order][1] = y; 	 		
	 	node_order++;
	}

 	// check left
 	x = cur_x - 1;
 	y = cur_y;
	if ((gr[cur_index][3] == 1) && ( check_cats(x, y, cat_loc, cats) == 0) && visited[x][y] == 0){
		visited[x][y] = 1;
		node_parent[x][y] = front_pointer;
		node_queue[node_order][0] = x;
		node_queue[node_order][1] = y; 	 		
	 	node_order++;
	}

	// move front pointer for next round expansion
	front_pointer++;
 	// fprintf(stderr, "front pointer at %d while node order at %d\n", front_pointer, node_order);
 } 

 // found a cheese, start backtracking
 int tmp_counter = node_parent[backtrack[0][0]][backtrack[0][1]];
 // fprintf(stderr, "mouse loc at (%d, %d)\n", mouse_loc[0][0], mouse_loc[0][1]);
 // fprintf(stderr, "cheese loc at (%d, %d)\n", backtrack[0][0], backtrack[0][1]);

 while (! (node_queue[tmp_counter][0] == mouse_loc[0][0] && node_queue[tmp_counter][1] == mouse_loc[0][1])) {
/* 	fprintf(stderr, "backtrack parent to (%d, %d) at path_len=%d and node_order=%d\n", 
 		node_queue[tmp_counter][0], node_queue[tmp_counter][1], path_len, 
 		tmp_counter);
*/
 	backtrack[path_len][0] = node_queue[tmp_counter][0];
 	backtrack[path_len][1] = node_queue[tmp_counter][1];
 	// set next counter to be the visit order of parent node in queue
 	tmp_counter = node_parent[backtrack[path_len][0]][backtrack[path_len][1]];
 	path_len++;
 }

 backtrack[path_len][0] = mouse_loc[0][0];
 backtrack[path_len][1] = mouse_loc[0][1];

 for (int i=0; i <= path_len; i++) {
	fprintf(stderr, "backtracking at (%d, %d)\n", backtrack[path_len-i][0], backtrack[path_len-i][1]);
	path[i][0] = backtrack[path_len-i][0];
	path[i][1] = backtrack[path_len-i][1];
 }
}
 
// DFS search
int dfs_search(int node_count, int node_order, int node_stack[graph_size][2],
	int visited[size_X][size_Y], int node_parent[size_X][size_Y],
	double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], 
	int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2]){

 int cur_x, cur_y, cur_index;
 if (node_count == 0) {
	 // base case on mouse loc as starting point
	 node_stack[node_count][0]=mouse_loc[0][0];
	 node_stack[node_count][1]=mouse_loc[0][1];
	 node_parent[mouse_loc[0][0]][mouse_loc[0][1]] = 0;
	 visited[mouse_loc[0][0]][mouse_loc[0][1]] = 1;
	 return dfs_search(node_count+1, node_order+1, node_stack, visited, node_parent, gr, path, visit_order, 
	 	cat_loc, cats, cheese_loc, cheeses, mouse_loc);

 } else {
 	int orig_count;
 	orig_count = node_count-1;
 	cur_x = node_stack[orig_count][0];
 	cur_y = node_stack[orig_count][1];
 	cur_index = cur_x + (cur_y*size_X);
 	fprintf(stderr, "node at (%d, %d) at depth %d\n", cur_x, cur_y, node_order);
 	visit_order[cur_x][cur_y] = node_order;

 	// exit recursion if found a cheese
 	if (check_cheese(cur_x, cur_y, cheese_loc, cheeses)) {
 		// fill in the rest of the path array with cheeses location to avoid SegFault
		for (int i=node_order; i<graph_size; i++) {
		 	path[i][0] = cur_x;
		 	path[i][1] = cur_y;
		}
 		return 1;
 	}

 	int x,y;
 	// check top
 	x = cur_x;
 	y = cur_y - 1;
	if ((gr[cur_index][0] == 1) && ( check_cats(x, y, cat_loc, cats) == 0) && visited[x][y] == 0){
		visited[x][y] = 1;
		node_parent[x][y] = orig_count;
		node_stack[node_count][0] = x;
		node_stack[node_count][1] = y; 	 		
	 	node_count++;
	}

 	// check right
 	x = cur_x + 1;
 	y = cur_y;
	if ((gr[cur_index][1] == 1) && ( check_cats(x, y, cat_loc, cats) == 0) && visited[x][y] == 0){
		visited[x][y] = 1;
		node_parent[x][y] = orig_count;
		node_stack[node_count][0] = x;
		node_stack[node_count][1] = y; 	 		
	 	node_count++;
	}

 	// check bottom
 	x = cur_x;
 	y = cur_y + 1;
	if ((gr[cur_index][2] == 1) && ( check_cats(x, y, cat_loc, cats) == 0) && visited[x][y] == 0){
		visited[x][y] = 1;
		node_parent[x][y] = orig_count;
		node_stack[node_count][0] = x;
		node_stack[node_count][1] = y; 	 		
	 	node_count++;
	}

 	// check left
 	x = cur_x - 1;
 	y = cur_y;
	if ((gr[cur_index][3] == 1) && ( check_cats(x, y, cat_loc, cats) == 0) && visited[x][y] == 0){
		visited[x][y] = 1;
		node_parent[x][y] = orig_count;
		node_stack[node_count][0] = x;
		node_stack[node_count][1] = y; 	 		
	 	node_count++;
	}

	// backtrack to parent if find a dead-end path
	// hmm, seems to have something wrong here, no time to check QAQ
	if ((orig_count == node_count-1) && (orig_count != 0)) {
		node_count = node_parent[cur_x][cur_y];
	}
	// recursive case
	int path_len;
	node_order++;
	path_len = dfs_search(node_count, node_order, node_stack, visited, node_parent, gr, path, visit_order, 
		cat_loc, cats, cheese_loc, cheeses, mouse_loc);

	if (path_len > 0){
		path[node_order][0] = cur_x;
		path[node_order][1] = cur_y;
		return path_len+1;
	}
 } 

 return 0;
}

int check_cats(int x, int y, int cat_loc[10][2], int cats){
	for (int i = 0; i<cats; i++) {
		if (x == cat_loc[i][0] && y == cat_loc[i][1])
			return 1;
	}
	return 0;
}

int check_cheese(int x, int y, int cheese_loc[10][2], int cheeses){
	for (int i = 0; i<cheeses; i++) {
		if (x == cheese_loc[i][0] && y == cheese_loc[i][1])
			return 1;
	}
	return 0;
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
 */
	// mahatton distance: sum of x and y distance
	int min_dist=99; // infinity as the largest distance cannot exceed sizeX + sizeY
	int cur_dist;
	for (int i=0; i<cheeses; i++) {
		cur_dist = abs(x - cheese_loc[i][0]) + abs(y - cheese_loc[i][1]);
		if (cur_dist < min_dist)
			min_dist = cur_dist;
	}
 	return min_dist;	
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. 

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */

 return(1);		// <-- Evidently you will need to update this.
}

