#include "io.h"
#include "simulation_state.h"
#include "grid.h"

#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sstream>   // <-- needed for stringstream

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
        cout<<"LEVEL FILE OPENED SUCCESSFULLY!"<<endl;
    }

    initializeSimulationState();

    string line;
    bool inMap=false;   // are we inside the MAP section?
    bool inTrainsline=false;   // are we inside TRAINS section
    int row=0;

    while (getline(file,line)) {
        // first line map ha?
        if (line=="MAP:") 
        {
            inMap=true;
            continue;
        }
        // leaving switches for now, maybe next step
        if (line=="SWITCHES:") 
        {
            inMap=false;
            continue;
        }
        // for reading trains section
        if (line=="TRAINS:") 
        {
            inMap=false;
            inTrainsline=true;
            continue;
        }

        //MAP SECTION 
        if (inMap) {
            // empty lines k liye
            if (line=="") {
                continue;
            }

            // length calculation for the row and assigning values.
            if (row < MaxRows)
             {
                int lengthline =(int)line.size();

                // har column k liye for the row
                for (int i = 0; i < lengthline && i < MaxColumns; i++) 
                {
                    g_grid[row][i]=line[i];
                }
                // extra columns mai spaces
                for (int i = lengthline; i < MaxColumns; i++) 
                {
                    g_grid[row][i] = ' ';
                }

                row = row + 1;
            }
            continue;
        }

        //TRAINS SECTION 
        if (inTrainsline) 
        {
            if (line=="") {
                // skip empty lines inside TRAINS
                continue;
            }

            stringstream ss(line);
            int spawnTick, x, y, dir, color;

            // try to read 5 integers from the line
            if (ss>>spawnTick>>x>>y>>dir>>color) {
                if (g_numtrains<MaxTrains) {
                    int i=g_numtrains;

                    g_trainSpawnTick[i] = spawnTick;
                    g_trainX[i]= x;
                    g_trainY[i]= y;
                    g_traindirection[i]=dir;
                    g_trainactive[i]= false; // will become true when spawned

                    g_numtrains++;
                }
            }

            continue; // done handling this line
        }

        // Any other lines (like NAME:, ROWS:, COLS:) are ignored for now.
    }

    //set grid size
    g_rows=row;
    g_columns=MaxColumns;   // for now do same width/height

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
