/*
	CSC D84 - Unit 3 - Reinforcement Learning
	
	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.
	
	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
 /*
   This function implementes the Q-Learning update as stated in Lecture. It 
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.
   
   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table
     
   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not 
   have to change their values. Use them as they are.
     
   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */
 
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  double prev_Q, new_Q, max_new_Q;
  
  prev_Q = *(QTable+(4*s)+a);
  // printf("previous Q entry is %f\n", prev_Q);
  
  // find max_{a'}(QTable(s', a'))
  max_new_Q = -99999;
  for (int i=0; i<4; i++){
      new_Q = *(QTable+(4*s_new)+i);
      if (new_Q > max_new_Q) {
        max_new_Q = new_Q;
      }
  }
  // fprintf(stderr, "max_new_Q is %f\n", max_new_Q);

  *(QTable+(4*s)+a) = prev_Q + alpha * ((r + lambda * max_new_Q) - prev_Q);
  
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
      This function decides the action the mouse will take. It receives as inputs
      - The graph - so you can check for walls! The mouse must never move through walls
      - The mouse position
      - The cat position
      - The chees position
      - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.
       
      Remember that the training process involves random exploration initially, but as training
      proceeds we use the QTable more and more, in order to improve our QTable values around promising
      actions.

      The value of pct is controlled by QLearn_core_GL, and increases with each round of training.

      This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left
        
      QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
      the mouse leave the map - this should not happen. If you see a warning, fix the code in this
      function!
       
      The Q-table has been pre-allocated and initialized to 0. The Q-table has
      a size of

          graph_size^3 x 4
          
      This is because the table requires one entry for each possible state, and
      the state is comprised of the position of the mouse, cat, and cheese. 
      Since each of these agents can be in one of graph_size positions, all
      possible combinations yield graph_size^3 states.

      Now, for each state, the mouse has up to 4 possible moves (up, right,
      down, and left). We ignore here the fact that some moves are not possible
      from some states (due to walls) - it is up to the QLearn_action() function
      to make sure the mouse never crosses a wall. 

      So all in all, you have a big table.
          
      For example, on an 8x8 maze, the Q-table will have a size of

         64^3 x 4  entries
         
         with 
         
         size_X = 8		<--- size of one side of the maze
         graph_size = 64		<--- Total number of nodes in the graph
         
      Indexing within the Q-table works as follows:

       say the mouse is at   i,j
           the cat is at     k,l
           the cheese is at  m,n
           
       state = (i+(j*size_X)) * ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
       ** Make sure you undestand the state encoding above!
       
       Entries in the Q-table for this state are

       *(QTable+(4*state)+a)      <-- here a is the action in [0,3]
       
       (yes, it's a linear array, no shorcuts with brackets!)
       
       NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
      */
    
    /***********************************************************************************************
     * TO DO: Complete this function
     ***********************************************************************************************/   
  // int i, j, k, l, m, n, cur_index, cur_state;
  int cur_index, cur_state;
  int next_move_count=0;
  int possible_moves[4];
  double max_Q, new_Q;
  int ideal_a;

  // i = mouse_pos[0][0];
  // j = mouse_pos[0][1];
  // get the current state index in the Qtable
  cur_state = get_state(mouse_pos, cats, cheeses, size_X, graph_size);

  // found possible moves for mouse (i.e not a wall)
  // cur_index = i + (j*size_X);
  cur_index = get_index(mouse_pos, size_X);
  for (int c=0; c<4; c++){
    if (gr[cur_index][c]==1) {
      possible_moves[next_move_count] = c;
      next_move_count++;
    }
  }
  // fprintf(stderr, "next move count is %d at (%d, %d)\n", next_move_count, mouse_pos[0][0], mouse_pos[0][1]);

  double rdm;
  rdm = rand_percent();
  // fprintf(stderr, "ramdom %f vs pct % \n", rdm, pct);
  // for pct amount of time find the optimal move by QTable
  if (rdm < pct){
    max_Q = -99999;
    for (int c=0; c<next_move_count; c++){ // TODO: check if upperbound should be 4 or not
      new_Q = *(QTable+(4*cur_state)+possible_moves[c]);
      if (new_Q > max_Q) {
        max_Q = new_Q;
        ideal_a = possible_moves[c];
      }
    }  
    // fprintf(stderr, "ideal_a at (%d, %d) is %d with Q=%f\n", mouse_pos[0][0],mouse_pos[0][1], ideal_a, max_Q);
    // fprintf(stderr, "is it a wall? %f\n", gr[cur_index][ideal_a]);

  // rest of the time choose randomly from available move  
  } else {
    // fprintf(stderr, "moving randomly\n");
    ideal_a = possible_moves[rand()%next_move_count];

  }
  return ideal_a;
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position. 
    
    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the 
    mouse.
    
    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward. 
        
    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.      
   */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/ 
    int mouse_cat_dist, mouse_cheese_dist;
    int cur_index, ngbr_count;
    double reward;

    mouse_cat_dist = manhattan_dist(mouse_pos[0][0], mouse_pos[0][1], cats[0][0], cats[0][1]);
    mouse_cheese_dist = manhattan_dist(mouse_pos[0][0], mouse_pos[0][1], cheeses[0][0], cheeses[0][1]);

    reward = mouse_cat_dist - mouse_cheese_dist;

    if (mouse_pos[0][0] == cats[0][0] && mouse_pos[0][1] == cats[0][1]){
      reward = reward - 100;  
    }

    if (mouse_pos[0][0] == cheeses[0][0] && mouse_pos[0][1] == cheeses[0][1]){
      reward = reward + 100;  
    }

    // punish slightly if at deadend (choose it when cheese is at the deadend)
    cur_index = get_index(mouse_pos, size_X);
    for (int i=0; i<4; i++) {
      if (gr[cur_index][i]) {
        ngbr_count++;
      }
    }
    if (ngbr_count == 1) {
      reward = reward - 50;
    }
   // fprintf(stderr, "reward at (%d, %d) is %f\n", mouse_pos[0][0], mouse_pos[0][1], reward);

    return reward;
}

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward 
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)
    
    Your code must then evaluate the update and apply it to the weights in the weight array.    
   */
  
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
    double prev_weight;
    double cur_Q, max_new_Q;
    int ideal_a;
    double features[25];

    evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
    cur_Q = Qsa(weights, features);
    // fprintf(stderr, "updating: cur_Q %f\n", cur_Q);

    // get max_{s'}(Q(s')) assigned to max_new_Q
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &max_new_Q, &ideal_a);
    // fprintf(stderr, "updating: got max_Q %f\n", max_new_Q);

    
    for (int i=0; i< numFeatures; i++) {
      prev_weight = weights[i];
      weights[i] = prev_weight + alpha * ( (reward + lambda*max_new_Q) - cur_Q) * features[i];
      // fprintf(stderr, "updating w %i from %f to %f\n", i, prev_weight, weights[i]);
    }

    // fprintf(stderr, "updating end...\n");
    return;
}

