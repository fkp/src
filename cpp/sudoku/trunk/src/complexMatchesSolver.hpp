// $Id: complexMatchesSolver.hpp 19 2007-05-02 21:31:02Z fraser $
//

#if !defined(SOLVER_HEADER)
  #define SOLVER_HEADER
  #include <solver.hpp>
#endif

#if !defined(BOARD_HEADER)
  #define BOARD_HEADER
  #include <board.hpp>
#endif

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

class ComplexMatchesSolver : public Solver
{
  private:
    bool CheckForDupSolutions(Square ** squares, Coords coords, int excludeIndex);

  public:
    ComplexMatchesSolver(Board * boardToUse) : Solver(boardToUse) {};
    bool SolveSquare(Coords coords);
    string SolverName();
    const map<string, int> SolverStats();
};
