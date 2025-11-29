#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include <cstdlib>
#include <ctime>

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation() {
    g_trainsArrived=0;
    g_currentTickNum=0;

}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick() {

    spawnTrainsForTick();
    bool allValid=determineNextPosition();
    if (!allValid) {
        return;  //thi gona skip the tick if their are invalid positions
    }
    updateSwitchCounters();
    queueSwitchFlips();
    detectCollisions();
    moveAllTrains();
    checkArrivals();
}



// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete() {
    //for now we will stop the thing at 50 ticks
    return (g_currentTickNum > 50);

}
