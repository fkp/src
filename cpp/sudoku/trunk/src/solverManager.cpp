// $Id: solverManager.cpp 24 2007-05-07 12:38:20Z fraser $
//

#include<iostream>

#if !defined(SOLVERMANAGER_HEADER)
  #define SOLVERMANAGER_HEADER
  #include<solverManager.hpp>
#endif

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

SolverManager::SolverManager(Board * boardToUse)
{
  _theBoard = boardToUse;

  // Note that the lower indexes get priority in solving
  _solver[0] = new InCollectionSolver(_theBoard);
  _solver[1] = new UniqueInCollectionSolver(_theBoard);
  _solver[2] = new ComplexMatchesSolver(_theBoard);
}

bool SolverManager::Solve()
{
  Square * thisSquare;
  bool madeProgress;
  int solvedValue;

  if(!_theBoard->BoardValid())
  {
    cout << "Cannot solve as the board state is invalid" << endl;
    return false;
  }

  do
  {
    madeProgress = false;

    for (int x=0; x<Common::NoNumbers; x++)
    {
      for (int y=0; y<Common::NoNumbers; y++)
      {
        Coords coords (x,y);
        thisSquare = _theBoard->GetSquare(coords);

        if (!thisSquare->SolvedValue())
        {
          if (WorkOn(coords) && !madeProgress)
          {
            madeProgress = true;
          }

          solvedValue = thisSquare->SolvedValue();

          if (solvedValue)
          {
            cout << "Square " << coords.ToString() << " solved to: " << solvedValue << endl;
          }
        }
      }
    }
  } while (madeProgress);

  for (int i=0; i<NoSolvers; i++)
  {
    cout << "Solver " << _solver[i]->SolverName() << " has stats: " << endl;
    const map<std::string, int> stats = _solver[i]->SolverStats();

    for (map<std::string, int>::const_iterator it = stats.begin();
      it!= stats.end(); it++)
    {
      cout << "  " << it->first << ": " << it->second << endl;
    }
  }

  if(!_theBoard->BoardValid())
  {
    cout << "Error, the resulting board is invalid" << endl;
    return false;
  }

  // Return whether the whole board is solved
  return _theBoard->IsSolved();
}

bool SolverManager::WorkOn(Coords coords)
{
  // As soon as we make progress in one solver, then stop - this way we get
  // the simple solutions first, then progressing to the more complex solvers
  // only when required
  for(int i=0; i<NoSolvers; i++)
  {
    if (_solver[i]->SolveSquare(coords))
    {
      return true;
    }
  }
  
  return false;
}

SolverManager::~SolverManager()
{
  for (int i=0; i<NoSolvers; i++)
  {
    delete _solver[i];
  }
}

