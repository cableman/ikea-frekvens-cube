#include "lemming_frames.h"

int LemmingFrames::f1[FRAMES][ROWS][COLUMNS]= {
  { 
    { 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 1, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 0, 0 }
  },
  { 
    { 0, 0, 1, 0, 1, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 1, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 0, 1, 1, 1, 0, 1 }, 
    { 0, 0, 1, 1, 0, 1 }, 
    { 0, 1, 1, 0, 1, 0 }, 
    { 0, 1, 1, 0, 0, 0 }
  },
  { 
    { 0, 0, 0, 0, 0, 0 }, 
    { 0, 1, 0, 1, 0, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 1, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 1, 1, 1, 1, 1, 0 }, 
    { 0, 1, 1, 1, 1, 0 }, 
    { 1, 1, 0, 0, 1, 1 }
  },
  { 
    { 0, 0, 0, 0, 0, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 1, 1, 1, 1, 0 }, 
    { 0, 1, 1, 1, 1, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 0, 1, 1, 1, 0, 0 }, 
    { 0, 0, 1, 1, 0, 0 }, 
    { 1, 1, 1, 1, 1, 0 }, 
    { 1, 0, 0, 1, 1, 0 }
  }  
};

/**
 * Constructor.
 */
LemmingFrames::LemmingFrames() {
  this->current = 0;
}

void LemmingFrames::nextFrame(FrekvensPanel panel, int offsetRows, int offsetCols) {
  // loop through array's rows
  for ( int i = 0; i < ROWS; ++i ) {
    // loop through columns of current row
    for ( int j = 0; j < COLUMNS; ++j ) {
      if (this->f1[this->current][ i ][ j ] == 1) {
         panel.drawPixel(j + offsetCols, i + offsetRows, 1);
      }
    }
  } 
  
  this->current++;
  if (this->current > 3) {
    this->current = 0;
  }
}
