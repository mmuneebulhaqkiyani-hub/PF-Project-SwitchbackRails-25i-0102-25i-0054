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

static void markTrainCrash(int idx) {
    if (idx < 0 || idx >= MaxTrains) {
        return;
    }
    if (g_futureTrainActive[idx]) {
        g_futureTrainActive[idx] = false;
        g_trainactive[idx] = false;
        g_trainsCrashed++;
    }
}

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
//4/10 ----------------------------------------------------------------------------
void spawnTrainsForTick() {

    for (int i=0;i<g_numtrains;i++)
    {
        if (g_trainactive[i])
        {
            continue;
        }

        if (g_trainSpawnTick[i]!=g_currentTickNum)
        {
            continue;
        }

        if (!isInBounds(g_trainX[i], g_trainY[i]) || !isSpawnPoint(g_trainX[i], g_trainY[i]))
        {
            if (g_numspawns > 0)
            {
                int pick = i % g_numspawns;
                g_trainX[i] = g_spawnX[pick];
                g_trainY[i] = g_spawnY[pick];
            }
        }

        bool blocked=false;
        for (int j=0;j<g_numtrains;j++)
        {
            if (!g_trainactive[j])
            {
                continue;
            }
            if (g_trainX[j]==g_trainX[i] && g_trainY[j]==g_trainY[i])
            {
                blocked=true;
                break;
            }
        }

        if (blocked)
        {
            // try again next tick if the spawn is occupied
            g_trainSpawnTick[i]=g_currentTickNum+1;
            continue;
        }

        g_trainactive[i]=true;

        // at spawn both will be same they will be udated later
        g_pastTrainX[i]=g_trainX[i];
        g_pastTrainY[i]=g_trainY[i];

        g_futureTrainX[i]=g_trainX[i];
        g_futureTrainY[i]= g_trainY[i];
        g_futureTrainActive[i]= true;
    }

}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------