int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size)
{
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.
    
    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.
    
    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.
    
    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.    
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
  // fprintf(stderr, "#########taking actions...\n");
  int cur_index, cur_state;
  int next_move_count=0;
  int possible_moves[4];
  double max_Q, new_Q;
  int ideal_a;

  cur_index = get_index(mouse_pos, size_X);
  for (int c=0; c<4; c++){
    if (gr[cur_index][c]==1) {
      possible_moves[next_move_count] = c;
      next_move_count++;
    }
  }

  double rdm;
  rdm = rand_percent();
    // fprintf(stderr, "ramdom %f vs pct %f \n", rdm, pct);
  if (rdm < pct){
    // choose max_Q
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &max_Q, &ideal_a);
    // fprintf(stderr, "taking action: got max_Q %f\n", max_Q);

  } else{
    // choose randomly
    ideal_a = possible_moves[rand()%next_move_count];
    // fprintf(stderr, "moving randomly to %d\n", ideal_a);
  }  

  return ideal_a;
}

void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.
   
   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!
   
   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.
   
   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/      
   
    // feature 0: distance to closest cheese
    int closest_cheese_dist, mouse_cheese_dist;
    int i=0;

    closest_cheese_dist = 999;
    // hmm, can only escape case where first cheese is not at (0, 0)
    while (!(cheeses[i][0] == 0 && cheeses[i][1] == 0) || i == 0) {
      // fprintf(stderr, "cheeses : (%d, %d) \n", cheeses[i][0], cheeses[i][1]);
      mouse_cheese_dist = manhattan_dist(mouse_pos[0][0], mouse_pos[0][1], cheeses[i][0], cheeses[i][1]);
      if (mouse_cheese_dist < closest_cheese_dist) {
        closest_cheese_dist = mouse_cheese_dist;
      }
      i++;
    }
    // in favour of smaller cheese dist
    features[0] = (size_X*2 - closest_cheese_dist) / (float)(size_X*2);

    // feature 1: if there are cats around in 2 manhantton distances
    int closest_cat_dist, mouse_cat_dist;
    
    i=0;
    closest_cat_dist = 999;
    while (!(cats[i][0] == 0 && cats[i][1] == 0) || i==0) {
      mouse_cat_dist = manhattan_dist(mouse_pos[0][0], mouse_pos[0][1], cats[i][0], cats[i][1]);
      // fprintf(stderr, "cats : (%d, %d) with dist %d when mouse at  (%d, %d)\n", cats[i][0], cats[i][1], mouse_cat_dist, mouse_pos[0][0], mouse_pos[0][1]);
      if (mouse_cat_dist < closest_cat_dist) {
        closest_cat_dist = mouse_cat_dist;
      }
      i++;
    }
    features[1] = closest_cat_dist / (float)(size_X*2);

    // feature 2: if mouse is in a deadend
    int cur_index, ngbr_count;
    cur_index = get_index(mouse_pos, size_X);
    for (int c=0; c<4; c++) {
      if (gr[cur_index][c]) {
        ngbr_count++;
      }
    }
    
    features[2] = (ngbr_count - 1) *3;

    // fprintf(stderr, "frature 0, 1 and 2 is %f, %f, %f\n", features[0], features[1], features[2]);
}

