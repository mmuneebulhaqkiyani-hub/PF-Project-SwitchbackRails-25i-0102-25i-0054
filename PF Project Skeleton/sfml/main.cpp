#include "app.h"  // not really used now, but OK
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include "../core/grid.h"

#include <iostream>

using namespace std;

// Print current grid to terminal
static void printGridToTerminal() {
    for (int y = 0; y < g_rows; y++) {
        for (int x = 0; x < g_columns; x++) {
            cout << g_grid[y][x];
        }
        cout << "\n";
    }
}

int main(int argc, char** argv) {
    // Need a level file path
    if (argc < 2) {
        cout << "Usage:\n";
        cout << "  ./switchback_rails data/levels/easy_level.lvl\n";
        cout << "  ./switchback_rails data/levels/medium_level.lvl\n";
        cout << "  ./switchback_rails data/levels/hard_level.lvl\n";
        cout << "  ./switchback_rails data/levels/complex_network.lvl\n";
        return 1;
    }

    const char* levelPath = argv[1];

    if (!loadLevelFile(levelPath)) {
        cerr << "Failed to load level file: " << levelPath << "\n";
        return 1;
    }

    initializeSimulation();

    const int MAX_TICKS = 100000; // safety cap

    while (true) {
        cout << "==============================\n";
        cout << "Tick: " << g_currentTickNum << "\n";
        printGridToTerminal();
        cout << "==============================\n\n";

        if (isSimulationComplete() || g_currentTickNum >= MAX_TICKS) {
            break;
        }

        simulateOneTick();
    }

    cout << "Simulation finished.\n";
    cout << "Total ticks run: "   << g_totalTicksRun  << "\n";
    cout << "Trains delivered: "  << g_trainsArrived
         << " / " << g_numtrains << "\n";
    cout << "Trains crashed: "    << g_trainsCrashed << "\n";

    return 0;
}
