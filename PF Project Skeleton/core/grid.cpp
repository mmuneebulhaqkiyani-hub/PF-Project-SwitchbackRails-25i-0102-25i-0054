#include "grid.h"
#include "simulation_state.h"

// ============================================================================
// GRID.CPP - Grid utilities
// ============================================================================

// ----------------------------------------------------------------------------
// Check if a position is inside the grid.
// ----------------------------------------------------------------------------
// Returns true if x,y are within bounds.
// ----------------------------------------------------------------------------
bool isInBounds(int x, int y) {
    return (x >= 0 && x < g_columns && y >= 0 && y < g_rows);
}

// ----------------------------------------------------------------------------
// Check if a tile is a track tile.
// ----------------------------------------------------------------------------
// Returns true if the tile can be traversed by trains.
// ----------------------------------------------------------------------------
bool isTrackTile(char ITT) {
    if (ITT=='-' || ITT=='|' || ITT=='/'|| ITT=='\\' ||ITT =='+' || ITT=='='|| ITT=='S'|| ITT=='D') 
    {
        return true;
    }
    else {
        if (isSwitchTile(ITT)) 
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
}


// ----------------------------------------------------------------------------
// Check if a tile is a switch.
// ----------------------------------------------------------------------------
// Returns true if the tile is 'A'..'Z'.
// ----------------------------------------------------------------------------
bool isSwitchTile(char IST) {
    if (IST=='S' || IST=='D') 
    {
        return false;
    }
    else 
    {
        if (IST>='A' && IST<='Z')
         {
            return true;
        }
        else 
        {
            return false;
        }
    }
}


// ----------------------------------------------------------------------------
// Get switch index from character.
// ----------------------------------------------------------------------------
// Maps 'A'..'Z' to 0..25, else -1.
// ----------------------------------------------------------------------------
int getSwitchIndex(char tile) {
    //first check is it even a switch
    if (tile>='A' && tile<='Z') 
    {
     int index = 0;
     char letter = 'A';
    //index loop
    while (letter != tile) {
            index = index + 1;
            letter = letter + 1;
        }
        
      return index;
    }
    else 
    {
       return -1;
    }
}


// ----------------------------------------------------------------------------
// Check if a position is a spawn point.
// ----------------------------------------------------------------------------
// Returns true if x,y is a spawn.
// ----------------------------------------------------------------------------

bool isSpawnPoint(int x, int y) {
    if (isInBounds(x, y)) 
    {
        if (g_grid[y][x] == 'S') 
        {
         return true;
        }
        else 
        {
         return false;
        }
    }
    else 
    {
        return false;
    }
}


// ----------------------------------------------------------------------------
// Check if a position is a destination.
// ----------------------------------------------------------------------------
// Returns true if x,y is a destination.
// ----------------------------------------------------------------------------
bool isDestinationPoint(int x, int y) {
    if (isInBounds(x, y)) 
    {
        if (g_grid[y][x] == 'D') 
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
    else 
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
// Toggle a safety tile.
// ----------------------------------------------------------------------------
// Returns true if toggled successfully.
// ----------------------------------------------------------------------------
bool toggleSafetyTile(int x, int y) {
    if (isInBounds(x, y)) 
    {
        char *cell = &g_grid[y][x];

        if (*cell == ' ') 
        {
            *cell = '=';
            return true;
        }
        else 
        {
            if (*cell=='=') 
            {
                *cell=' ';
                return true;
            }
            else 
            {
                return false;
            }
        }
    }
    else 
    {
        return false;
    }
}

