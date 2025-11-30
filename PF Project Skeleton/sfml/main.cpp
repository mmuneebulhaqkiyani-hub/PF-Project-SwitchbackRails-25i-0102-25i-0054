#include "app.h"  // not really used now, but OK
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include "../core/grid.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Print current grid to terminal
static void printGridToTerminal() {
    // copy grid into a temporary buffer so we can overlay trains
    std::vector<std::string> rows(g_rows, std::string(g_columns, ' '));
    for (int y = 0; y < g_rows; y++) {
        for (int x = 0; x < g_columns; x++) {
            rows[y][x] = g_grid[y][x];
        }
    }

    for (int i = 0; i < g_numtrains; i++) {
        if (!g_trainactive[i]) {
            continue;
        }
        int x = g_trainX[i];
        int y = g_trainY[i];
        if (!isInBounds(x, y)) {
            continue;
        }

        char symbol;
        if (i < 10) {
            symbol = '0' + i;
        } else {
            symbol = 'A' + ((i - 10) % 26);
        }
        rows[y][x] = symbol;
    }

    for (int y = 0; y < g_rows; y++) {
        cout << rows[y] << "\n";
    }
}

static const char* directionName(int dir) {
    switch (dir) {
        case 0: return "UP";
        case 1: return "RIGHT";
        case 2: return "DOWN";
        case 3: return "LEFT";
        default: return "?";
    }
}

static const char* trainStateLabel(int idx) {
    if (g_trainactive[idx]) {
        return "ACTIVE";
    }
    if (g_trainX[idx] == g_traindestinationX[idx] && g_trainY[idx] == g_traindestinationY[idx]) {
        return "ARRIVED";
    }
    return "CRASHED";
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
        for (int i = 0; i < g_numtrains; i++) {
            cout << "Train " << i << " : (" << g_trainX[i] << "," << g_trainY[i]
                 << ") dir=" << directionName(g_traindirection[i])
                 << " state=" << trainStateLabel(i) << "\n";
        }
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
