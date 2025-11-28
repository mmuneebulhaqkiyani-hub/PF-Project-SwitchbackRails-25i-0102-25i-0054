#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================

// ----------------------------------------------------------------------------
// GRID CONSTANTS
// ----------------------------------------------------------------------------
 const int MaxRows= 50;
 const int MaxColumns=80;       
// this will be like the qactual grid size for whatever the current level is
// since the actual memory is in .cpp file we have to use the extern thing 
 extern int g_rows; 
 extern int g_columns;
 // just remembered that we have to make 2d array for the maximum width and hieght of the grid thing
 extern char g_grid[MaxRows][MaxColumns];
// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
  // we have to declare max train constant here and since it dont mention the exact number lets just do a century (W babar lol)
  const int MaxTrains=100;

  extern int g_numtrains; // this is the current number of trains in the simulation
 // we have to make arrays for the train properties like its position direction speed and state and stuff;
 extern int g_trainX[MaxTrains]; // this says the x position of each train


   extern int g_trainY[MaxTrains]; // this says the y position of each train
// there also should be variable like array that says where thee direction of the train is like north south east west
// we should use char for that and have it like N S E W. thhis is smart move by me fr. W high iq me lol.
 extern int g_traindirection[MaxTrains]; // this says the direction of each train
 //now for wheteher the train is alive or dead we can use bool array for that and like if the train has spawned in the lobby or not yet
 extern bool g_trainactive[MaxTrains];
 //each train will have o go to some destination so we need arrays for that too
 //This should be useful when we do the distance calculation adn stuff;
 extern int g_traindestinationX[MaxTrains]; // destination x for each train

 extern int g_traindestinationY[MaxTrains]; // destination y for each train
 extern int g_pastTrainX[MaxTrains]; //using it to store previous position
 extern int g_futureTrainY[MaxTrains];//using it to store next position
// ---- ------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------
//now we should have constants for the switches too
//but S and D are not switches so we have to exclude them but confused on this rn if we should keep it 26 or nah lets just do 26 we will change this if necessary
 const int Maxswitches=26;// A-Z means 26 switches max
//each level will have specific number of switches so we need a variable for that too
 extern int g_Switchcurrent; // current number of switches in the level
 //we need x and y position arrays for the switches too
 extern int g_switchX[Maxswitches]; // x position for each switch
extern int g_switchY[Maxswitches]; // y position for each switch

extern int  g_switchState[Maxswitches];// current state (0 = straight, 1 = turn)

extern int  g_switchInitState[Maxswitches]; // initial state from level file
extern int  g_switchMode[Maxswitches];// 0 = PER_DIR, 1 = GLOBAL
extern int  g_switchCounterUp[Maxswitches];
extern int  g_switchCounterRight[Maxswitches];
extern int  g_switchCounterDown[Maxswitches];
extern int  g_switchCounterLeft[Maxswitches];
extern bool g_switchQueueFlip[Maxswitches];// ts wold be true if switch should flip at end of tick so like if tick is 5 and we queue flip it then at the end of tick 5 it will flip and be effective for tick 6 (hopefully)
extern int g_switchKUp[Maxswitches];
extern int g_switchKRight[Maxswitches];
extern int g_switchKDown[Maxswitches];
extern int g_switchKLeft[Maxswitches];

// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------

const int WeatherNormal = 0;
const int WeatherRain   = 1;
const int WeatherFog    = 2;
// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------
const int SignalRed    = 0;
const int SignalYellow = 1;
const int SignalGreen  = 2;



// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
// ----------------------------------------------------------------------------

//no need to do nothing fr cuz we already declared the g_variables above

// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------
extern int g_currentTickNum; // this will be  the current simulation tick 


// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
// ----------------------------------------------------------------------------
extern int  g_trainSpawnTick[MaxTrains];

// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
// ----------------------------------------------------------------------------

//ok so we have to set like the max number of D (destination) points too
const int MaxDestinations = 50;  
extern int g_numdestinations;//ts wil tell us how many destination tiles in the map
extern int g_destinationX[MaxDestinations];
extern int g_destinationY[MaxDestinations];

// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
extern int g_weatherMode;// here its gonna be for like various modes like weather normal / weather rain / weather fog
extern int g_randomSeed;// this is the seed for therandom behaviour thing

// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
// ----------------------------------------------------------------------------
extern int g_totalTicksRun;//ok so this will tell us how many ticks are done in the simulation thing


extern int g_trainsArrived;//ts tells how many trains that reached destination
extern int g_trainsCrashed;// ts the trains that crashed thing

// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------
//so we gonna need bool to see if the emergency halt zone is active or no it will be useful when we implement the emergency halt feature and extern is becz it will be in the .cpp file
extern bool g_emergencyActive;
extern int  g_emergencyCenterX;// center x of 3x3 emergency zone
extern int  g_emergencyCenterY;// center y of 3x3 emergency zone
extern int  g_emergencyTicksRemaining;// ticks left for halt

// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void initializeSimulationState();

#endif
