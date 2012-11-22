// $Id: puzzleGenerator.cpp 23 2007-05-07 12:19:54Z fraser $
//

#if !defined(PUZZLEGENERATOR_HEADER)
  #define PUZZLEGENERATOR_HEADER
  #include <puzzleGenerator.hpp>
#endif

#include <iostream>

using namespace std;

PuzzleGenerator::PuzzleGenerator(BoardBuilder * boardToUse)
{
  _theBoard = boardToUse;
  _solverManager = new SolverManager(_theBoard);
}

PuzzleGenerator::~PuzzleGenerator()
{
  delete _solverManager;
}

bool PuzzleGenerator::Generate()
{
  while (_theBoard->SolveRandomLocation() == true)
  {
    // Remove possibilities from our board which are actually illegal
    _solverManager->Solve();

    // Make a copy of this board and cast it to a regular solvable board
    Board * boardToSolve = new Board((const Board *) _theBoard);

    // Check whether the solve matches any of the solve profiles
    // delete the boardToSolve

    // Display the contents
    _theBoard->DisplayContents();

    if (!_theBoard->BoardValid())
    {
      cout << "Board invalid..." << endl;
      return false;
    }
  }

  return true;
}
