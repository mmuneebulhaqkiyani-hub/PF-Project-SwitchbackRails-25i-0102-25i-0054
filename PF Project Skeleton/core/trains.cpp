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
//4/10 ----------------------------------------------------------------------------
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
    
    bool allValidcheck=true;
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
             allValidcheck=false;
             //this will make g_trainactive[i] = false;
        }
 return allValidcheck;
}
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int currentDir,char tile) {
     //ths - is for left right movement only 
    if (tile== '-') 
    {
        if (currentDir==1||currentDir==3) 
        {
            return currentDir;//ts will make the train thing to keep going horizontally
        }
        return -1;//cannot move vertically on '-'
    }
    //ts | is for up downnmovement thing only
    if (tile=='|') 
    {
        if (currentDir==0||currentDir==2) 
        {
            return currentDir;//keep going vertically
        }
        return -1;//cannot move sideways on '|'
    }
    //this / is for saying the train to move in curve way
    if (tile =='/')
    {
        if (currentDir==0) 
        
        return 1;// up ====> right
        if (currentDir==1)
         return 0;// right --> up
        if (currentDir==2) 
        return 3;// down --> left
        if (currentDir==3) 
        return 2;// left -> down

        return -1;
    }

    // now fr this \ thing btw we have to use double \ bcz its escape characterthing
    if (tile == '\\')
    {
        if (currentDir==0) 
        return 3;// up --> left
        if (currentDir==3) 
        return 0;// left -> up
        if (currentDir==2) 
        return 1;// down ---> right
        if (currentDir == 1) 
        return 2;// right ==> down

        return -1;
    }

    // the + will make the train just go ahead in the same direction
    if (tile == '+')
    {
        return currentDir;
    }
    //the A to Z thing is for switvches thing
    // For now just keep direction (switch logic gonna add ater)
    if (tile>='A'&&tile<='Z')
    {
        return currentDir;
    }
  //these aint gonna change the direction so just keep it as it is ig
    if (tile=='S'||tile=='D'||tile=='=')
    {
        return currentDir;
    }

    return -1;
      
}


// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int currentDir, int x, int y, int finalgoalX, int finaLgoalY) 
{ 
    int bestDir=-1;
    int bestDist=99999999999;  
      for (int dir=0;dir<4;dir++)
    {
        int newx=x;
        int newy =y;

        //compute where this direction leads
        if (dir==0) 
        {
            newy=newy-1;   //THIS FOR UP
        } else if (dir==1) 
        {
            nx=nx+1;   //this is for right
        } else if (dir==2) 
        {
            newy= newy+1;   //this is for down
        } else if (dir==3) 
        {
            newx=newx-1;   //this for left
        }

        //check if the tile exists and is track
        if (!isInBounds(newx,newy)) 
        {
            continue;
        }
        if (!isTrackTile(g_grid[newy][newx])) 
        {
            continue;
        }

        //Manhattan distance
        int dx =newx-goalX;
        if (dx< 0) 
        dx =-dx;

        int dy = newy-goalY;
        if (dy <0) 
        dy = -dy;

        int dist=dx+dy;

        //pick smallest
        if (dist<bestDist)
        {
            bestDist=dist;
            bestDir=dir;
        }
    }

    return bestDir;   // -1 if none valid
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
