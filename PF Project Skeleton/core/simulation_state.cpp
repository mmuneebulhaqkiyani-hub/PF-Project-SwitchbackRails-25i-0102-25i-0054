#include "simulation_state.h"

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ############################################################################
// GRID
// ############################################################################
int g_rows=0;
int g_columns=0;
char g_grid[MaxRows][MaxColumns];

// ############################################################################
// TRAINS
// ############################################################################
int g_numtrains=0;
int g_trainX[MaxTrains];
int g_trainY[MaxTrains];
int g_traindirection[MaxTrains];
bool g_trainactive[MaxTrains];

int g_traindestinationX[MaxTrains];
int  g_traindestinationY[MaxTrains];

int  g_pastTrainX[MaxTrains];
int  g_pastTrainY[MaxTrains];
int  g_futureTrainX[MaxTrains];
int  g_futureTrainY[MaxTrains];
bool g_futureTrainActive[MaxTrains];

int  g_trainSpawnTick[MaxTrains];
int  g_trainColor[MaxTrains];
int  g_numspawns=0;
int  g_spawnX[MaxSpawns];
int  g_spawnY[MaxSpawns];
int  g_safetyDelay[MaxTrains];

int  g_currentTickNum=0;

// ############################################################################
// SWITCHES
// ############################################################################
int  g_switchState[Maxswitches];
int  g_switchInitState[Maxswitches];
int  g_switchMode[Maxswitches];

int  g_switchKUp[Maxswitches];
int  g_switchKRight[Maxswitches];
int  g_switchKDown[Maxswitches];
int  g_switchKLeft[Maxswitches];

int  g_switchCounterUp[Maxswitches];
int  g_switchCounterRight[Maxswitches];
int  g_switchCounterDown[Maxswitches];
int  g_switchCounterLeft[Maxswitches];

int  g_Switchcurrent=0;          // *** DEFINITION LIVES HERE ***
int  g_switchX[Maxswitches];       // *** AND HERE ***
int  g_switchY[Maxswitches];
bool g_switchQueueFlip[Maxswitches];

// ############################################################################
// DESTINATION POINTS
// ############################################################################
int g_numdestinations=0;
int g_destinationX[MaxDestinations];
int g_destinationY[MaxDestinations];

// ############################################################################
// SIMULATION PARAMETERS
// ############################################################################
int g_weatherMode=WeatherNormal;
int g_randomSeed=0;

// ############################################################################
// METRICS
// ############################################################################
int g_totalTicksRun=0;
int g_trainsArrived=0;
int g_trainsCrashed=0;

// ############################################################################
// EMERGENCY HALT
// ############################################################################
bool g_emergencyActive=false;
int  g_emergencyCenterX=0;
int  g_emergencyCenterY=0;
int  g_emergencyTicksRemaining=0;

// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
void initializeSimulationState() {
    // Grid
    g_rows= 0;
    g_columns= 0;
    for (int r=0;r< MaxRows;r++) {
        for (int c=0;c< MaxColumns;c++) {
            g_grid[r][c] = ' ';
        }
    }

    // Trains
    g_numtrains= 0;
    g_currentTickNum= 0;
    g_totalTicksRun= 0;
    g_trainsArrived= 0;
    g_trainsCrashed= 0;
    for (int i=0;i< MaxTrains;i++) {
        g_trainactive[i]= false;
        g_trainX[i]=0;
        g_trainY[i]= 0;
        g_traindirection[i]=0;
        g_traindestinationX[i]=0;
        g_traindestinationY[i]=0;
        g_pastTrainX[i]=0;
        g_pastTrainY[i]=0;
        g_futureTrainX[i]=0;
        g_futureTrainY[i]=0;
        g_futureTrainActive[i] = false;
        g_trainSpawnTick[i]=0;
        g_trainColor[i]=0;
        g_safetyDelay[i]=0;
    }

    g_numspawns = 0;
    for (int i = 0; i < MaxSpawns; i++) {
        g_spawnX[i] = -1;
        g_spawnY[i] = -1;
    }

    // Switches
    g_Switchcurrent = 0;
    for (int i = 0; i < Maxswitches; i++) {
        g_switchMode[i]=0;   // PER_DIR
        g_switchState[i]=0;   // STRAIGHT
        g_switchInitState[i]=0;

        g_switchKUp[i]= 0;
        g_switchKRight[i]=0;
        g_switchKDown[i]=0;
        g_switchKLeft[i]=0;

        g_switchCounterUp[i]=0;
        g_switchCounterRight[i]=0;
        g_switchCounterDown[i]=0;
        g_switchCounterLeft[i]=0;

        g_switchQueueFlip[i]   = false;

        g_switchX[i]=-1;  // "no switch here" sentinel
        g_switchY[i]=-1;
    }

    // Destinations
    g_numdestinations = 0;
    for (int i =0;i< MaxDestinations;i++) {
        g_destinationX[i] =0;
        g_destinationY[i] = 0;
    }

    // Weather & randomness
    g_weatherMode = WeatherNormal;
    g_randomSeed= 0;

    // Emergency halt
    g_emergencyActive= false;
    g_emergencyCenterX= 0;
    g_emergencyCenterY= 0;
    g_emergencyTicksRemaining= 0;
}
