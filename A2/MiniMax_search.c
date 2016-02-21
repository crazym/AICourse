/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
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

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], 
	int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, 
	double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], 
		int cats, int cheeses, int depth, double gr[graph_size][4]), 
	int agentId, int depth, int maxDepth, double alpha, double beta)
{
 /*
 Instruction:
	   This function is the interface between your solution for the assignment and the driver code. The driver code
	   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
	   
	   Board and game layout:

		Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
		list and agent positions are stored.	

		Note that in this case, the path will contain a single move - at the top level, this function will provide
		an agent with the 'optimal' mini-max move given the game state.


	   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
		 	   all game states down to the maximum search depth specified by the user. In order to do that,
			   the function needs to be called with the correct state at each specific node in the mini-max
			   search tree.

			   The game state is composed of:

				* Mouse, cat, and cheese positions (and number of cats and cheeses)
				
			   At the top level (when this function is called by the mini-max driver code), the game state
			   correspond to the current situation of the game. But once you start recursively calling
			   this function for lower levels of the search tree the positions of agents will have changed.
			   
			   Therefore, you will need to define local variables to keep the game state at each node of the
			   mini-max search tree, and you will need to update this state when calling recursively so that
			   the search does the right thing.

			   This function *must check* whether:
				* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
				  at which point it calls the utility function to get a value
		 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
				  the utility function to get a value
				* Otherwise, call recursively using the candidate configuration to find out what happens
				  deeper into the mini-max tree.

	   Arguments:
			gr[graph_size][4]   		- This is an adjacency list for the maze
			path[1][2] 			- Your function will return the optimal mini-max move in this array.
			minmax_cost[size_X][size_Y]	- An array in which your code will store the
							  minimax value for maze locations expanded by
							  the search *when called for the mouse, not
							  for the cats!*

							  This array will be used to provide a visual 
							  display of minimax values during the game.

			cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
						 but there can be fewer). Only valid cat locations are 0 to (cats-1)
			cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
						     but possibly fewer). Valid locations are 0 to (cheeses-1)
			mouse_loc[1][2] - Mouse location - there can be only one!
			mode - Search mode selection:
						mode = 0 	- No alpha-beta pruning
						mode = 1	- Alpha-beta pruning

			(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
					- This is a pointer to the utility function which returns a value for a specific game configuration

					   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 					 that requires knowledge of the maze for computing the utility values.

					  * How to call the utility function from within this function : *
						- Like any other function:
							u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
							
			agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
	                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
	                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
		
			depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
			
			maxDepth: maximum desired search depth - once reached, your code should somehow return
				  a minimax utility value for this location.

			alpha. beta: alpha and beta values passed from the parent node to constrain search at this
				     level.

	   Return values:
			Your search code will directly update data passed-in as arguments:
			
			- Mini-Max value	: Notice this function returns a double precision number. This is
						  the minimax value at this level of the tree. It will be used 
						  as the recursion backtracks filling-in the mini-max values back
						  from the leaves to the root of the search tree. 

			- path[1][2]		: Your MiniMax function will return the location for the agent's 
						  next location (i.e. the optimal move for the agent). 
			- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
							   minimax value for locations that were expanded during
							   the search. This must be done *only* for the mouse.

							   Values in this array will be in the range returned by
							   your utility function.

			* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
		     	  or the location of the mouse - if you try, the driver code will know it *
				
			That's that, now, implement your solution!
	 */

	 /********************************************************************************************************
	 * 
	 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
	 *              expansion. Once nodes below return values, your function will propagate minimax utilities
	 *		as per the minimax algorithm.
	 *	
	 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
	 *		along the path.
	 *
	 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
	 *		file and explain in your code why they are needed and how they are used.
	 *
	 *		Recursion should appear somewhere.
	 *
	 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
	 *		in the maze, you must update minmax_cost[][] for that location.
	 *
	 *		How you design your solution is up to you. But:
	 *
	 *		- Document your implementation by adding concise and clear comments in this file
	 *		- Document your design (how you implemented the solution, and why) in the report
	 *
	 ********************************************************************************************************/

 // // Stub so that the code compiles/runs - This will be removed and replaced by your code!
 // path[0][0]=mouse_loc[0][0];
 // path[0][1]=mouse_loc[0][1];
 // return(0.0);
    int util;
    int child_count = 0;
    int child_list[4][2];
    int my_path[4][2];
    int child_utilities[4];
    int max_util, min_util, best_utility;
	int best_c_index;

    int cur_x, cur_y, cur_index;
    int cur_cat;

    int my_mouse_location[1][2], my_cat_location[10][2];

    //initialization: duplicate current positions as local game state
	my_mouse_location[0][0] = mouse_loc[0][0];
	my_mouse_location[0][1] = mouse_loc[0][1];
	for (int i=0; i< cats; i++) {	
		// fprintf(stderr, "cat location at %d is (%d, %d)\n", i, cat_loc[i][0], cat_loc[i][1]);
		my_cat_location[i][0] = cat_loc[i][0];
		my_cat_location[i][1] = cat_loc[i][1];
	}
	// set top level position
    if (agentId == 0) {
    	cur_x = mouse_loc[0][0];
    	cur_y = mouse_loc[0][1];
    } else {
    	cur_cat = (agentId-1) % cats;
    	cur_x = cat_loc[cur_cat][0];
    	cur_y = cat_loc[cur_cat][1];
    }
 	cur_index = cur_x + (cur_y*size_X);

	// if (depth == maxDepth)
		// printf("at depth %d\n", depth);
    // if the current move results in a terminal configuration or max search depth has been reached
    // call utility functin to get a value and 
    // TODO update the path?
	if ((checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)) || (depth == maxDepth)) {

		// printf("mouse location (%d, %d)\ncheese location location (%d, %d)\ncat location (%d, %d)\n",
		// 		mouse_loc[0][0], mouse_loc[0][1], cheese_loc[0][0], cheese_loc[0][1],
		// 		cat_loc[0][0], cheese_loc[0][1]);
		// printf("in terminal node at depth %d\n", depth);
		best_utility = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
		// fprintf(stderr, "current utility at (%d, %d) is %d\n", cur_x, cur_y, best_utility);

		path[0][0] = cur_x;
		path[0][1] = cur_y;
		return best_utility;
	} else { // otherwise call MiniMax recursively to find out what happend deeper into the tree
		
		// find all available successors
		int nghb_x_trbl[4] = {0, 1, 0, -1};
		int nghb_y_trbl[4] = {-1, 0, 1, 0};
		
		for (int i=0; i<4; i++) {
			if (gr[cur_index][i]) { // TODO check for cats and visited?
				child_list[child_count][0] = cur_x + nghb_x_trbl[i];
				child_list[child_count][1] = cur_y + nghb_y_trbl[i];
				// printf("added %dth children (%d, %d)\n", child_count, child_list[child_count][0], child_list[child_count][1]);
				child_count++;
			}
		}

		// max_util = -99999;
		// min_util = 99999;

		// ah! increment depth and agentId out of the loop...
		int next_agendID, next_depth;
		next_agendID = (agentId+1) % (cats+1);
		next_depth = depth+1;
		for (int c=0; c < child_count; c++) {
			// printf("check %dth children (%d, %d)\n", c, child_list[c][0], child_list[c][1]);
			if (agentId == 0) {
				my_mouse_location[0][0] = child_list[c][0];
		    	my_mouse_location[0][1] = child_list[c][1];
		    } else {
		    	cur_cat = (agentId-1) % cats;
		    	my_cat_location[cur_cat][0] = child_list[c][0];
		    	my_cat_location[cur_cat][1] = child_list[c][1];
		    }
    // 		printf("when makeing the call for agent %d at depth %d:\nmouse location (%d, %d)\ncat location (%d, %d)\n",
				// next_agendID, next_depth, my_mouse_location[0][0], my_mouse_location[0][1],
				// my_cat_location[0][0], my_cat_location[0][1]);
			// printf("after incremented at depth %d\n", depth);
			util = MiniMax(gr, &my_path[c], minmax_cost, my_cat_location, cats, cheese_loc, cheeses, 
				my_mouse_location, mode, utility, next_agendID, next_depth, maxDepth, alpha, beta);
			// if (util > max_util) max_util = util;
			// if (util < min_util) min_util = util;
			// fprintf(stderr, "get utility %d at depth %d for agent %d\n", util, depth, agentId);
			child_utilities[c] = util;
		}

		// if current exploring next moves for mouse, find the max child utility
		if (agentId == 0) {
			// find the best next move
			best_utility = -99999;
			for (int c=0; c < child_count; c++) {
				if (child_utilities[c] > best_utility) {
					best_utility = child_utilities[c];
					best_c_index = c;
				}
			}
			// update the minmax_cost array since the location is expanded for the mouse
			minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = best_utility;
		// otherwise find the min child utility
		} else {
			cur_cat = (agentId-1) % cats;
			// printf("current agent is %d with actual id %d\n", cur_cat, agentId);
			// find the best next move
			best_utility = 99999;
			for (int c=0; c < child_count; c++) {
				if (child_utilities[c] < best_utility) {
					best_utility = child_utilities[c];
					best_c_index = c;
				}
			}			
		}
	
		if (depth == 0) {
			fprintf(stderr, "agentid is %d, currently at (%d, %d) at depth %d\n", agentId, 
				cur_x, cur_y, depth);
			for (int c=0; c < child_count; c++) {
				fprintf(stderr, "child %d with utility %d at position (%d, %d)\n", c, child_utilities[c], child_list[c][0], child_list[c][1]);
			}
			fprintf(stderr, "agentid is %d, best at position (%d, %d) with utillity %d\n\n", agentId, 
				child_list[best_c_index][0], child_list[best_c_index][1], child_utilities[best_c_index]);
		}


		// update the next move for the mouse (NOT either in max round or min round)
		if (depth == 0) {
			path[0][0] = child_list[best_c_index][0];
			path[0][1] = child_list[best_c_index][1];
		}			
		
	}

	return best_utility;
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */
	
	// mahatton distance to cheese - mahatton distance to cat
		int dist_cheeses = 0;
		int dist_cats = 0;
		for (int i=0; i<cats; i++) {
			// printf("abs(mouse_loc[0][0] - cat_loc[i][0]) = %d\n", abs(mouse_loc[0][0] - cat_loc[i][0]));
			// printf("abs(mouse_loc[0][1] - cat_loc[i][1]) = %d\n", abs(mouse_loc[0][1] - cat_loc[i][1]));
			dist_cats = dist_cats + abs(mouse_loc[0][0] - cat_loc[i][0]) + abs(mouse_loc[0][1] - cat_loc[i][1]);
			// fprintf(stderr, "dist_cats = %d\n", dist_cats);
		}

		for (int j=0; j<cheeses; j++){
			dist_cheeses = dist_cheeses + abs(mouse_loc[0][0] - cheese_loc[j][0]) + abs(mouse_loc[0][1] - cheese_loc[j][1]);
			// printf("dist_cheese = %d\n", dist_cheeses);
		}
		// fprintf(stderr, "current utility at (%d, %d) is %d\n", mouse_loc[0][0], mouse_loc[0][1], dist_cheeses*10 - dist_cats);
		return dist_cheeses*10 - dist_cats;
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /* 
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}

