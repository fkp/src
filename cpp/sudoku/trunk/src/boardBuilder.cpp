// $Id: boardBuilder.cpp 40 2008-02-17 13:29:43Z fraser $
//

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<iostream>

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

#if !defined(BOARDBUILDER_HEADER)
  #define BOARDBUILDER_HEADER
  #include<boardBuilder.hpp>
#endif

bool BoardBuilder::Init()
{
  // Initialise the board as all unsolved squares
  for(int y=0; y<Common::NoNumbers; y++)
  {
    for(int x=0; x<Common::NoNumbers; x++)
    {
      _theSquares[x][y] = new Square();

      // Add these coordinates to the ones we know aren't solved
      Coords theseCoords(x,y);
      unsolvedCoords.push_back(theseCoords);
    }
  }

  // Initialize random seed
  srand ( time(NULL) );

  return true;
}

bool BoardBuilder::SolveRandomLocation()
{
  int coordsIndex;
  Square * thisSquare;

  do
  {
    // Choose random coordinate to work with
    coordsIndex = rand() % unsolvedCoords.size();
    Coords coords = unsolvedCoords[coordsIndex];
    thisSquare = GetSquare(coords);

    // Remove these coordinates from those we know are unsolved
    unsolvedCoords.erase(unsolvedCoords.begin()+coordsIndex);
  } while (thisSquare->SolvedValue() && unsolvedCoords.size() > 0);

  if (unsolvedCoords.size() == 0)
  {
    return false;
  }
  else
  {
    // Get the unsolved values of this square and make one of them the solution
    const vector<int> values = thisSquare->Values();
    int solnIndex = rand() % values.size();
    cout << "About to randomly solve square to " << values[solnIndex] <<
      " possibilities are: " << thisSquare->ToString() << endl;

    thisSquare->SolveAs(values[solnIndex]);
  }

  return true;
}
