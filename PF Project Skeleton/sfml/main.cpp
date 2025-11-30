#include "app.h"  // not really used now, but OK
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include "../core/grid.h"

#include <iostream>
#include <string>
#include <unistd.h>  //for usleep()

using namespace std;

// Printing the grid to terminal
static void printGridToTerminal() {
    for (int y=0;y<g_rows;y++) {
        for (int x=0;x<g_columns;x++) {
            //STARTING
            char c=g_grid[y][x];

            //Check if any active train
            for (int i=0;i<g_numtrains;i++) {
                if (!g_trainactive[i]) {
                    continue;
                }
                if (g_trainX[i]==x&& g_trainY[i]== y &&isInBounds(x, y)) {
                    c = 'O';//O IS TRAIN
                    break;  
                }
            }

            cout<<c;
        }
        cout <<"\n";
    }
}

static void clearScreen() {
#if defined(_WIN32)
    std::system("cls");  //widow
#else
    std::system("clear"); //linux or khatara ubuntu
#endif
}


int main(int argc, char** argv) {
    //loading the file finally
    if (argc < 2) {
        cout << "TRY THIS:\n";
        cout << "  ./switchback_rails data/levels/easy_level.lvl\n";
        cout << "  ./switchback_rails data/levels/medium_level.lvl\n";
        cout << "  ./switchback_rails data/levels/hard_level.lvl\n";
        cout << "  ./switchback_rails data/levels/complex_network.lvl\n";
        return 1;
    }

    const char* levelPath=argv[1];

    if (!loadLevelFile(levelPath)) {
        cerr << "Failed to load level file: "<<levelPath<<"\n";
        return 1;
    }

    initializeSimulation();

    const int MAX_TICKS=99999; 

    clearScreen();
    cout << "Tick: "<< g_currentTickNum << "\n";
    printGridToTerminal();

    while (true) {
        if (isSimulationComplete()||g_currentTickNum >= MAX_TICKS) {
            break;
        }

        simulateOneTick();

        clearScreen();
        cout << "Tick: "<< g_currentTickNum<< "\n";
        printGridToTerminal();
        usleep(1000000); //0.1 second delay
        
    }

    cout << "Simulation finished.\n";
    cout << "Total ticks run: " << g_totalTicksRun  << "\n";
    cout << "Trains delivered: "<< g_trainsArrived
         << " / "<< g_numtrains<< "\n";
    cout << "Trains crashed: "<< g_trainsCrashed << "\n";

    return 0;
}
