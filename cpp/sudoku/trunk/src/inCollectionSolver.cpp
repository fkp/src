// $Id: inCollectionSolver.cpp 19 2007-05-02 21:31:02Z fraser $
//

#include<iostream>
#include<inCollectionSolver.hpp>

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

string InCollectionSolver::SolverName()
{
  return "InCollectionSolver";
}

const map<string, int> InCollectionSolver::SolverStats()
{
  return _stats;
}

bool InCollectionSolver::SolveSquare(Coords coords)
{
  bool didSomething = false;
  Square * toSolve = _theBoard->GetSquare(coords);
  const vector<int> values = toSolve->Values();

  if (CheckIn(toSolve, _theBoard->GetCol(coords), coords.YCoord()))
  {
    _stats[ColToken]++;
    didSomething = true;
  }

  if (CheckIn(toSolve, _theBoard->GetRow(coords), coords.XCoord()))
  {
    _stats[RowToken]++;
    didSomething = true;
  }

  if (CheckIn(toSolve, _theBoard->GetArea(coords),
    _theBoard->IndexInAreaMap(coords)))
  {
    _stats[AreaToken]++;
    didSomething = true;
  }

  // Return whether we have managed to eliminate any possibilities
  return didSomething;
}

// A method used to take a square to work on and find all the possible values
// it can still be. If one of these possible values appears as a solved value in
// the array of pointers to squares we pass it then we will remove that value as
// a possibility for the squre being worked on. Note that as the square being
// solved will normally be one of the squares in the array, we exclude this
// index from those we check against.
bool InCollectionSolver::CheckIn(Square * theSquare, Square ** toCheck,
  int skipIndex)
{
  bool hasChanged = false;

  // Get the values in the square which remain unsolved
  const vector<int> values = theSquare->Values();

  if (values.size() > 1)
  {
    for(vector<int>::const_iterator it = values.begin(); it!=values.end(); it++)
    {
      // See if a square in this collection has this number as a solved
      // one. If so we can eliminate it from our square. Note that we will
      // skip the nominated index, as it will represent the square we are
      // solving in the row/column
      for (int i=0; i<Common::NoNumbers; i++)
      {
        if (i != skipIndex && toCheck[i]->SolvedValue() == *it)
        {
          //cout << "About to remove possibility " << *it << " from square" << endl;
          theSquare->RemovePossibility(*it);

          // flag that we have made some progress
          hasChanged = true;
        }
      }
    }
  }

  return hasChanged;
}

