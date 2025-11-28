#include "simulation_state.h"
#include <cstring>

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ############################################################################
// GRID
// ############################################################################
int g_rows=0; // this the number of rows in the grid
int g_columns=0; // this is the number of columns in the grid 
char g_grid[MaxRows][MaxColumns];  // 2D array representing the grid map

// ############################################################################
// TRAINS

// ############################################################################

// using the exact variable names i wrote in the .h file so nothing breaks
int g_numtrains=0;  // current number of trains in the simulation (my original var)
int g_trainX[MaxTrains]; // this says the x position of each train
int g_trainY[MaxTrains]; // this says the y position of each train
// direction array for the train 0 1 2 3
int g_traindirection[MaxTrains]; 
// alive or dead / spawned or not spawned
bool g_trainactive[MaxTrains];
// destination arrays (will be used later for stuff maybe)
int g_traindestinationX[MaxTrains];
int g_traindestinationY[MaxTrains];
// spawn tick array
int g_trainSpawnTick[MaxTrains];
// tick number for simulation
int g_currentTickNum=0;

// ############################################################################
// SWITCHES
// ############################################################################
   //filling it because we will work on it tomorrow (DONE FILLINGGGG)
int g_switchState[Maxswitches];// current state (0 = straight, 1 = turn)
int g_switchInitState[Maxswitches];// initial state from level file
int g_switchMode[Maxswitches];// 0 = PER_DIR, 1 = GLOBAL
 // switch direction arrays
int g_switchKUp[Maxswitches];
int g_switchKRight[Maxswitches];

int g_switchKDown[Maxswitches];

int g_switchKLeft[Maxswitches];
  //their counters too
int g_switchCounterUp[Maxswitches];
int g_switchCounterRight[Maxswitches];
int g_switchCounterDown[Maxswitches];
int g_switchCounterLeft[Maxswitches];

// ############################################################################
// DESTINATION POINTS
// ############################################################################
int g_numdestinations = 0;// how many destination tiles in the map
int g_destinationX[MaxDestinations];
int g_destinationY[MaxDestinations];


// ############################################################################
// SIMULATION PARAMETERS
// ############################################################################
int g_weatherMode=WeatherNormal;// WeatherNormal / WeatherRain / WeatherFog
int g_randomSeed=0;// seed for deterministic random behaviour
int g_currentick=0;

// ############################################################################
// METRICS
// ############################################################################
int g_totalTicksRun= 0;// how many ticks ran in the simulation
int g_trainsArrived=0;// trains that reached destination
int g_trainsCrashed= 0;// trains that crashed


// ############################################################################
// EMERGENCY HALT
// ############################################################################
bool  g_emergencyActive=false;// true if emergency halt zone is active

int g_emergencyCenterX=0;// center x of 3x3 emergency zone
int g_emergencyCenterY=0;// center y of 3x3 emergency zone
int g_emergencyTicksRemaining=0; // ticks left for halt so we starting from 0


// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
void initializeSimulationState() {
        
    g_rows = 0;
    g_columns = 0;

    // clearing da grid with spaces
    for (int r = 0; r < MaxRows; r++) {
        for (int c = 0; c < MaxColumns; c++) {
            g_grid[r][c] = ' ';
        }
    }

// reset trains
g_numtrains = 0;
g_currentTickNum = 0;

for (int i = 0; i < MaxTrains; i++) {
    g_trainactive[i]=false;
    g_trainX[i]=0;
    g_trainY[i]=0;
    g_traindirection[i]=0; // default to north (will be overwritten by .lvl)
    g_traindestinationX[i]=0;
    g_traindestinationY[i]=0;
    g_trainSpawnTick[i]=0;
}



}
