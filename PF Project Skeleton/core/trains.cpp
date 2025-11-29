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
int getSmartDirectionAtCrossing() {
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

    return bestDir;   //-1 if none is valid
}
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
        int bestDist =999999999999; 
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
        int x=g_trainX[i];
        int y =g_trainY[i];
        int dir= g_traindirection[i];


        //ifthe train iz somhow out of bounds then deactivate
        if (!isInBounds(x,y))
        {
            g_trainactive[i]  =false;
            continue;
        }

        char tile=g_grid[y][x];
        //ts gon   decide next direction
        int nextDirection;
        if (tile=='+')
        {
            // use smart crossing logic based +
            int goalX=g_traindestinationX[i];
            int goalY=g_traindestinationY[i];


            nextDirection=getSmartDirectionAtCrossing(dir,x,y,goalX,goalY);
        }
        else
        {
            //normal direction changes likstraight, curves, etc
            nextDirection=getNextDirection(dir,tile);


        }

        // if no valid directionthen the train stops/crashes
        if (nextDirection==-1)
        {
            g_trainactive[i]=false;
            continue;
        }
        // store new direction
        g_traindirection[i] = nextDirection;
        //ys gonnado the next position from direction
        int nx = x;
        int ny = y;

        if(nextDirection==0)//up
        {


            ny=ny-1;
        }
        else if (nextDirection==1)//right
        {
            nx=nx+1;
        }
        else if (nextDirection==2)//down
        {
            ny=ny+1;
        }
        else if (nextDirection==3)// left
        {
            nx=nx-1;
        }

        //check new position
        if(!isInBounds(nx,ny))
        {
            // off the grid tn deactivate the thing
            g_trainactive[i]=false;
            continue;
        }
        if(!isTrackTile(g_grid[ny][nx]))
        {

            //not on track then train not active
            g_trainactive[i]   =false;



            continue;
        }

        //apply themovements
        g_trainX[i]=nx;
        g_trainY[i]=ny;



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
                int dist_j = dfx_j + dfy_j;

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
                    g_futureTrainActive[i] = false;
                    g_futureTrainActive[j] = false;
                }

                
                continue;
            }

           
            //HEADON SWAP COLLISION                  
            bool 1_hits_2 = (g_futureTrainX[i]==g_trainX[j] &&
                             g_futureTrainY[i] ==g_trainY[j]);
            bool 2_hits_1 = (g_futureTrainX[j] ==g_trainX[i] &&
                             g_futureTrainY[j] ==g_trainY[i]);

            if (1_hits_2 && 2_hits_1)
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
                    g_futureTrainActive[i] = false;
                    g_futureTrainActive[j] = false;
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