double Qsa(double weights[25], double features[25])
{
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
  * TO DO: Complete this function
  ***********************************************************************************************/  
  
  // return(0);		// <--- stub! compute and return the Qsa value
  double cur_Q=0.0;

  
  for (int i=0; i< numFeatures; i++){
    // fprintf(stderr, "for %d weight= %f and feature = %f\n", i, weights[i], features[i]);
    cur_Q += weights[i]*features[i];
  }

  // fprintf(stderr, "evaluated Q is %f\n", cur_Q);
  return cur_Q;
}

void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA)
{
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.
   
   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall. 
  */
 
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  
 
  int cur_state, cur_index; 
  int next_x, next_y, next_state;
  int next_mouse_pos[0][2];
  int possible_moves[4];
  int next_move_count=0;
  double max_Q, next_Q;
  int max_a=0;

  double features[25];

  cur_index = get_index(mouse_pos, size_X);
  // fprintf(stderr, "in maxQsa\n");

  for (int c=0; c<4; c++){
    if (gr[cur_index][c]==1) {
      possible_moves[next_move_count] = c;
      next_move_count++;
    }
  }

  int nghb_x_trbl[4] = {0, 1, 0, -1};
  int nghb_y_trbl[4] = {-1, 0, 1, 0};

  // fprintf(stderr, "mouse at (%d, %d) with %d nbgrs \n", mouse_pos[0][0], mouse_pos[0][1], next_move_count);
  max_Q = -99999;
  for (int c=0; c<next_move_count; c++){
    next_x = mouse_pos[0][0] + nghb_x_trbl[possible_moves[c]];
    next_y = mouse_pos[0][1] + nghb_y_trbl[possible_moves[c]];
    next_mouse_pos[0][0] = next_x;
    next_mouse_pos[0][1] = next_y;

    // get the evaluated features for the neighbour state
    evaluateFeatures(gr, features, next_mouse_pos, cats, cheeses, size_X, graph_size);
    
    // get the actual Q value given the state and obtained features
    next_Q = Qsa(weights, features);
    // fprintf(stderr, "getting Q %f\n", next_Q);
    if (next_Q > max_Q) {
      max_Q = next_Q;
      max_a = possible_moves[c];
    }
  }

  *maxU = max_Q;
  *maxA = max_a;
  return;
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below 
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/
int get_state(int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
  int i, j, k, l, m, n;
  
  i = mouse_pos[0][0];
  j = mouse_pos[0][1];
  k = cats[0][0];
  l = cats[0][1];
  m = cheeses[0][0];
  n = cheeses[0][1];
  return (i+(j*size_X)) + 
          ((k+(l*size_X))*graph_size) + 
          ((m+(n*size_X))*graph_size*graph_size);
}

int get_index(int mouse_pos[1][2], int size_X){

  return mouse_pos[0][0] + (mouse_pos[0][1]*size_X);
}

double rand_percent(){
    // return (double)rand() / (double)RAND_MAX ;
  return drand48();
}

int manhattan_dist(int x1, int y1, int x2, int y2){
  return abs(x1 - x2) + abs(y1 - y2);
}

void expensiveFeature1(double gr[max_graph_size][4], int path[max_graph_size][2],
                        int start_x, int start_y, 
                        int (*goalCheck)(int x, int y, int pos[5][2]), 
                        int pos[5][2], int s_type, int *l, int size_X){

}
int checkForGoal(int x, int y, int pos[5][2]){

}
