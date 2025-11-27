#include "simulation_state.h"
#include <cstring>

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
int g_rows = 0; // this the number of rows in the grid
int g_columns = 0; // this is the number of columns in the grid 
char g_grid[MaxRows][MaxColumns];  // 2D array representing the grid map

// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------
// using the exact variable names i wrote in the .h file so nothing breaks
int g_numtrains = 0;  // current number of trains in the simulation (my original var)
int g_trainX[MaxTrains]; // this says the x position of each train
int g_trainY[MaxTrains]; // this says the y position of each train
// direction array (N S E W) cuz i wrote it like this in .h
char g_traindirection[MaxTrains]; 
// alive or dead / spawned or not spawned
bool g_trainactive[MaxTrains];
// destination arrays (will be used later for stuff maybe)
int g_traindestinationX[MaxTrains];
int g_traindestinationY[MaxTrains];
// spawn tick array
int g_trainSpawnTick[MaxTrains];
// tick number for simulation
int g_tickNumber = 0;

// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
// will fill later (when i get to switches logic)


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
g_tickNumber = 0;

for (int i = 0; i < MaxTrains; i++) {
    g_trainactive[i]=false;
    g_trainX[i]=0;
    g_trainY[i]=0;
    g_traindirection[i]='N'; // default to north (will be overwritten by .lvl)
    g_traindestinationX[i]=0;
    g_traindestinationY[i]=0;
    g_trainSpawnTick[i]=0;
}



}