bool determineNextPosition() {

    bool allValidcheck = true;
    for (int i = 0; i < g_numtrains; i++)
    {
        g_futureTrainActive[i] = false;
        if (!g_trainactive[i])
        {
            continue;
        }

        // store where the train was before planning this tick
        g_pastTrainX[i] = g_trainX[i];
        g_pastTrainY[i] = g_trainY[i];

        int currentX = g_trainX[i];
        int currentY = g_trainY[i];

        g_futureTrainX[i] = currentX;
        g_futureTrainY[i] = currentY;
        g_futureTrainActive[i] = true;

        char tile = g_grid[currentY][currentX];

        if (tile == '=') {
            if (g_safetyDelay[i] == 0)
            {
                g_safetyDelay[i] = 1;
                continue;
            }
            else
            {
                g_safetyDelay[i] = 0;
            }
        }

        int nextDirection = -1;
        if (tile == '+')
        {
            int goalX = g_traindestinationX[i];
            int goalY = g_traindestinationY[i];
            nextDirection = getSmartDirectionAtCrossing(g_traindirection[i], currentX, currentY, goalX, goalY);
        }
        else if (isSwitchTile(tile))
        {
            int idx = getSwitchIndex(tile);
            int state = (idx >= 0 && idx < Maxswitches) ? g_switchState[idx] : 0;
            if (state == 0) {
                nextDirection = g_traindirection[i];
            } else {
                // simple turn: rotate 90 degrees to the right for deterministic behaviour
                nextDirection = (g_traindirection[i] + 1) % 4;
            }
        }
        else
        {
            nextDirection = getNextDirection(g_traindirection[i], tile);
        }

        if (nextDirection == -1)
        {
            allValidcheck = false;
            markTrainCrash(i);
            continue;
        }

        // store new direction now so collisions know the intent
        g_traindirection[i] = nextDirection;

        int nx = currentX;
        int ny = currentY;
        if (nextDirection == 0)
        {
            ny = ny - 1;
        }
        else if (nextDirection == 1)
        {
            nx = nx + 1;
        }
        else if (nextDirection == 2)
        {
            ny = ny + 1;
        }
        else if (nextDirection == 3)
        {
            nx = nx - 1;
        }

        if (!isInBounds(nx, ny) || !isTrackTile(g_grid[ny][nx]))
        {
            allValidcheck = false;
            markTrainCrash(i);
            continue;
        }

        g_futureTrainX[i] = nx;
        g_futureTrainY[i] = ny;
    }
    return allValidcheck;
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int currentDir,char tile) {
     //ths - is for left right movement only 
    if (tile == '-') 
    {
        if (currentDir==1||currentDir==3) 
        {
            return currentDir;//ts will make the train thing to keep going horizontally
        }
        return -1;//cannot move vertically on '-'
    }
    //ts | is for up downnmovement thing only
    if (tile == '|') 
    {
        if (currentDir==0||currentDir==2) 
        {
            return currentDir;//keep going vertically
        }
        return -1;//cannot move sideways on '|'
    }
    //this / is for saying the train to move in curve way
    if (tile == '/')
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

int getSmartDirectionAtCrossing(int currentDir, int x, int y, int finalgoalX, int finalgoalY) 
{ 
    int bestDir=currentDir;
    int bestDist=99999;  
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
            newx=newx+1;   //this is for right
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
        int dx =newx-finalgoalX;
        if (dx< 0) 
        dx =-dx;

        int dy = newy-finalgoalY;
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
     //if no valid direction return -1
    if (bestDist==99999)
    {
        return -1;
    }

    return bestDir;//INVALID
}




// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
    //this function is for determining the route for all trains
      if (g_numdestinations<=0) 
    {
        for (int i=0;i<g_numtrains;i++) 
        {
            g_traindestinationX[i]=g_trainX[i];



        
            g_traindestinationY[i]=g_trainY[i];
        }
        return;
    }

    // For each train, pick the nearest destination
    for (int i=0;i<g_numtrains; i++) 
    {
        int trainX=g_trainX[i];
        int trainY=g_trainY[i];


        int bestIndex=0;
        int bestDist =99999; 
        // Check all destinations
        for (int j=0;j<g_numdestinations;j++) 
        {
            int destinationX=g_destinationX[j];
            int destinationY=g_destinationY[j];
            // Manhattan distance
            int dx=trainX-destinationX;
            if (dx<0)dx=-dx;
            int dy=trainY-destinationY;
            if (dy<0)
            dy=-dy;

            int dist=dx+dy;


           if (dist<bestDist) 
            {
                bestDist=dist;
                bestIndex=j;
            }
        }

        // ts gonna assign the closest destination to this train
        g_traindestinationX[i]=g_destinationX[bestIndex];
        g_traindestinationY[i]=g_destinationY[bestIndex];
    }
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
    for (int i=0;i<g_numtrains;i++)
    {
        if (!g_trainactive[i])
        {
            continue;//ts gonn skip inactive trains
        }
        if (!g_futureTrainActive[i])
        {
            g_trainactive[i] = false;
            continue;
        }

        g_trainX[i] = g_futureTrainX[i];
        g_trainY[i] = g_futureTrainY[i];

    }


}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
    for (int i=0;i<g_numtrains;i++) 
    {
        if(!g_trainactive[i]) 
        {
            continue;
        }
        if(!g_futureTrainActive[i]) 
        {
            continue;
        }

        for (int j=i +1;j <g_numtrains;j++) 
        {
            if(!g_trainactive[j]) 
            {
                continue;
            }
            if (!g_futureTrainActive[j]) 
            {
                continue;
            }

            //SAMESPOT COLLISION
            //Both trains plan to go to the same tile
          
            if (g_futureTrainX[i]==g_futureTrainX[j] &&
                g_futureTrainY[i]==g_futureTrainY[j])
            {
                // compute Manhattan distance for future positions
                int dfx_i=g_futureTrainX[i]-g_traindestinationX[i];
                
                if (dfx_i<0) 
                {
                    dfx_i=-dfx_i;
                }
                int dfy_i=g_futureTrainY[i]-g_traindestinationY[i];
                if (dfy_i <0) 
                {
                    dfy_i=-dfy_i;
                }
                int dist_i=dfx_i + dfy_i;

                int dfx_j=g_futureTrainX[j]-g_traindestinationX[j];
                if (dfx_j<0) 
                {
                    dfx_j=-dfx_j;
                }
                int dfy_j=g_futureTrainY[j]-g_traindestinationY[j];
                if (dfy_j<0) 
                {
                    dfy_j=-dfy_j;
                }
                int dist_j=dfx_j+dfy_j;

                if (dist_i>dist_j)
                {
                    //train 1 is farther from its destination So higher priority
                    // i moves, j must WAITs
                    g_futureTrainX[j]=g_trainX[j];
                    g_futureTrainY[j]=g_trainY[j];
                    // j remains active (no crash), just doesn't move this tick
                }
                else if (dist_j>dist_i)
                {
                    // train 2 is farther so higher priority
                    g_futureTrainX[i] =g_trainX[i];
                    g_futureTrainY[i]=g_trainY[i];
                }
                else
                {
                    // equal distance both crash
                    markTrainCrash(i);
                    markTrainCrash(j);
                }

                
                continue;
            }

           
            //HEADON SWAP COLLISION                  
            bool i_hits_j = (g_futureTrainX[i]==g_trainX[j] &&
                             g_futureTrainY[i]==g_trainY[j]);
            bool j_hits_i = (g_futureTrainX[j]==g_trainX[i] &&
                             g_futureTrainY[j]==g_trainY[i]);

            if (i_hits_j && j_hits_i)
            {
                //compute Manhattan distance again
                int dx_i = g_futureTrainX[i] -g_traindestinationX[i];
                if (dx_i<0) 
                {
                    dx_i=-dx_i;
                }
                int dy_i = g_futureTrainY[i] -g_traindestinationY[i];
                if (dy_i<0) 
                {
                    dy_i=-dy_i;
                }
                int dist_i=dx_i+dy_i;

                int dx_j=g_futureTrainX[j]- g_traindestinationX[j];
                if (dx_j<0) 
                {
                    dx_j=-dx_j;
                }
                int dy_j=g_futureTrainY[j] -g_traindestinationY[j];
                if (dy_j<0) 
                {
                    dy_j=-dy_j;
                }
                int dist_j=dx_j+dy_j;

                if (dist_i>dist_j)
                {
                    //same as before t1 farther
                    g_futureTrainX[j]=g_trainX[j];
                    g_futureTrainY[j]=g_trainY[j];
                }
                else if (dist_j>dist_i)
                {
                    //t2 farther
                    g_futureTrainX[i] =g_trainX[i];
                    g_futureTrainY[i] =g_trainY[i];
                }
                else
                {
                    //equal distance  both crash
                    markTrainCrash(i);
                    markTrainCrash(j);
                }
            }
        }
    }

}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
    for (int i=0;i<g_numtrains;i++) 
    {
        if (!g_trainactive[i]) 
        {
            continue;
        }
        if (g_trainX[i]==g_traindestinationX[i] && g_trainY[i]==g_traindestinationY[i])
        {
            g_trainactive[i]=false;   // train arrived so despawn
            g_trainsArrived++;       //count arrivals 
        }
    }
  
}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
    if (!g_emergencyActive || g_emergencyTicksRemaining <= 0)
    {
        return;
    }

    int minX = g_emergencyCenterX - 1;
    int maxX = g_emergencyCenterX + 1;
    int minY = g_emergencyCenterY - 1;
    int maxY = g_emergencyCenterY + 1;

    for (int i = 0; i < g_numtrains; i++)
    {
        if (!g_trainactive[i])
        {
            continue;
        }

        if (!g_futureTrainActive[i])
        {
            continue;
        }

        int fx = g_futureTrainX[i];
        int fy = g_futureTrainY[i];
        if (fx >= minX && fx <= maxX && fy >= minY && fy <= maxY)
        {
            g_futureTrainX[i] = g_trainX[i];
            g_futureTrainY[i] = g_trainY[i];
        }
    }
}

// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {
    if (!g_emergencyActive)
    {
        return;
    }

    if (g_emergencyTicksRemaining > 0)
    {
        g_emergencyTicksRemaining--;
    }

    if (g_emergencyTicksRemaining <= 0)
    {
        g_emergencyActive = false;
    }
}
