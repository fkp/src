// $Id: uniqueInCollectionSolver.cpp 19 2007-05-02 21:31:02Z fraser $
//

#include<iostream>
#include<uniqueInCollectionSolver.hpp>

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

string UniqueInCollectionSolver::SolverName()
{
  return "UniqueInCollectionSolver";
}

bool UniqueInCollectionSolver::SolveSquare(Coords coords)
{
  bool madeProgress = false;
  Square * toSolve = _theBoard->GetSquare(coords);

  // For this solver, as we will solve the square as soon as we make progress
  // there is no point in checking any other collections as soon as one returns
  // true
  if (CheckForUniqueSolution(_theBoard->GetArea(coords), toSolve,
    _theBoard->IndexInAreaMap(coords)))
  {
    _stats[AreaToken]++;
    madeProgress = true;
  }

  if (!madeProgress && CheckForUniqueSolution (_theBoard->GetRow(coords),
    toSolve, coords.XCoord()))
  {
    _stats[RowToken]++;
    madeProgress = true;
  }

  if (!madeProgress && CheckForUniqueSolution(_theBoard->GetCol(coords),
    toSolve, coords.YCoord()))
  {
    _stats[ColToken]++;
    madeProgress = true;
  }

  return madeProgress;
}

bool UniqueInCollectionSolver::CheckForUniqueSolution(Square ** theCollection,
  Square * squareToSolve, int indexToIgnore)
{
  Square * compSquare;
  bool canSolve = false;

  // Loop through the possible solutions to this square - if one of
  // these is the only place in this area where the possibility can
  // actually be, then we can solve the square to this number
  const vector<int> values = squareToSolve->Values();

  if (values.size() > 1)
  {
    for(vector<int>::const_iterator it = values.begin(); it!=values.end(); it++)
    {
      // Assume that we can solve the square to this value until we find
      // anything to the contrary
      canSolve = true;
       
      // Check for other squares in the area which have this as a possibility
      for (int i=0; i<Common::NoNumbers; i++)
      {
        if (i != indexToIgnore)
        {
          if(theCollection[i]->CouldBe(*it))
          {
            // We can't solve this square to this value, as atleast one other
            // square in its area has this value as a possible solution
            canSolve = false;
          }
        }
      }

      // If we haven't found any other square in the area which has this
      // possible solution, our square is the only one which can have it
      // therefore solve the square here and stop looking for other
      // solutions
      if (canSolve)
      {
        squareToSolve->SolveAs(*it);
        break;
      }
    }

    // Return whether we have managed to solve the square
    return canSolve;
  }
  else
  {
    // We could in theory be passed a square which has been solved by one
    // of the other solvers so just return false if this is the case
    return false;
  }
}

