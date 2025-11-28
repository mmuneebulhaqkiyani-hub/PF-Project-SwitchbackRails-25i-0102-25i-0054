#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>


// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

// Storage for planned moves (for collisions).

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
     
      for (int i=0;i<g_numtrains;i++) 
    {
        // if train  not on
        if (!g_trainactive[i]) 
        {
            // check if its spawn tick matches the current tick
            if (g_trainSpawnTick[i]==g_currentTickNum) 
            {
                // activate train
                g_trainactive[i]=true;

                // at spawn both will be same they will be udated later
                g_pastTrainX[i]=g_trainX[i];
                g_pastTrainY[i]=g_trainY[i];

                g_futureTrainX[i]=g_trainX[i];
                g_futureTrainY[i]= g_trainY[i];
                g_futureTrainActive[i]= true;
            }
        }
    }

}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------

bool determineNextPosition() {
    
    bool allValid=true;
    for (int i=0;i< g_numtrains;i++)
    {
        if (!g_trainactive[i]) 
        {
            continue;
        }
        int nextX=g_trainX[i];


        int nextY=g_trainY[i];
        // direction is integer: 0=up,1=right,2=down,3=left
        if (g_traindirection[i]==0) 
        {
            //up
           
            nextY--;
        } 
        else if (g_traindirection[i]==1) 
        {   
            //rigght
            nextX++;
        } 
        else if (g_traindirection[i]==2)
         
        {   
            
            //down
            nextY++;
        } 
        
        else if (g_traindirection[i] == 3) 
        {   
            
            //lft
            nextX--;
        }

        //ts gonna check if that next tile is valid
        if (!isInBounds(nextX,nextY)||!isTrackTile(g_grid[nextY][nextX])) 
        
        {
             allValid=false;
             //this will make g_trainactive[i] = false;
        }


        
    return true; //this is gonna be placeholder for now we will move to thi later
}
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection() {
      return 0; // placeholder
}


// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing() {
    return 0; // placeholder
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {

    for (int i=0;i<g_numtrains;i++) //ts will checks for all trains
    {
        if (!g_trainactive[i]) {
            continue; // skip inactive trains
        }
        int dx,dy;
        getDeltaForDirection(g_traindirection[i], dx, dy);
        int newx=g_trainX[i] + dx;
        int newy=g_trainY[i] + dy;
        if (newx<0 || newx>=g_columns || newy<0 || newy>=g_rows) {
            //ts means train outt of bounds 
            g_trainactive[i] = false;//ts suld deactivate the train
            continue;
        }
        g_trainX[i]=newx;
        g_trainY[i]=newy;
    }
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
}

// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {
}
