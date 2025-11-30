#include "io.h"
#include "simulation_state.h"
#include "grid.h"

#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sstream>   // needed for stringstream

using namespace std;

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ----------------------------------------------------------------------------

bool loadLevelFile(const char* filename) {
 
      ifstream file(filename);
    if (!file.is_open()) 
    {
        cout <<"LEVEL FILE FAILED TO OPEN: "<< filename<<endl;
        return false;
    } else 
    {
        cout <<"LEVEL FILE OPENED SUCCESSFULLY: "<<filename<<endl;
    }

    initializeSimulationState();

    string line;
    bool Mapinside=false;   //are we inside the MAP section?
    bool inTrainsline=false;   //are we inside TRAINS section ""Train:"
    bool inSwitchesline=false; //are we inside SWITCHES section "Switches:"
    int row=0;

    while (getline(file,line)) {

        if (expectRows)
        {
            stringstream ss(line);
            ss>>headerRows;
            expectRows=false;
            continue;
        }
        //if firt thng is col then the next line is number of columns
        if (expectCols)
        {
            stringstream ss(line);
            ss>>headerCols;
            expectCols=false;
            continue;
        }
        // ts id the section headers
        if (line=="MAP:")
        {
            Mapinside=true;
            inTrainsline=false;
            inSwitchesline=false;
            continue;
        }
        if (line=="ROWS:")
        {
            expectRows=true;
            continue;
        }
        if (line=="COLS:")
        {
            expectCols=true;
            continue;
        }
        
        // first line map ha?
        if (line=="MAP:") 
        {
            Mapinside=true;
            continue;
        }
        // leaving switches for now, maybe next step
        if (line=="SWITCHES:") 
        {
            Mapinside=false;
            inSwitchesline=true;
            inTrainsline=false;
            continue;
        }
        // for reading trains section
        if (line=="TRAINS:") 
        {
            Mapinside=false;
            inTrainsline=true;
            inSwitchesline=false;
            continue;
        }
        //ts is th switches section
        if (inSwitchesline) {
          if (line == "") {
              continue; // skip empty lines in switches block
            }
           // expected line format: A PER_DIR 0 2 2 2 2 STRAIGHT TURN
            stringstream ss(line);
            char letter;
            string mode;
            int initState;
            int kup, kright, kdown, kleft;
            string straightName;
            string turnName;
            if (ss >> letter >> mode >> initState >> kup >> kright >> kdown >> kleft >> straightName >> turnName) {
                int idx = getSwitchIndex(letter);
                if (idx >= 0 && idx < Maxswitches) 
                {
                    // mode: PER_DIR or GLOBAL
                    if (mode == "GLOBAL") 
                    {
                        g_switchMode[idx]=1;
                    } 
                    else 
                    {
                        g_switchMode[idx]=0;// PER_DIR
                    }

                    g_switchInitState[idx]= initState;
                    g_switchState[idx]=initState;
                    //for the Kswitches thing
                    g_switchKUp[idx]=kup;
                    g_switchKRight[idx]=kright;
                    g_switchKDown[idx]=kdown;
                    g_switchKLeft[idx]=kleft;

                    g_switchCounterUp[idx]=0;
                    g_switchCounterRight[idx]=0;
                    g_switchCounterDown[idx]=0;
                    g_switchCounterLeft[idx]=0;
                    g_switchQueueFlip[idx]=false;
                }
            }
            continue;
        }

        //MAP SECTION 
        if (Mapinside) {
            // empty lines k liye
            if (line=="") 
            {
                continue;
            }

            // length calculation for the row and assigning values.
            if (row<MaxRows)
             {
                int lengthline =(int)line.size();

                // har column k liye for the row
                for (int i = 0;i <lengthline && i < MaxColumns;i++) 
                {
                    g_grid[row][i]=line[i];
                }
                // extra columns mai spaces
                for (int i =lengthline; i<MaxColumns; i++) 
                {
                    g_grid[row][i]= ' ';
                }

                row = row + 1;
            }
            continue;
        }

        //TRAINS SECTION 
        if (inTrainsline) 
        {
            if (line=="") 
            {
                // skip empty lines inside TRAINS
                continue;
            }

            stringstream ss(line);
            int spawnTick, x, y, dir,color;

            // this read 5 integers from the line
            if (ss>>spawnTick>>x>>y>>dir>>color) {
                if (g_numtrains<MaxTrains) {
                    int i=g_numtrains;

                    g_trainSpawnTick[i]=spawnTick;
                    g_trainX[i]=x;
                    g_trainY[i]=y;
           
                    g_traindirection[i]=dir;

                    g_trainactive[i]=false; //will become true when spawne

                    g_trainColor[i]=color;

                    g_numtrains++;
                }
            }

            continue; // done handling this line
        }

        
    }
    // set grid size from what we actually read
    g_rows= row;
    g_columns =MaxColumns;

 // now weve got to read and save the destinations and switches
    g_numdestinations=0;
    g_Switchcurrent= 0;
    //initalizing with false
    bool switchEncountered[Maxswitches];
    for (int i=0;i<Maxswitches;i++) {
        switchEncountered[i]=false;
    }
    //checkin the whole grid and marking destinations in another array
    for (int y=0;y<g_rows;y++) {
        for (int x=0;x<g_columns;x++) {
            char tile=g_grid[y][x];

            if (tile=='D') 
            {
                if (g_numdestinations <MaxDestinations) 
                {
                g_destinationX[g_numdestinations]=x;
                g_destinationY[g_numdestinations]=y;
                g_numdestinations++;
                }
            }
   //gotta do the same for switches too
            if (isSwitchTile(tile)) {
                int idx = getSwitchIndex(tile);
                if (idx>=0 && idx<Maxswitches && !switchEncountered[idx]) 
                {
                    switchEncountered[idx] =true;
                    g_switchX[idx]=x;
                    g_switchY[idx]=y;
                }
            }
        }
    }

    for (int i = 0; i < Maxswitches; i++) 
    {
        if (switchEncountered[i]) {
            g_Switchcurrent++;
        }
    }

    file.close();
    return true;
}
   


