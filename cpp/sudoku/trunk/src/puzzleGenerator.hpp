// $Id: puzzleGenerator.hpp 23 2007-05-07 12:19:54Z fraser $
//

#if !defined(BOARDBUILDER_HEADER)
  #define BOARDBUILDER_HEADER
  #include <boardBuilder.hpp>
#endif

#if !defined(SOLVERMANAGER_HEADER)
  #define SOLVERMANAGER_HEADER
  #include <solverManager.hpp>
#endif

class PuzzleGenerator
{
  private:
    BoardBuilder * _theBoard;
    SolverManager * _solverManager;

  public:
    PuzzleGenerator(BoardBuilder * boardToUse);
    ~PuzzleGenerator();
    bool Generate();
};
