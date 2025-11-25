#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ----------------------------------------------------------------------------
bool loadLevelFile() {
     // hard coding it for now ,we'll deal with it later
     ifstream file("data/levels/easy_level.lvl");
     if (!file.is_open()) {
        cout << "LEVEL FILE FAILED TO OPEN!" << endl;
        return false;
    }
    else {
        cout << "LEVEL FILE OPENED SUCCESSFULLY!" << endl;
    }
   
     initializeSimulationState();
    

    string line;
    bool inMap = false;   // are we inside the MAP section?
    int row = 0;
     while (getline(file, line)) {
        //first line map ha?
        if (line == "MAP:") {
            inMap = true;
            continue;
        }
        
        if (line == "SWITCHES:") {
            inMap = false;
            break;              
        }
        if (inMap) {
        //empty lines k liye
        if (line == "") {
            continue;
        }
        //length caclulation for the row and assigning  values.
        //har column k liye for the row
        if (row < MaxRows) {
                int lengthline = (int)line.size();
        
        for (int i = 0; i < lengthline && i < MaxColumns; i++) {
                    g_grid[row][i] = line[i];
        }            
        //extra rows mai spaces
        for (int i = lengthline; i < MaxColumns; i++) {
                    g_grid[row][i] = ' ';
        }    
        row = row + 1;
}
}
}

    g_rows = row;
    //for now do same width and height
    g_columns = MaxColumns;
    file.close();
    return true;
}   
// ----------------------------------------------------------------------------
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------
void initializeLogFiles() {
}

// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace() {
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState() {
}

// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState() {
}

// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics() {
}