// ----------------------------------------------------------------------------
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------
void initializeLogFiles() {
    //trace
    {
        ofstream out("out/trace.csv");
        if (out.is_open()) 
        {
            out<<"Tick,TrainID,X,Y,Direction,State\n";
        }
    }

    //switches
    {
        ofstream out("out/switches.csv");
        if (out.is_open()) 
        {
            out<<"Tick,Switch,Mode,State\n";
        }
    }

    //signals
    {
        ofstream out("out/signals.csv");
        if (out.is_open()) 
        {
            out<<"Tick,Switch,Signal\n";
        }
    }

    //metrics
    {
        ofstream out("out/metrics.txt");
        if (out.is_open()) 
        {
            out << "";  
        }
    }
}

// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace() {
     ofstream out("out/trace.csv", ios::app);
    if (!out.is_open()) 
    return;

    for (int i = 0; i < g_numtrains; i++) {
        const char *stateStr = g_trainactive[i] ?"ACTIVE":"INACTIVE";
        out << g_currentTickNum << ","
            << i << ","
            << g_trainX[i] << ","
            << g_trainY[i] << ","
            << g_traindirection[i] << ","
            << stateStr << "\n";
    }
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState() {
    ofstream out("out/switches.csv", ios::app);
    if (!out.is_open()) return;

    for (int i = 0; i < Maxswitches; i++) {
        //only log switches that exist on the board
        if (g_switchX[i] < 0 || g_switchY[i]<0) continue;

        char letter = 'A' + i;
        const char *modeStr = (g_switchMode[i]==1) ? "GLOBAL" : "PER_DIR";
        const char *stateStr= (g_switchState[i]==0) ? "STRAIGHT" : "TURN";

        out << g_currentTickNum << ","
            << letter << ","
            << modeStr << ","
            << stateStr << "\n";
    }

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
    ofstream out("out/metrics.txt", ios::app);
    if (!out.is_open()) return;

    out << "TotalTicks: "     << g_totalTicksRun  << "\n";
    out << "TrainsDelivered: "<< g_trainsArrived << "\n";
    out << "TrainsCrashed: "  << g_trainsCrashed << "\n";

    // TODO: add AverageWaitTime, EnergyUsed, SwitchFlips, Efficiency, etc.
}
