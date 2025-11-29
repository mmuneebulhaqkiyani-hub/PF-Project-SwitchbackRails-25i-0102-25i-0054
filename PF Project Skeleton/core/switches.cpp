#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================

// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters() {
      // go over all trains
    for (int i=0;i<g_numtrains;i++)
    {
        if (!g_trainactive[i])
        {
            continue;
        }

        int x=g_trainX[i];
        int y=g_trainY[i];
        if (!isInBounds(x,y))
        {
            continue;
        }

        char tile=g_grid[y][x];

        //not a switch letter (A..Z)
        if (!isSwitchTile(tile))
        {
            continue;
        }
        //valid switch index??? 
        int idx=getSwitchIndex(tile);

        if (idx<0 || idx >=Maxswitches)
        {
            continue;
        }

        //determine entry direction 
        int pastx=g_pastTrainX[i];
        int pasty=g_pastTrainY[i];

        int direction=-1;

        if (pasty==y-1) //comparing past y with current y
        {
            direction=0; //from up
        }
        if (pastx==x+1) 
        {
            direction=3; //came from left
        }
        if (pasty==y+1) 
        {
            direction=2; //from down
        }
        if (pastx==x-1) 
        {
            direction=1; //from right
        }

        //this for safety reasons like the train didnt come from any valid direction and was on switch
        if (direction==-1)
        {
            continue;
        }

     //counter
        if (g_switchMode[idx]==0) //global k  
        {
            if (direction==0)
            {
                g_switchCounterUp[idx]++;
                if (g_switchCounterUp[idx]>=g_switchKUp[idx])
                {
                    g_switchQueueFlip[idx]=true;
                    g_switchCounterUp[idx]=0;
                }
            }
            else if (direction==1)
            {
                g_switchCounterRight[idx]++;
                if (g_switchCounterRight[idx]>=g_switchKRight[idx])
                {
                    g_switchQueueFlip[idx]=true;
                    g_switchCounterRight[idx]=0;
                }
            }
            else if (direction==2)
            {
                g_switchCounterDown[idx]++;
                if (g_switchCounterDown[idx]>=g_switchKDown[idx])
                {
                    g_switchQueueFlip[idx]=true;
                    g_switchCounterDown[idx]=0;
                }
            }
            else if (direction==3)
            {
                g_switchCounterLeft[idx]++;
                if (g_switchCounterLeft[idx]>=g_switchKLeft[idx])
                {
                    g_switchQueueFlip[idx]=true;
                    g_switchCounterLeft[idx]=0;
                }
            }
        }

        //Global k
        else   
        {
            // We using "up" for global k kinda cheating but it works the same
            g_switchCounterUp[idx]++;

            if (g_switchCounterUp[idx]>=g_switchKUp[idx])
            {
                g_switchQueueFlip[idx]=true;
                g_switchCounterUp[idx]=0;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips() {
    // go through all switches A..Z (0..Maxswitches-1)
    for (int i=0;i<Maxswitches;i++)
    {
        int mode=g_switchMode[i];   //for our reference 0=PER_DIR, 1=GLOBAL

       //dirmode
        if (mode==0)
        {
            bool shouldFlip=false;

            //from up
            if (g_switchKUp[i]>0 && g_switchCounterUp[i]>=g_switchKUp[i])
            {
                shouldFlip=true;
                g_switchCounterUp[i]=0;  // reset so it can count again
            }

            //from right
            if (g_switchKRight[i]>0 && g_switchCounterRight[i]>=g_switchKRight[i])
            {
                shouldFlip=true;
                g_switchCounterRight[i]=0;
            }

            //from down
            if (g_switchKDown[i]>0 && g_switchCounterDown[i]>=g_switchKDown[i])
            {
                shouldFlip=true;
                g_switchCounterDown[i]=0;
            }

            //from left
            if (g_switchKLeft[i]>0 && g_switchCounterLeft[i]>=g_switchKLeft[i])
            {
                shouldFlip=true;
                g_switchCounterLeft[i]=0;
            }

            if (shouldFlip)
            {
                g_switchQueueFlip[i]=true;
            }
        }
        //global mode
        else
        {
            int K=g_switchKUp[i];  //treat Kup as global K as we did before

            if (K >0 && g_switchCounterUp[i] >= K)
            {
                g_switchQueueFlip[i]=true;
                g_switchCounterUp[i]=0; //reset
            }
        }
    }
}


// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
      for (int i=0;i<Maxswitches;i++)
    {
        //Check if the switch has been marked for the flip 
        if (g_switchQueueFlip[i]) 
        {
            //fllippin
            if (g_switchState[i]==0) //currently STRAIGHT
            {
                g_switchState[i]=1;//flip to turn or vice versa
            }
            else
            {
                g_switchState[i]=0; //flip to straight or vice versa
            }

            //Reset it as it has been applied
            g_switchQueueFlip[i]=false;
        }
    }
}


// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState() {
}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection() {
}
